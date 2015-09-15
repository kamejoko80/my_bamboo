/******************************************************************************
**
**  COPYRIGHT (C) 2005, 2006 Intel Corporation.
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
**  FILENAME:	TIM.h
**
**  PURPOSE: 	Defines the Trusted/Non-Trusted Image Module
**
**
******************************************************************************/

#ifndef __TIM_H__
#define __TIM_H__

#include "general.h"

/******************* TIM Layout ************************************/
/* Below Presents the Maxiumum Size TIM layout in memory

VERSION_I      	VersionBind;         			//
FLASH_I        	FlashInfo;           			//
UINT_T         	NumImages;           			//
UINT_T         	NumKeys;						//
UINT_T         	SizeOfReserved;					//
IMAGE_INFO 		IMG[MAX_IMAGES];                //
KEY_MOD        	Key[MAX_KEYS];                  //
UINT_T         	Reserved[0x4E8];       			//
PLAT_DS        	TBTIM_DS;                       //
												//
Below Presents the Maxiumum Size NTIM layout in memory

VERSION_I   	VersionBind;         			//
FLASH_I     	FlashInfo;           			//
UINT_T      	NumImages;           			//
UINT_T         	NumKeys;						//
UINT_T      	SizeOfReserved;					//
IMAGE_INFO 		IMG[MAX_IMAGES];                //
UINT_T      	Reserved[0xD80];       			//
NTIM, *pNTIM;									//
/******************* TIM Layout ************************************/

// TIM Versions
#define TIM_3_1_01			0x30101
#define TIM_3_2_00			0x30200			// Support for Partitioning
#define TIM_3_3_00			0x30300			// Support for ECDSA and 64 bit addressing

// B1 TIM
#include "Typedef.h"
#define MAX_IMAGES			10
#define MAX_KEYS	 		3
#define MAX_TIM_SIZE 		0x1000
#define TIMBUFFER 			4096    		//4k for a Tim structure size
#define MAXRSAKEYSIZEWORDS	 64				// 2048 bits
#define MAXECDSAKEYSIZEWORDS 17				// 521 bits+

// Predefined Image Identifiers
#define TIMIDENTIFIER		0x54494D48		// "TIMH"
#define TIMDUALBOOTID		0x54494D44		// "TIMD"
#define WTMIDENTIFIER		0x57544D49		// "WTMI"
#define OBMIDENTIFIER		0x4F424D49		// "OBMI"
#define MONITORIDENTIFIER	0x4D4F4E49		// "MONI"
#define TZSWIDENTIFIER		0x545A5349		// "TZSI"    This is also used as consumer ID
#define TBRIDENTIFIER		0x54425249		// "TBRI"	 This is also used as consumer ID
#define DKBIDENTIFIER		0x444B4249		// "DKBI"
#define JTAGIDENTIFIER		0x4A544147		// "JTAG"
#define PATCHIDENTIFIER		0x50415443		// "PATC"
#define TCAIDENTIFIER		0x5443414B		// "TCAK"
#define OSLOADERID			0x4F534C4F		// "OSLO"

//Define ID types
//  Use upper 3 bytes as a type identifier and allow up to 256 of a particular type
//
#define TYPEMASK			0xFFFFFF00
#define DDRTYPE				0x444452	 	// "DDR"
#define TIMTYPE				0x54494D		// "TIM"

// WTP Format Recognized Reserved Area Indicator
#define WTPRESERVEDAREAID	0x4F505448      // "OPTH"

// Reserved Area Package Headers
#define AUTOBIND			0x42494E44		// "BIND"
#define TERMINATORID		0x5465726D		// "Term"
#define GPIOID				0x4750494F		// "GPIO"
#define UARTID				0x55415254		// "UART"
#define USBID				0x00555342		// "USB"
#define RESUMEID			0x5265736D		// "Resm"
#define USBVENDORREQ		0x56524551		// "VREQ"
#define TBR_XFER			0x54425258		// "TBRX"
#define RESUMEBLID			0x52736D32		// "Rsm2"
#define ESCAPESEQID			0x45534353		// "ESCS"
#define OEMCUSTOMID			0x43555354		// "CUST"

// DDR Related ID's
#define DDRID				0x44447248		// "DDRH"
#define DDRGID				0x44445247		// "DDRG"
#define DDRTID				0x44445254		// "DDRT"
#define DDRCID				0x44445243		// "DDRC"
#define CMCCID				0x434d4343		// "CMCC"

// Operating mode related ID's
#define FREQID				0x46524551		// "FREQ"
#define VOLTID				0x564f4c54		// "VOLT"
#define OPMODEID			0x4d4f4445		// "MODE"
#define OPDIVID 			0x4f504456		// "OPDV"

//Trustzone
#define TZID                0x545A4944      // "TZID" Field based
#define TZON                0x545A4f4E      // "TZON" Register based

// USB
#define DESCRIPTOR_RES_ID	0x55534200      // "USB"
#define NUM_USB_DESCRIPTORS			10
#define MAX_USB_STRINGS 			7

typedef enum
{
 USB_DEVICE_DESCRIPTOR   				= (DESCRIPTOR_RES_ID | 0),
 USB_CONFIG_DESCRIPTOR   				= (DESCRIPTOR_RES_ID | 1),
 USB_INTERFACE_DESCRIPTOR 				= (DESCRIPTOR_RES_ID | 2),
 USB_LANGUAGE_STRING_DESCRIPTOR   		= (DESCRIPTOR_RES_ID | 3),
 USB_MANUFACTURER_STRING_DESCRIPTOR   	= (DESCRIPTOR_RES_ID | 4),
 USB_PRODUCT_STRING_DESCRIPTOR   	  	= (DESCRIPTOR_RES_ID | 5),
 USB_SERIAL_STRING_DESCRIPTOR   	  	= (DESCRIPTOR_RES_ID | 6),
 USB_INTERFACE_STRING_DESCRIPTOR   		= (DESCRIPTOR_RES_ID | 7),
 USB_DEFAULT_STRING_DESCRIPTOR			= (DESCRIPTOR_RES_ID | 8),
 USB_ENDPOINT_DESCRIPTOR 				= (DESCRIPTOR_RES_ID | 9)
} USB_DESCRIPTORS;


// Global Identifiers
#define FFIDENTIFIER		0x00004646		// "FF"
#define ALTIDENTIFIER		0x00414C54		// "ALT"
#define DIFFIDENTIFIER		0x44696666		// "Diff"
#define SEIDENTIFIER		0x00005345		// "SE"
#define U2DIDENTIFIER		0x55534232		// "USB2"
#define PINSIDENTIFIER		0x50696E73		// "Pins"
#define MDOCFORMATID        0x6d444f43      // "mDOC"
#define MDOCBINID           0x6d42944E      // "mBIN"
#define MDOCBDTLID          0x6d424454	    // "mBDT"
#define CI2IDENTIFIER       0x00434932     // "CI2"
#define OTGIDENTIFIER		0x554F5447		// "UOTG"
#define INVALIDID           0x21212121      // "!!!!" Should never appear in actual TIM file
#define PARTIONIDENTIFIER	0x50415254      // "PART"

// identifier for common tools Marvell Format image file
#define FBFIDENTIFIER       0x46424649      // "FBFI" Should never appear in actual TIM file

// Miscellanous
#define BINDKEYSIZE			128				// (MAXKEYSIZEWORDS * 2)
#define FLASHSIGMASK		0x0000000F		// mask off the 4 bits of platform state

/********** WTP Recognized Reserved Area Layout ********************************
*
*	WTPTP_Defined_Reserved_Format_ID    \	  This clues BR, OBM and DKB that the reserved area is in a known format
*	Number of Reserved Area Packages    /	  For each package there is a header, payload size and payload
*
*	Header	 	 		\	  Indicates what type of a Reserved Area Package
*	Size		  		 \	  Size Comprises a single Reserved Area Package
*	Payload		  		 /	  There may be any number of Packages so long as TIM/NTIM < 4KB
*			 			/
*
*	Header		 		\	  The Last one should be a Package with a Terminator Header
*	Size		 		/	  The size should be 8 bytes (the size of this package)
*
**********************************************************************************/
typedef struct
{
 UINT_T WTPTP_Reserved_Area_ID;	  	// This clues BR, OBM and DKB that the reserved area is in a known format
 UINT_T	NumReservedHeaders;	  		// For each package there is a header, payload size and payload
}WTP_RESERVED_AREA, *pWTP_RESERVED_AREA;

typedef struct
{
 UINT_T Identifier;					// Identification of this reserved area entry
 UINT_T Size;						// Size  = Payload Size + 2 words (8 bytes).
}WTP_RESERVED_AREA_HEADER, *pWTP_RESERVED_AREA_HEADER;

typedef struct
{
 WTP_RESERVED_AREA_HEADER WRAH;
 UINT_T 	bmRequestType;
 UINT_T 	bRequest;
 UINT_T		wValue;
 UINT_T		wIndex;
 UINT_T		wLength;
 UINT_T		wData; 							// First word of the proceeding Data. Note, there may be more traialing data
} USB_VENDOR_REQ, *pUSB_VENDOR_REQ;			// There is no restriction that data need be 32 bit aligned.

typedef struct
{
 volatile int *Addr;
 UINT_T Value;
}GPIO_DEF, *pGPIO_DEF;

typedef struct
{
 WTP_RESERVED_AREA_HEADER WRAH;
 UINT_T XferLoc;						//location to put the XFER table
 UINT_T NumPairs;
 XFER_DATA_PAIR_T * pXferPairs;
}OPT_XFER_SET, *pOPT_XFER_SET;

typedef struct
{
 WTP_RESERVED_AREA_HEADER WRAH;
 UINT_T Port;
 UINT_T Enabled; 					// 1 - Enabled
}OPT_PROTOCOL_SET, *pOPT_PROTCOL_SET;

typedef struct
{
 WTP_RESERVED_AREA_HEADER WRAH;
 UINT_T EscSeqTimeOutMS;  				// Value is interpreted in milli secs.
}OPT_ESCAPE_SEQUENCE, *P_OPT_ESCAPE_SEQUENCE;

typedef struct
{
 WTP_RESERVED_AREA_HEADER WRAH;
 UINT_T NumGpios;
 pGPIO_DEF	GPIO;
}OPT_GPIO_SET, *pOPT_GPIO_SET;

typedef struct
{
 UINT_T ResumeAddr;
 UINT_T ResumeParam;
 UINT_T ResumeFlag;
}OPT_RESUME_DDR_INFO, *pOPT_RESUME_DDR_INFO;

typedef struct
{
 WTP_RESERVED_AREA_HEADER WRAH;
 OPT_RESUME_DDR_INFO ResumeDDRInfo;
}OPT_RESUME_SET, *pOPT_RESUME_SET;

// Do not remove - used by TBR and BootLoader
#define RESUME_FLAG_MASK 0x55AA55AA

typedef struct
{
 UINT_T DDRResumeRecordAddr;
 void	*DDRScratchAreaAddr;
 UINT_T DDRScratchAreaLength;
}OPT_TIM_RESUME_DDR_INFO, *pOPT_TIM_RESUME_DDR_INFO;

typedef struct
{
 WTP_RESERVED_AREA_HEADER	WRAH;
 OPT_TIM_RESUME_DDR_INFO	TimResumeDDRInfo;
}OPT_TIM_RESUME_SET, *pOPT_TIM_RESUME_SET;

typedef struct
{
 WTP_RESERVED_AREA_HEADER WRAH;
 UINT_T ACCR_VALUE;
 UINT_T MDCNFG_VALUE;
 UINT_T DDR_HCAL_VALUE;
 UINT_T MDREFR_VALUE;
}OPT_DDR_SET, *pOPT_DDR_SET;

typedef struct
{
 WTP_RESERVED_AREA_HEADER WRAH;
 UINT_T	ImageAddr;
 UINT_T	ImageSize;
 UINT_T	ImageCRC;
}OPT_RESM_LOC, *pOPT_RESM_LOC;

typedef struct
{
 WTP_RESERVED_AREA_HEADER WRAH;
 UINT_T AutoBind;
}OPT_AUTOBIND, *pOPT_AUTOBIND;

/****  Used only for backwards compatability!!! ***/
typedef struct
{
 UINT_T Identifier;
 UINT_T PortType;
 UINT_T Port;
 UINT_T GPIOPresent;
 UINT_T NumGpios;
 pGPIO_DEF	GPIO;
}OPT_SET, *pOPT_SET;

/*********************************************************************************/

typedef enum
{
	PlatformVerificationKey,
	NetworkOperatorCAKey,
	SoftwareUpdateCAKey,
	DRMDeviceCAKey,
	OEMReservedKey1,
	OEMReservedKey2,
	NUMKEYMODULES
}
KEYMODULES_T;

typedef enum
{
	Marvell_DS = 0,
	PKCS1_v1_5_Caddo = 1,
	PKCS1_v2_1_Caddo = 2,
	PKCS1_v1_5_Ippcp = 3,
	PKCS1_v2_1_Ippcp = 4,
	ECDSA_256 = 5,
	ECDSA_521 = 6
}
ENCRYPTALGORITHMID_T;

#define Intel_DS Marvell_DS

typedef enum
{
	SHA160 = 20,
	SHA256 = 32
}
HASHALGORITHMID_T;

typedef struct
{
 UINT_T Version;
 UINT_T	Identifier;					// "TIMH"
 UINT_T Trusted;					// 1- Trusted, 0 Non
 UINT_T IssueDate;
 UINT_T OEMUniqueID;
} VERSION_I, *pVERSION_I;			// 0x10 bytes

typedef struct
{
 UINT_T WTMFlashSign;
 UINT_T WTMEntryAddr;
 UINT_T WTMEntryAddrBack;
 UINT_T WTMPatchSign;
 UINT_T WTMPatchAddr;
 UINT_T BootFlashSign;
} FLASH_I, *pFLASH_I;				// 0x10 bytes

typedef struct
{
 UINT_T ImageID;					// Indicate which Image
 UINT_T NextImageID;				// Indicate next image in the chain
 UINT_T FlashEntryAddr;			 	// Block numbers for NAND
 UINT_T LoadAddr;
 UINT_T ImageSize;
 UINT_T ImageSizeToHash;
 UINT_T HashAlgorithmID;            // See HASHALGORITHMID_T
 UINT_T Hash[8];					// Reserve 256 bits for the hash
 UINT_T PartitionNumber;			// This is new for V3.2.0
} IMAGE_INFO, *pIMAGE_INFO;			// 0x40 bytes

typedef struct
{
 UINT_T ImageID;					// Indicate which Image
 UINT_T NextImageID;				// Indicate next image in the chain
 UINT_T FlashEntryAddr;			 	// Block numbers for NAND
 UINT_T LoadAddr;
 UINT_T ImageSize;
 UINT_T ImageSizeToHash;
 UINT_T HashAlgorithmID;            // See HASHALGORITHMID_T
 UINT_T Hash[8];					// Reserve 256 bits for the hash
} IMAGE_INFO_3_1_0, *pIMAGE_INFO_3_1_0; 	// 0x3C bytes

typedef struct
{
 UINT_T	KeyID;						// Associate an ID with this key
 UINT_T HashAlgorithmID;            // See HASHALGORITHMID_T
 UINT_T KeySize;					// Specified in bits
 UINT_T PublicKeySize;				// Specified in bits
 UINT_T RSAPublicExponent[MAXRSAKEYSIZEWORDS]; // Contents depend on PublicKeySize
 UINT_T RSAModulus[MAXRSAKEYSIZEWORDS]; // Up to 2K bits
 UINT_T KeyHash[8]; 				// Reserve 256 bits for the hash
} KEY_MOD, *pKEY_MOD;				// 0x22C bytes

#if RVCT
#pragma anon_unions
#endif
typedef struct
{
 UINT_T KeyID;				// Associate an ID with this key
 UINT_T HashAlgorithmID;    // See HASHALGORITHMID_T
 UINT_T KeySize;			// Specified in bits
 UINT_T PublicKeySize;		// Specified in bits
 UINT_T EncryptAlgorithmID;	// See ENCRYPTALGORITHMID_T;
 union
 {
	struct
	{
 		UINT_T RSAPublicExponent[MAXRSAKEYSIZEWORDS];	// Contents depend on PublicKeySize
		UINT_T RSAModulus[MAXRSAKEYSIZEWORDS];			// Up to 2K bits
	}Rsa;

	struct
	{
        UINT_T PublicComponentA[MAXECDSAKEYSIZEWORDS]; // Contents depend on PublicKeySize
        UINT_T PublicComponentB[MAXECDSAKEYSIZEWORDS]; // Up to 521 bits
        // Pad this struct so it remains consistent with RSA struct
	    UINT_T Reserved[(2*MAXRSAKEYSIZEWORDS)-(2*MAXECDSAKEYSIZEWORDS)];
	}Ecdsa;
 };

 UINT_T KeyHash[8]; 				// Reserve 256 bits for the hash
} KEY_MOD_3_3, *pKEY_MOD_3_3;		//

typedef struct
{
 UINT_T DSAlgorithmID; 		  // See ENCRYPTALGORITHMID_T
 UINT_T HashAlgorithmID;      // See HASHALGORITHMID_T
 UINT_T KeySize;	          // Specified in bits
 UINT_T Hash[8];			  // Reserve 256 bits for optional key hash
 union 	// Note that this union should not be included as part of the hash for TIM in the Digital Signature
 {
	struct
	{
		UINT_T RSAPublicExponent[MAXRSAKEYSIZEWORDS];
		UINT_T RSAModulus[MAXRSAKEYSIZEWORDS];           	// Up to 2K bits
		UINT_T RSADigS[MAXRSAKEYSIZEWORDS];				// Contains TIM Hash
	}Rsa;

	struct
	{
		UINT_T ECDSAPublicKeyCompX[MAXECDSAKEYSIZEWORDS]; // Allow for 544 bits (17 words, 68 bytes for use with EC-521)
		UINT_T ECDSAPublicKeyCompY[MAXECDSAKEYSIZEWORDS];
		UINT_T ECDSADigS_R[MAXECDSAKEYSIZEWORDS];
		UINT_T ECDSADigS_S[MAXECDSAKEYSIZEWORDS];
		// Pad this struct so it remains consistent with RSA struct
		UINT_T Reserved[(MAXRSAKEYSIZEWORDS*3)-(MAXECDSAKEYSIZEWORDS*4)];
	} Ecdsa;
 };
} PLAT_DS, *pPLAT_DS;

// Constant part of the TIMs
typedef struct
{
 VERSION_I      VersionBind;         			// 0
 FLASH_I        FlashInfo;           			// 0x10
 UINT_T         NumImages;           			// 0x20
 UINT_T         NumKeys;						// 0x24
 UINT_T         SizeOfReserved;					// 0x28
} CTIM, *pCTIM;									// 0x2C

// TIM structure for use by DKB/OBM/BootROM
typedef struct
{
 pCTIM 			pConsTIM;			// Constant part
 pIMAGE_INFO	pImg;				// Pointer to Images
 pKEY_MOD_3_3	pKey;				// Pointer to Keys
 PUINT			pReserved;			// Pointer to Reserved Area
 pPLAT_DS       pTBTIM_DS;			// Pointer to Digital Signature
} TIM, *pTIM;

// NTIM structure for use by DKB/OBM/BootROM
typedef struct
{
 pCTIM 			pConsTIM;			// Constant part
 pIMAGE_INFO	pImg;				// Pointer to Images
 PUINT			pReserved;			// Pointer to Reserved Area
} NTIM, *pNTIM;


/* tim.c Function Proto's */
UINT_T SetTIMPointers( UINT8_T * StartAddr, TIM *pTIM_h);
UINT_T LoadTim(UINT8_T *TIMArea, TIM *pTIM_h, UINT_T SRAMLoad);
pIMAGE_INFO ReturnPImgPtr(pTIM pTIM_h, UINT_T ImageNumber);
UINT_T GetTIMValidationStatus();
void SetTIMValidationStatus(UINT_T status);
pIMAGE_INFO FindImageInTIM(pTIM, UINT_T);
pWTP_RESERVED_AREA_HEADER FindPackageInReserved (UINT_T * Retval, pTIM pTIM_h, UINT_T Identifier);
// These 2 functions allow for multiple instances of one "TYPE" of
// package to be retrieved from the reserved area of the TIM
//-----------------------------------------------------------------------------------------------------
pWTP_RESERVED_AREA_HEADER FindFirstPackageTypeInReserved (UINT_T * Retval, pTIM pTIM_h, UINT_T Identifier);
pWTP_RESERVED_AREA_HEADER FindNextPackageTypeInReserved (UINT_T * Retval);
UINT_T CheckReserved (pTIM pTIM_h);
UINT_T CheckAutoBindPacket(pTIM pTIM_h);
UINT_T InitTIMPort( pFUSE_SET pFuses, pTIM pTIM_h);
void InitDefaultPort(pFUSE_SET pFuses);
#endif
