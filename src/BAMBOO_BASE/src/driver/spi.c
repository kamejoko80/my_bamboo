#include "SPIFlash.h"
#include "gpio.h"

void SPI_ReadID(unsigned int *buffer);
void SPI_erase_sector(unsigned int address);
unsigned int SPI_ReadStatus(unsigned int Wait);
void SPI_WriteEnable(void);

void SPI_Init(void)
{
    unsigned int SPI_ID;
    unsigned int *pID = &SPI_ID;

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

	SPI_ID = SPI_ID;

}

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
	temp = 0xD8 << 24;
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

unsigned int SPI_ReadStatus(unsigned int Wait)
{
	int i=0;
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
		*SSP_DR = 0x05;
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

		//set the READY flag if the status wait bit is cleared
		if((status & 0x01) == 0)		// operation complete (eg. not busy)?
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


void SPI_WriteEnable(void)
{
	reg_write(SSP_CR0, SSP_CR0_INITIAL);
	reg_write(SSP_CR1, SSP_CR1_INITIAL);
	reg_bit_set(SSP_CR0, SSP_CR0_SSE);
	//deassert CS - clear GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PCR), BIT14);
	//load the command
	*SSP_DR = 0x06;

	//wait till TX fifo is empty
	while((*SSP_SR & 0xF00) != 0);
	//assert CS - set GPIO 110
	spi_reg_bit_set((GPIO3_BASE | GPIO_PSR), BIT14);
	//no need to clear SSE bit, as normal operation will
	// terminate session once the TX Fifo becomes empty.
	return;
}

unsigned int Endian_Convert (unsigned int in)
{
	unsigned int out;
	out = in << 24;
	out |= (in & 0xFF00) << 8;
	out |= (in & 0xFF0000) >> 8;
	out |= (in & 0xFF000000) >> 24;
	return out;
}

unsigned int SPI_Read(unsigned int FlashOffset, unsigned int Buffer, unsigned int Size)
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
	temp = 0x03 << 24;
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
		if((*SSP_SR & 0x8) == 0x8)
		{
			buff[i] = *SSP_DR;
			i++;
		}
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
