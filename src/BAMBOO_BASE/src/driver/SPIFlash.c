/******************************************************************************
 *
 *  (C)Copyright 2005 - 2008 Marvell. All Rights Reserved.
 *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF MARVELL.
 *  The copyright notice above does not evidence any actual or intended
 *  publication of such source code.
 *  This Module contains Proprietary Information of Marvell and should be
 *  treated as Confidential.
 *  The information in this file is provided for the exclusive use of the
 *  licensees of Marvell.
 *  Such users have the right to use, modify, and incorporate this code into
 *  products for purposes authorized by the license agreement provided they
 *  include this notice and the associated copyright notice with any such
 *  product.
 *  The information in this file is provided "AS IS" without warranty.
 *
 ******************************************************************************/

#include "SPIFlash.h"
#include "Errors.h"
#include "Flash.h"
#include "gpio.h"
//#include "misc.h"
#include "PlatformConfig.h"
//#include "platform_interrupts.h"
//#include "xllp_dmac.h"

extern void EnableIrqInterrupts();
extern void DisableIrqInterrupts();

const SPI_COMMANDS_T *pSPI_commands;

SPI_DEVICE_INFO_T const spi_devices[] = {
//	ID 	1 2	3  Capacity  Sector Sz Page Sz
	{0x202013, 0x080000, 0x10000,  0x100},	//Numonyx:	0x20	 4Mb	64Kb	256B
	{0x202016, 0x400000, 0x10000,  0x100},	//ST Micro:	0x20	32Mb	64Kb	256B
	{0x010215, 0x400000, 0x10000,  0x100},	//Spansion:	0x20	32Mb	64Kb	256B
	{0x1F6604, 0x080000, 0x10000,  0x100},	//Atmel:	0x1F	 4Mb	64Kb	256B
	{0x1F2800, 0x800000, 0x02000,  0x400},	//Atmel:	0x1F	64Mb	 8Kb	 4KB
	{0,0,0,0}	//terminator
};
//note: for Atmel, we use Blocks for eraseing, so all variables named Sector refer to Atmel 'blocks'

SPI_COMMANDS_T const spi_command_set[] = {
//	 READ STATUS WrEN  Wr1   Wr2   Erase ECLen       WaitBit
	{0x03, 0x05, 0x06, 0x02, NULL, 0xD8, 0xC7,       0x01, 0x01},	//Numonyx M25P40
	{0x03, 0x05, 0x06, 0x02, NULL, 0xD8, 0xC7,       0x01, 0x01},	//ST Micro M25P32
	{0x03, 0x05, 0x06, 0x02, NULL, 0xD8, 0xC7,       0x01, 0x01},	//Spansion S25FL032P
	{0x03, 0x05, 0x06, 0x02, NULL, 0xD8, 0xC7,       0x01, 0x01},	//Atmel AT25FS040
	{0x03, 0xD7, NULL, 0x84, 0x88, 0x50, 0xC794809A, 0x04, 0x80},	//Atmel AT45DB642D
	{0,0,0,0,0,0,0} //terminator
};

// make SPI_ID global so its persistent and easy to locate in a debugger.
unsigned int SPI_ID;

FlashProperties_T FlashProp[2];  // 0 - Boot Flash, 1- Caddo Save State Flash
static FlashBootType_T CurrentFlashBootType;

P_FlashProperties_T GetFlashProperties(FlashBootType_T FlashBootType)
{
    if (FlashBootType == BOOT_FLASH)
        return &FlashProp[0];
    else
        return &FlashProp[1];
}

void SetCurrentFlashBootType(FlashBootType_T FlashBootType)
{
     CurrentFlashBootType = FlashBootType;
     return;
}

//endian conversion function: self explanatory
unsigned int Endian_Convert (unsigned int in)
{
	unsigned int out;
	out = in << 24;
	out |= (in & 0xFF00) << 8;
	out |= (in & 0xFF0000) >> 8;
	out |= (in & 0xFF000000) >> 24;
	return out;
}

/***********************************************************
*    InitializeSPIFlashDevice()
*       Initializes the SDMMC port on the platform and all
*       of the other necessary hardware needed for SDMMC
*       reading.
*    input:
*       none
*    output:
*       All the needed hardware should be initialized.
*    returns:
*       Error Status
************************************************************/
UINT_T InitializeSPIDevice(UINT8_T FlashNum, FlashBootType_T FlashBootType)
{
	unsigned int Retval = NoError;
	unsigned int i, ID;

	P_FlashProperties_T pFlashP = GetFlashProperties(FlashBootType);
	pSPI_commands = NULL; //null out pointer

	ChipSelectSPI();

	//set GPIO 110 as output
	spi_reg_bit_set((GPIO3_BASE | GPIO_PDR), BIT14);
	//assert CS - set GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PSR), BIT14);


	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);
	reg_bit_set(SSP_CR0, SSP_CR0_SSE);

	// copy command to the data buffer
	*SSP_DR = SPI_CMD_RELEASE_POWER_DOWN;
	//deassert CS - clear GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PCR), BIT14);

	// wait till TxFIFO is empty - means command has finished
	while (*SSP_SR & 0xF10);
	//assert CS - set GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PSR), BIT14);

	// issue Read JEDEC ID
	SPI_ReadID(&SPI_ID);
	for(i = 0; (spi_devices[i].SPI_ID != 0); i++)
	{
		if((SPI_ID & 0xFFFFFF) == spi_devices[i].SPI_ID)
		{
			//setup Flash Properties info
			pFlashP->BlockSize = spi_devices[i].SPI_SECTOR_SIZE;
		    pFlashP->PageSize = spi_devices[i].SPI_PAGE_SIZE;
			pFlashP->NumBlocks = spi_devices[i].SPI_CAPACITY / spi_devices[i].SPI_SECTOR_SIZE;
			//setup correct Commands
			pSPI_commands = &spi_command_set[i];
			//clear Retval, so we know we found a good device
			Retval = NoError;
			break;
		}
	}
	//default to Numonyx case
	if(pSPI_commands == NULL)
	{	//setup Flash Properties info
		pFlashP->BlockSize = spi_devices[0].SPI_SECTOR_SIZE;
	    pFlashP->PageSize = spi_devices[0].SPI_PAGE_SIZE;
		pFlashP->NumBlocks = spi_devices[0].SPI_CAPACITY / spi_devices[0].SPI_SECTOR_SIZE;
		//setup correct Commands
		pSPI_commands = &spi_command_set[0];
	}

	SetCurrentFlashBootType(FlashBootType);
    //define functions
    pFlashP->ReadFromFlash = &SPI_Read;
    pFlashP->WriteToFlash = &SPI_Write;
    pFlashP->EraseFlash =  &SPI_Erase;
    pFlashP->WipeFlash =  &SPI_Wipe;
    pFlashP->ResetFlash = NULL;
    pFlashP->FlashSettings.UseBBM = 0;
    pFlashP->FlashSettings.UseSpareArea = 0;
    pFlashP->FlashSettings.SASize = 0;
    pFlashP->FlashSettings.UseHwEcc = 0;
	pFlashP->StreamingFlash = FALSE;
	pFlashP->StagedInitRequired = FALSE;
	pFlashP->FlashType = SPI_FLASH;
	pFlashP->FinalizeFlash = NULL;
    //---------------------------------------
	pFlashP->TimFlashAddress = 0;


	return Retval;
}


// Note: DMA enabled SPI reads are the default...
#ifndef SPI_PIO

// structures required for dma-based reads:
//	descriptors:
//		pTX_Desc[0]: transmit descriptor, used for transmitting the four byte command.
//		pRX_Desc[0]: receive descriptor,  used for getting the dummy word generated while the command is transmitted.
//		pTX_Desc[1]: transmit descriptor, used to keep the clocks going to pull the receive data in. just loops resending a single word.
//		pRX_Desc[1]: receive descriptor,  controls the transfer of data into the input buffer
//	transmit/receive buffers:
//		tx1:         transmit buffer, contains the command (read opcode + 3 byte address)
//		rx1:		 receive buffer,  recieves the four bytes pulled in while the command is being sent out.
//		tx2:		 transmit buffer, single word, value not important. purpose is to keep the clock running while rx data is pulled in.
//		Buffer:		 receive buffer,  input parameter, points to place to store incoming data from the SPI.

// all buffers (per DMA spec) must be 16 byte aligned.
__attribute__ ((aligned(16)))XLLP_DMAC_DESCRIPTOR_T	TX_Desc0;			// sends the command
__attribute__ ((aligned(16)))XLLP_DMAC_DESCRIPTOR_T	TX_Desc1;			// used for getting the data - just keeps the clock running.
__attribute__ ((aligned(16)))XLLP_DMAC_DESCRIPTOR_T	RX_Desc0;			// garbage data during command transmission
__attribute__ ((aligned(16)))XLLP_DMAC_DESCRIPTOR_T	RX_Desc1;			// used for getting the data
// why not use an array for the above descriptors? because if the structure length every changes from being a multiple
// of 16 bytes (the alignment requirement), then the other elements in the array would not be aligned properly.

__attribute__ ((aligned(16)))unsigned long	tx1_command;		// contains command opcode and address
__attribute__ ((aligned(16)))unsigned long	rx1_fromcommand;	// receive garbage clocked in while command is transmitted
__attribute__ ((aligned(16)))unsigned long	tx2_runclocks;		// word to transmit to keep clocks running while pulling in data
// end of structure allocation...


// SPI READ: DMA - with linked descriptors
unsigned int SPI_Read(unsigned int FlashOffset, unsigned int Buffer, unsigned int Size, FlashBootType_T FlashBootType)
{
	int i=0;
	unsigned int	*buff;
	unsigned int	read_size, total_size;
	DMA_CMDx_T		RX_Cmd, TX_Cmd;
	P_XLLP_DMAC_DESCRIPTOR_T pRX_Desc[2], pTX_Desc[2];	//need 4 descriptors: 2 for RX, 2 for TX

	//turn off interrupts during the Read
	DisableIrqInterrupts();

	// * initialize variables * //
	//save off the buffer pointer - for endian convert at end of routine
	buff = (unsigned int *)Buffer;
	//remember how much needs to be Read - for endian convert at the end of routine
	total_size = Size;
	//keep track of size per transfer
	read_size = Size > 4096 ? 4096 : Size;	//find out how much to read (max 4k for DMA)
	Size -= read_size;	//capture how much is left to be read

	//need to put tx entries on a 64bit aligned address

	//tx0 - load the SPI command and address (0xCmd_AddrHI_AddrMid_AddrLow)
	tx1_command  = (pSPI_commands->SPI_READ_CMD << 24) | (FlashOffset & 0x00FFFFFF);
	tx1_command  = Endian_Convert(tx1_command);  //endian convert because of how TX fifo drains
	//tx2 - dummy word that will be used to fill TX fifo when reading
	tx2_runclocks = 0xb2b4b6ba;	// value not important. but this pattern is easily recognizable during debug.

	//carve out the descriptor space
	pRX_Desc[0] = &RX_Desc0;
	pRX_Desc[1] = &RX_Desc1;

	pTX_Desc[0] = &TX_Desc0;
	pTX_Desc[1] = &TX_Desc1;

	//fill out commands
	RX_Cmd.value = 0;
	RX_Cmd.bits.IncSrcAddr = 0;
	RX_Cmd.bits.IncTrgAddr = 1;
	RX_Cmd.bits.FlowSrc = 1;
	RX_Cmd.bits.FlowTrg = 0;
	RX_Cmd.bits.Width = 1;
	RX_Cmd.bits.MaxBurstSize = 1;

	TX_Cmd.value = 0;
	TX_Cmd.bits.IncSrcAddr = 0;
	TX_Cmd.bits.IncTrgAddr = 0;
	TX_Cmd.bits.FlowSrc = 0;
	TX_Cmd.bits.FlowTrg = 1;
	TX_Cmd.bits.Width = 1;
	TX_Cmd.bits.MaxBurstSize = 1;

	// * configure RX descriptors * //
	configDescriptor(pRX_Desc[0], pRX_Desc[1],(unsigned int)SSP_DR, (unsigned int)&rx1_fromcommand, &RX_Cmd, 4,         0);
	configDescriptor(pRX_Desc[1], NULL,       (unsigned int)SSP_DR, (unsigned int)Buffer,           &RX_Cmd, read_size, 1);

	// * configure TX descriptors * //
	configDescriptor(pTX_Desc[0], pTX_Desc[1], (unsigned int)&tx1_command,   (unsigned int)SSP_DR, &TX_Cmd, 4,         0);
	configDescriptor(pTX_Desc[1], NULL,        (unsigned int)&tx2_runclocks, (unsigned int)SSP_DR, &TX_Cmd, read_size, 1);

	//calculate next read size
	Buffer += read_size;
	read_size = Size > 4096 ? 4096 : Size;

	//setup rest of DMA
	//Map Device to Channels
	XllpDmacMapDeviceToChannel(SSP_RX_DMA_DEVICE, SSP_RX_CHANNEL);
	XllpDmacMapDeviceToChannel(SSP_TX_DMA_DEVICE, SSP_TX_CHANNEL);

	//Load descriptors
	loadDescriptor (pRX_Desc[0], SSP_RX_CHANNEL);
	loadDescriptor (pTX_Desc[0], SSP_TX_CHANNEL);

	//turn ON user alignment - in case buffer address is 64bit aligned
	alignChannel(SSP_RX_CHANNEL, 1);

	//reset SSP CR's
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL | 0x300483);

	//fire SSP up
	reg_bit_set(SSP_CR0, SSP_CR0_SSE);

	//deassert CS - clear GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PCR), BIT14);

	//Kick off DMA's
	XllpDmacStartTransfer(SSP_RX_CHANNEL);
	XllpDmacStartTransfer(SSP_TX_CHANNEL);

	//read until Size = 0
	while(Size > 0)
	{
		// debug
        // if(i++ < 20)
        // write_status(0x11111111, *SSP_SR);

		//check and restart TX and RX dma's
		if( (readDmaStatusRegister(SSP_TX_CHANNEL) & XLLP_DMAC_DCSR_STOP_INTR) &&
			(readDmaStatusRegister(SSP_RX_CHANNEL) & XLLP_DMAC_DCSR_STOP_INTR) )
		{
			//update descriptor (LENGTH field gets updated)
			configDescriptor(pRX_Desc[1], NULL, (unsigned int)SSP_DR,         (unsigned int)Buffer, &RX_Cmd, read_size, 1);
			configDescriptor(pTX_Desc[1], NULL, (unsigned int)&tx2_runclocks, (unsigned int)SSP_DR, &TX_Cmd, read_size, 1);
			loadDescriptor (pRX_Desc[1], SSP_RX_CHANNEL);
			loadDescriptor (pTX_Desc[1], SSP_TX_CHANNEL);
			XllpDmacStartTransfer(SSP_RX_CHANNEL);
			XllpDmacStartTransfer(SSP_TX_CHANNEL);
			//after setting up dma, recalculate the counters
			Size -= read_size;
			Buffer += read_size;
			read_size = Size > 4096 ? 4096 : Size;
		}
	}

	//make sure last DMA is finished
	while((readDmaStatusRegister(SSP_RX_CHANNEL) & XLLP_DMAC_DCSR_STOP_INTR) == 0);

	//assert CS - set GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PSR), BIT14);

	//make sure SSP is disabled
	reg_bit_clr(SSP_CR0, SSP_CR0_SSE);
	//reset SSP CR's
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);

	//clear out DMA settings
	XllpDmacUnMapDeviceToChannel(SSP_RX_DMA_DEVICE, SSP_RX_CHANNEL);
	XllpDmacUnMapDeviceToChannel(SSP_TX_DMA_DEVICE, SSP_TX_CHANNEL);

	//turn interrupts back on
	EnableIrqInterrupts();

	return 0;
}

#else
// this will be a PIO based SPI read

/***********************************************************
*   SPI_Read
*      Reads from the SPI device
*   Returns:
*      none
*************************************************************/
unsigned int SPI_Read(unsigned int FlashOffset, unsigned int Buffer, unsigned int Size, FlashBootType_T FlashBootType)
{
	int i=0;
	unsigned int dummy[4];
	unsigned int temp;
	unsigned int *buff = (unsigned int *) Buffer;


	//reset SSP CR's
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);
	//setup in 32bit mode
	reg_bit_set(SSP_CR0, SSP_CR0_DSS_32);
	//fire it up
	reg_bit_set(SSP_CR0, SSP_CR0_SSE);

	//load the command and address
	temp = pSPI_commands->SPI_READ_CMD << 24;
	temp |= FlashOffset & 0xFFFFFF;

	//deassert CS - clear GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PCR), BIT14);

	*SSP_DR = temp;
	*SSP_DR = 0;

	//read the 4 dummy IN bytes
	do{ //make sure we're reading a good entry
		if((*SSP_SR & 0x8) == 0x8)
			dummy[i++] = *SSP_DR;
	}while (i < 1);


	//convert size from Bytes to DWORDs
	  Size >>= 2;
	//now read
	for(i = 0; i < Size;)
	{
		//write a value, so device will continue to read
		*SSP_DR = 0;
		//make sure we're reading a good entry
		while(!(*SSP_SR & 0x8));
		buff[i] = *SSP_DR;
		i++;
	}


	//assert CS - set GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PSR), BIT14);

	//make sure SSP is disabled
	reg_bit_clr(SSP_CR0, SSP_CR0_SSE);
	//reset SSP CR's
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);

	//postprocessing... endian convert
	for(i = 0; i < Size; i++)
		buff[i] = Endian_Convert(buff[i]);

	return 0;
}

#endif	// Select between DMA vs PIO based SPI reads




/***********************************************************
*   SPI_ReadStatus
*      Reads the status of SPI device.  Has option to wait for
*	   Write in Progress to finish
*   Inputs:
*	  Wait - waits till the WIP (or BUSY) bit gets cleared before returning
*		    valid values:  TRUE or FALSE
*   Returns:
*      last status value
*************************************************************/
unsigned int SPI_ReadStatus(unsigned int Wait)
{
	int i=0, test;
	unsigned int read, ready, dummy, status;

	status = NoError;
	read = FALSE;	//this flag gets set when we read first entry from fifo
	//if the caller waits to 'Wait' for the BUSY to be cleared, start READY off as FALSE
	//if the caller doesn't wait to wait, set READY as true, so we don't wait on the bit
	ready = (Wait) ? FALSE : TRUE;

	// note that one complete "send command/read response"
	// transaction is issued for each status register read.
	//
	// this is becomes some SPI devices have one to one
	// correlation between commands and status register reads.
	//
	// other SPI devices can operate in a mode where a valid
	// status register value is returned for each read after
	// getting just one read status opcode.
	//
	// using this 1 to 1 mode will cover parts of all types.

	do{
		//make sure SSP is disabled
		reg_bit_clr(SSP_CR0, SSP_CR0_SSE);
		//reset SSP CR's
		reg_write(SSP_CR0, SSP_CR0_INITIAL);
		reg_write(SSP_CR1, SSP_CR1_INITIAL);
		//fire it up
		reg_bit_set(SSP_CR0, SSP_CR0_SSE);

		//assert nCS - clear GPIO 110
		spi_reg_bit_set((GPIO3_BASE | GPIO_PCR), BIT14);

		//load the command
		*SSP_DR = pSPI_commands->SPI_READ_STAUS_CMD;
		*SSP_DR = 0;

		//read dummy byte
		while((*SSP_SR & 0x8) != 0x8);
		dummy = *SSP_DR;

		/////// Need to add a timeout!?

		//now read
		*SSP_DR = 0;										// write an entry so peripheral will continue to read
		while((*SSP_SR & 0x8) != 0x8);						// make sure we're reading a good entry

		//set the READ flag, and read the status
		read = TRUE;
		status = *SSP_DR;

        test = pSPI_commands->SPI_WAIT_BIT;

		//set the READY flag if the status wait bit is cleared
		if((status & pSPI_commands->SPI_WAIT_BIT) == 0)		// operation complete (eg. not busy)?
			ready = TRUE;

		//deassert nCS - set GPIO 110
		spi_reg_bit_set((GPIO3_BASE | GPIO_PSR), BIT14);

		//make sure SSP is disabled
		reg_bit_clr(SSP_CR0, SSP_CR0_SSE);
		//reset SSP CR's
		reg_write(SSP_CR0, SSP_CR0_INITIAL);
		reg_write(SSP_CR1, SSP_CR1_INITIAL);

		//we need to wait until we read at least 1 valid status entry
		//if we're waiting for the Write, wait till WIP bits goes to 0
	}while ((!read) || (!ready));


	//return last known status
	return status;
}



/***********************************************************
*   SPI_WriteEnable - Tell the device to set the Write Enable Latch (WEL) bit
*************************************************************/
void SPI_WriteEnable()
{
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);
	reg_bit_set(SSP_CR0, SSP_CR0_SSE);
	//deassert CS - clear GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PCR), BIT14);
	//load the command
	*SSP_DR = pSPI_commands->SPI_WRITE_ENABLE_CMD;

	//wait till TX fifo is empty
	while((*SSP_SR & 0xF00) != 0);
	//assert CS - set GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PSR), BIT14);
	//no need to clear SSE bit, as normal operation will
	// terminate session once the TX Fifo becomes empty.
	return;
}


/***********************************************************
*   SPI_ReadID
*      Reads the JEDEC ID of the device.
*************************************************************/
void SPI_ReadID(unsigned int *buffer)
{
	int i=0;
	unsigned int status, dummy;
	unsigned int read = FALSE;

	//make sure SSP is disabled
	reg_bit_clr(SSP_CR0, SSP_CR0_SSE);
	//reset SSP CR's
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);
	//need to use 32bit data
	reg_bit_set(SSP_CR0, SSP_CR0_DSS_32);
	//fire it up
	reg_bit_set(SSP_CR0, SSP_CR0_SSE);

	//load the command + 3 zero bytes
	*SSP_DR = (SPI_CMD_JEDEC_ID << 24);
	//deassert CS - clear GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PCR), BIT14);

	// wait till TxFIFO is empty - means command has finished
	while (*SSP_SR & 0xF10);
	//assert CS - set GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PSR), BIT14);


	//now read the entries out
	*buffer = *SSP_DR;

	//make sure SSP is disabled
	reg_bit_clr(SSP_CR0, SSP_CR0_SSE);
	//reset SSP CR's
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);

	return;
}


/***********************************************************
*    SPI_Write_1Stage()
*       breaks input data into pages, to call page write routine
*		makes sure status bits and WEL are set
*	  Returns:
*       Error reason
************************************************************/
unsigned int SPI_Write(unsigned int Address, unsigned int Buffer, unsigned int Size, FlashBootType_T FlashBootType)
{
	unsigned int status, write_size;
	unsigned int page_sz, page_mask;
	P_FlashProperties_T pFlashP = GetFlashProperties(FlashBootType);

	page_sz = pFlashP->PageSize;
	page_mask = page_sz - 1;
	//initial case: we can only program up to the end of a page
	//so if we start writing in the middle of a page, make sure
	//the amount we write (on the first Write CMD) is only till
	//the end of the page.
	write_size = page_sz - (Address & page_mask);
	//check to make we don't write too much
	write_size = write_size > Size ? Size : write_size;
	//make sure device is ready to be programmed
	status = SPI_ReadStatus(TRUE);

	do {
		//get device ready to Program
		SPI_WriteEnable();
		//write current page
		SPI_write_stage1(Address, Buffer, write_size);
		if(pSPI_commands->SPI_WRITE_STAGE2_CMD != NULL)
			SPI_write_stage2(Address);
		//update counters
		Address += write_size;
		Buffer += write_size;
		Size -= write_size;
		//at most, we can program 256 bytes at a time
		write_size = Size > page_sz ? page_sz : Size;
	} while(Size > 0);

	return 0;
}

/***********************************************************
*    SPI_page_write()
*       internal routine that writes a page
*		this routine does NOT check parameters for dubious cases, as
*		that is the job of the higher level routine
*    returns:
*       none
************************************************************/
void SPI_page_write(unsigned int Address, unsigned int Buffer, unsigned int Size)
{
	unsigned int temp;
	int i;
	unsigned int dummy[SSP_MAX_TX_SIZE_WORDS];
	unsigned int *buff = (unsigned int *) Buffer;

	//convert size from Bytes to DWORDs
	Size >>= 2;

	for(i = 0; i < Size; i++)
		dummy[i] = Endian_Convert(buff[i]);

	//make sure SSP is disabled
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);
	//need to use 32bit data, only way to keep up
	reg_bit_set(SSP_CR0, SSP_CR0_DSS_32);
	//cset the TX fifo threshold
	reg_bit_set(SSP_CR1, 0x000003C0);
	//fire it up
	reg_bit_set(SSP_CR0, SSP_CR0_SSE);

	//load the command
	temp = pSPI_commands->SPI_WRITE_STAGE1_CMD << 24;
	temp |= Address & 0xFFFFFF;
	//deassert CS - clear GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PCR), BIT14);
	*SSP_DR = temp;

	//load data
	for(i = 0; i < Size;)
	{	//make sure TX fifo isn't full
		if(*SSP_SR & 0x4)
		{	//load entry
			*SSP_DR = dummy[i];
			i++;
		}
	}

	//wait for TX fifo to empty AND busy signal to go away
	while((*SSP_SR & 0xF10) != 0x0);
	//assert CS - set GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PSR), BIT14);
	//make sure SSP is disabled
	reg_bit_clr(SSP_CR0, SSP_CR0_SSE);
	//reset SSP CR's
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);

	return;
}

/***********************************************************
*    SPI_write_stage1()
*       internal routine that sends the data for the write command
*    returns:
*       none
************************************************************/
void SPI_write_stage1(unsigned int Address, unsigned int Buffer, unsigned int Size)
{
	unsigned int i, wr_amt;

	for(i = 0; i < Size; )
	{
		//break the data transfer into blocks (default = 256B)
		wr_amt = Size >	SSP_MAX_TX_SIZE_BYTES ? SSP_MAX_TX_SIZE_BYTES : Size;
		//send a block
		SPI_page_write(Address + i, Buffer + i, wr_amt);
		//update counter
		i += wr_amt;
		//wait for write to finish (if applicable)
		SPI_ReadStatus(TRUE);
	}
	return;
}

/***********************************************************
*    SPI_write_stage2()
*       internal routine that sends the second part of the write command
*    returns:
*       none
************************************************************/
void SPI_write_stage2(unsigned int Address)
{
	unsigned int temp;

	//make sure SSP is disabled
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);
	//need to use 32bit data, only way to keep up
	reg_bit_set(SSP_CR0, SSP_CR0_DSS_32);
	//fire it up
	reg_bit_set(SSP_CR0, SSP_CR0_SSE);

	//load the command
	temp = pSPI_commands->SPI_WRITE_STAGE2_CMD << 24;
	temp |= Address & 0xFFFFFF;
	//deassert CS - clear GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PCR), BIT14);
	*SSP_DR = temp;

	//wait for TX fifo to empty AND busy signal to go away
	while((*SSP_SR & 0xF10) != 0x0);
	//assert CS - set GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PSR), BIT14);
	//make sure SSP is disabled
	reg_bit_clr(SSP_CR0, SSP_CR0_SSE);
	//reset SSP CR's
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);

	return;
}


/***********************************************************
*    SPI_Erase()
*       Erases any sector touched by Address->Address+Size
*    returns:
*       none
************************************************************/
unsigned int SPI_Erase(unsigned int Address, unsigned int Size, FlashBootType_T FlashBootType)
{
	unsigned int sector_mask, sector_size;
	P_FlashProperties_T pFlashP = GetFlashProperties(FlashBootType);

	sector_size = pFlashP->BlockSize;
	sector_mask = sector_size - 1;
	//align to the beginning of a sector
	//NOTE: the next two lines do nothing if Address is already sector aligned
	Size += Address & sector_mask; //increment the Size to account for the whole first sector
	Address &= ~sector_mask; //removes the lower bytes of Address (i.e. removes offset into sector)

	do{
		//make sure the device is ready for the command
		SPI_ReadStatus(TRUE);
		SPI_WriteEnable();
		//erase this sector
		SPI_erase_sector(Address);
		//increment counting stats
		Size -= Size > sector_size ? sector_size : Size;
		Address += sector_size;
	}while(Size > 0);

	return 0;
}

/***********************************************************
*    SPI_erase_sector()
*       internal routine that erases a sector
*    returns:
*       none
************************************************************/
void SPI_erase_sector(unsigned int address)
{
	unsigned int temp;

	//make sure SSP is disabled
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);
	//set into 32bit mode - easy since only TXing 32 bits
	reg_bit_set(SSP_CR0, SSP_CR0_DSS_32);
	//fire it up
	reg_bit_set(SSP_CR0, SSP_CR0_SSE);

	//load the command
	temp = pSPI_commands->SPI_ERASE_CMD << 24;
	temp |= address & 0xFFFFFF;
	//deassert CS - clear GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PCR), BIT14);
	*SSP_DR = temp;

	//wait for TX fifo to empty AND busy signal to go away
	while((*SSP_SR & 0xF10) != 0x0);
	//assert CS - set GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PSR), BIT14);
	//make sure SSP is disabled
	reg_bit_clr(SSP_CR0, SSP_CR0_SSE);
	//reset SSP CR's
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);

	return;
}


/***********************************************************
*    SPI_Wipe()
*       Erases the entire chip
*    returns:
*       none
************************************************************/
unsigned int SPI_Wipe(FlashBootType_T FlashBootType)
{
	P_FlashProperties_T pFlashP = GetFlashProperties(FlashBootType);
	unsigned int		temp;


	//make sure the device is ready for the command
	SPI_ReadStatus(TRUE);

	//enable program mode. this is required for any program, eg: erase, program page or otp or write status reg.
	SPI_WriteEnable();


	// sequence for issuing the wipe command (aka chip erase, aka bulk erase)
	//make sure SSP is disabled
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);


	//format the command
	temp = pSPI_commands->SPI_WIPE_CMD;

	// select the data sample size depending on the command length
	if( pSPI_commands->SPI_WIPE_CMDLEN == 4 )
	{
		reg_bit_set(SSP_CR0, SSP_CR0_DSS_32);
	}

	//fire it up...but nothing will happen until the fifo is written.
	reg_bit_set(SSP_CR0, SSP_CR0_SSE);

	//deassert CS - clear GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PCR), BIT14);

	// write the command to the fifo. this starts the spi clock running and the command appears on the bus.
	*SSP_DR = temp;


	//wait for TX fifo to empty AND busy signal to go away
	while((*SSP_SR & 0xF10) != 0x0);
	//now the command has been completely sent.


	//assert CS - set GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PSR), BIT14);
	//make sure SSP is disabled
	reg_bit_clr(SSP_CR0, SSP_CR0_SSE);
	//reset SSP CR's
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);



	//don't return until the operation completes.
	SPI_ReadStatus(TRUE);	// wait for the last erase operation to complete.
	return 0;
}

