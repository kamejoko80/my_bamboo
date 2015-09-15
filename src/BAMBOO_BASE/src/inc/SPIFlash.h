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

/***********************************************************
*    SPIFlash.h
*       This is the header file for the SPIFlash driver.
*
*    Revision History:
*       5/2/07 - initial draft
************************************************************/
#ifndef __SPIFLASH_H__
#define __SPIFLASH_H__

#include "Typedef.h"
#include "SSP.h"
#include "Flash.h"
#include "predefines.h"


/* Supported SPI Devices */
typedef struct{
	unsigned int SPI_ID;
	unsigned int SPI_CAPACITY;
	unsigned int SPI_SECTOR_SIZE;
	unsigned int SPI_PAGE_SIZE;
} SPI_DEVICE_INFO_T, *P_SPI_DEVICE_INFO_T;

/*
* SPI Command Set
*/
#define SPI_CMD_JEDEC_ID			0x009F
#define SPI_CMD_RELEASE_POWER_DOWN	0x00AB

typedef struct{
	unsigned char SPI_READ_CMD;
	unsigned char SPI_READ_STAUS_CMD;
	unsigned char SPI_WRITE_ENABLE_CMD;
	unsigned char SPI_WRITE_STAGE1_CMD;
	unsigned char SPI_WRITE_STAGE2_CMD;
	unsigned char SPI_ERASE_CMD;
	unsigned int  SPI_WIPE_CMD;				// erases the entire chip
	unsigned char SPI_WIPE_CMDLEN;
	unsigned char SPI_WAIT_BIT;
	unsigned char SPI_READY;
} SPI_COMMANDS_T, *P_SPI_COMMANDS_T;


/*
* SSP Register locations.
*
* Note:  SSP_BASE_FOR_SPI set in platform_config.h
*/

#define SSP_CR0	((volatile int *) (SSP_BASE_FOR_SPI+SSP_SSCR0))	// SSP Control Register 0
#define SSP_CR1	((volatile int *) (SSP_BASE_FOR_SPI+SSP_SSCR1))	// SSP Control Register 1
#define SSP_SR	((volatile int *) (SSP_BASE_FOR_SPI+SSP_SSSR))	// SSP Status Register
#define SSP_DR	((volatile int *) (SSP_BASE_FOR_SPI+SSP_SSDR))	// SSP Data Write/Read Register
#define SSP_TO  ((volatile int *) (SSP_BASE_FOR_SPI+SSP_SSTO))
#define SSP_SP	((volatile int *) (SSP_BASE_FOR_SPI+SSP_SSPSP))	// SSP Serial Protocol Register

//Control Register special values
#define SSP_CR0_INITIAL 	(SSP_SSCR0_TIM | SSP_SSCR0_RIM | 0x7)
#define SSP_CR0_SSE			SSP_SSCR0_SSE
#define SSP_CR0_FPCKE		SSP_SSCR0_FPCKE
#define SSP_CR0_DSS_32		SSP_SSCR0_EDSS | 0xF
#define SSP_CR0_DSS_16		0x0000000F

#define SSP_CR1_INITIAL 	SSP_SSCR1_TTELP | SSP_SSCR1_TTE
#define SSP_CR1_RWOT		SSP_SSCR1_RWOT

//limited for timing and stack concerns (see SPI_page_write routine)
#define SSP_MAX_TX_SIZE_WORDS		64
#define SSP_MAX_TX_SIZE_BYTES		SSP_MAX_TX_SIZE_WORDS << 2

//function definitions
UINT_T InitializeSPIDevice(UINT8_T FlashNum, FlashBootType_T FlashBootType);
unsigned int SPI_Read(unsigned int FlashOffset, unsigned int Buffer, unsigned int Size, FlashBootType_T FlashBootType);
unsigned int SPI_Write(unsigned int Address, unsigned int Buffer, unsigned int Size, FlashBootType_T FlashBootType);
unsigned int SPI_Erase(unsigned int Address, unsigned int Size, FlashBootType_T FlashBootType);
unsigned int SPI_Wipe(FlashBootType_T FlashBootType);

//internal functions
unsigned int SPI_ReadStatus(unsigned int Wait_for_Write);
void SPI_ReadID(unsigned int *buffer);
void SPI_WriteEnable();
void SPI_WriteDisable();
void SPI_page_write(unsigned int Address, unsigned int Buffer, unsigned int Size);
void SPI_write_stage1(unsigned int Address, unsigned int Buffer, unsigned int Size);
void SPI_write_stage2(unsigned int Address);
void SPI_erase_sector(unsigned int address);

#endif /* __SPIFLASH_H__ */
