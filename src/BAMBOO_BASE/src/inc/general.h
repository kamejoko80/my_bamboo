/******************************************************************************
**
**  COPYRIGHT (C) 2002, 2003 Intel Corporation.
**
**  This software as well as the software described in it is furnished under
**  license and may only be used or copied in accordance with the terms of the
**  license. The information in this file is furnished for informational use
**  only, is subject to change without notice, and should not be construed as
**  a commitment by Intel Corporation. Intel Corporation assumes no
**  responsibility or liability for any errors or inaccuracies that may appear
**  in this document or any software that may be provided in association with
**  this document. 
**  Except as permitted by such license, no part of this document may be 
**  reproduced, stored in a retrieval system, or transmitted in any form or by  
**  any means without the express written consent of Intel Corporation. 
**
**  FILENAME:	General.h
**
**  PURPOSE: 	Contain boot ROM definitions used across all components
**                  
******************************************************************************/
#ifndef _general_h
#define _general_h

#include "Typedef.h"
//#include "loadoffsets.h"

/*
 *  Fuse definition structure
 */
struct PLATFORM_FUSES
{
	unsigned int DebugDisable		:1;		// Bit 0 of SYS_BOOT_CNTRL and bit 79 dis_strap_override of AP config bits 
	unsigned int PlatformState		:4;		// Bit 62-65 of AP config bits and bits 22-25 of SYS_BOOT_CNTRL
	unsigned int NoNAND				:1;		// Bit 17 of SYS_BOOT_CNTRL
	unsigned int JTAG_Disable		:1;		// Bit 6 of SYS_BOOT_CNTRL	
	unsigned int UseWTM				:1;		// Bit 20 of SYS_BOOT_CNTRL (MMP2 and MRNA only)
	unsigned int Reserved0			:3;		// 
	unsigned int EscapeSeqDisable	:1;		// Bit 17 of SYS_BOOT_CNTRL (Tavor TD only)
	unsigned int USBDisable			:1;		// Bits 12 USBDisable of SYS_BOOT_CNTRL
	unsigned int UARTDisable		:1;		// Bits 13 UARTDisable of SYS_BOOT_CNTRL
	unsigned int MMCDisable			:1;		// Bit 14 MMC Enable of SYS_BOOT_CNTRL
	unsigned int Resume				:1;		//  
	unsigned int USBWakeup			:1;		// Bit 16 of SYS_BOOT_CNTRL
	unsigned int PortEnabled		:1;		// Bit 17: 1 = Ports have been enabled by bootROM
	unsigned int DDRInitialized		:1; 	// Bit 18: If the BootROM already initializes the DDR
	unsigned int Download_Disable	:1;		// Bit 19 of SYS_BOOT_CNTRL
	unsigned int SBE				:1;		// Bit 20 of SYS_BOOT_CNTRL (Not for MRNA or MMP2)
	unsigned int OverrideDisable	:1;		// Bit 78 of AP fuse config bits dis_aib_override
	unsigned int BootPlatformState	:4;		// Bits 22-25: There are the bits the BootROM booted from
    unsigned int USBPort            :2;     // Bits 26-27: 0 - DIFFUSB 1 - SEUSB
    unsigned int UARTPort           :2;     // Bits 28-29: 0 - FFUART, 1 - AltUART
	unsigned int TBROpMode			:2;		// Bits 30-31: Bits 10,11 od SYS_BOOT_CNTRL (MRNA and MMP2) only.
};

typedef enum 
{
	FFUART_PORT = 0,
	ALTUART_PORT = 1
} UART_PORTS; 

typedef enum 
{
	USB_DIFF_PORT = 0,
	USB_SE_PORT = 1,
	USB_U2D_PORT = 2,
	USB_CI2_PORT = 3
} USB_PORTS; 


typedef union{
	unsigned int value;
	struct PLATFORM_FUSES bits;
}FUSE_SET, *pFUSE_SET;

//list of IDS for use in XFER structure
typedef enum
{
	TIM_DATA = 0x54494D48,	// "TIMH" - refers to tim image
	PT_DATA = 0x4D505420, 	// "MPT " - refers to partition table
	BBT_DATA = 0x4D424254, 	// "MBBT" - refers to bad block table
	RD_DATA = 0x52444953	// "RDIS" - refers to read disturb list
} XFER_DATA;

typedef struct
{
	XFER_DATA data_id;
	UINT_T location;
}	XFER_DATA_PAIR_T, *P_XFER_DATA_PAIR_T;

// This is the Transfer Structure.
//  The first word is the id = 'TBRX'
//  The next few words are fixed data
//  lastly, there is a key/value array to indicate what the bootrom is passing to next image
typedef struct
{
	UINT_T TransferID;
	UINT_T SOD_ACCR0;
	UINT_T FuseVal;
	UINT_T ErrorCode;
	UINT_T ResumeParam[4];
	UINT_T num_data_pairs;
	XFER_DATA_PAIR_T data_pairs[1];
} TRANSFER_STRUCT, *P_TRANSFER_STRUCT;

// **************** Flash Boot State Offsets *************
// Default TIM flash locations
#define TIMOffset_CS2						0x00000000
#define TIMOffset_NAND						0x00000000
#define TIMOffset_MDOC						0x00000000	// Could be in partition 2
#define TIMOffset_ONENAND					0x00000000
#define TIMOffset_SDMMC						0x00000000	// Could be in Partition 1

// For debug purposes only
#define HEX_LED_CONTROL (volatile unsigned long *)( 0x08000040 )
#define HEX_LED_WRITE (volatile unsigned long *)( 0x08000010 )


#endif




