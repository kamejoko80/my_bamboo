/******************************************************************************
**  COPYRIGHT  2007 Marvell Inernational Ltd.
**  All Rights Reserved
******************************************************************************/

#ifndef __SDRAM_SUPPORT_H__
#define __SDRAM_SUPPORT_H__

#include "Typedef.h"

// DDR Geometry descriptors
typedef struct DDRGSpec_S
{
	unsigned long	KeyId;
	unsigned long	KeyValue;
} DDRGSpec_T;

typedef struct DDRGSpecList_S
{
	unsigned long	PID;			// package id
	int				NumBytes;
	DDRGSpec_T		DDRGSpecs[1];
} DDRGSpecList_T;



// DDR Timing descriptors
typedef struct DDRTSpec_S
{
	unsigned long	KeyId;
	unsigned long	KeyValue;
} DDRTSpec_T;

typedef struct DDRTSpecList_S
{
	unsigned long	PID;			// package id
	int				NumBytes;
	DDRTSpec_T		DDRTSpecs[1];
} DDRTSpecList_T;



// DDR Custom descriptors
typedef struct DDRCSpec_S
{
	unsigned long	KeyId;
	unsigned long	KeyValue;
} DDRCSpec_T;

typedef struct DDRCSpecList_S
{
	unsigned long	PID;			// package id
	int				NumBytes;
	DDRCSpec_T		DDRCSpecs[1];
} DDRCSpecList_T;



// Frequency descriptors
typedef struct FreqSpec_S
{
	unsigned long	KeyId;
	unsigned long	KeyValue;
} FreqSpec_T;

typedef struct FreqSpecList_S
{
	unsigned long	PID;			// package id
	int				NumBytes;
	FreqSpec_T		FreqSpecs;
} FreqSpecList_T;



// Clock enable descriptors
typedef struct CLKESpec_S
{
	unsigned long	KeyId;
	unsigned long	KeyValue;
} CLKESpec_T;

typedef struct CLKESpecList_S
{
	unsigned long	PID;			// package id
	int				NumBytes;
	CLKESpec_T		CLKESpecs[1];
} CLKESpecList_T;



// Voltage descriptors
typedef struct VoltSpec_S
{
	unsigned long	Start;
	unsigned long	Stop;
	unsigned long	Value;
} VoltSpec_T;

typedef struct VoltSpecList_S
{
	unsigned long	PID;			// package id
	int				NumBytes;
	VoltSpec_T		VoltSpecs[1];
} VoltSpecList_T;



// Configure Memory Controller Control descriptors
typedef struct CMCCSpec_S
{
	unsigned long	KeyId;
	unsigned long	KeyValue;
} CMCCSpec_T;

typedef struct CMCCSpecList_S
{
	unsigned long	PID;			// package id
	int				NumBytes;
	CMCCSpec_T		CMCCSpecs[1];
} CMCCSpecList_T;

typedef enum CMCC_E
{
	CMCC_CONFIG_ENA_ID,
	CMCC_MEMTEST_ENA_ID,
	CMCC_CONSUMER_ID
} CMCC_T;


// Trustzone descriptors
typedef struct TZSpec_S
{
	unsigned long	KeyId;
	unsigned long	KeyValue;
} TZSpec_T;

typedef struct TZSpecList_S
{
	unsigned long	PID;			// package id
	int				NumBytes;
	TZSpec_T		TZSpecs[1];
} TZSpecList_T;


// function prototypes
// bit field manipulation:
unsigned long SetBitField  ( unsigned long *pReg, unsigned long val, unsigned long bf_lsb, unsigned long bf_len );
unsigned long SetSplitBitField( unsigned long *pReg, unsigned long val,unsigned long bfs_hi, unsigned long bfl_hi, unsigned long bfs_low, unsigned long bfl_low );
unsigned long ReadBitField ( unsigned volatile long *pReg, unsigned long bf_lsb,	unsigned long bf_len );

// conversion routines:
unsigned long CR_banks(	unsigned long * args ) ;

unsigned long CR_rows( unsigned long * args );
 
unsigned long CR_columns( unsigned long * args );

unsigned long CR_megabytes( unsigned long * args );

unsigned long CR_none( unsigned long * args );

unsigned long CR_ns_to_clk(	unsigned long * args );

unsigned long CR_us_to_clk(	unsigned long * args );


// record parser helper routines:
int ConfigRecordCount( void *pSpecList );

// reliablity routines: read/modify/write verifier:
unsigned long CheckMemoryReliability( unsigned long	startaddr, unsigned long length, unsigned long *faultaddr );

// Trustzone configuration
void ConfigureTrustZone ( void *pTIM );
UINT_T TzGetRegion (UINT_T region, UINT_T ChipSelect, UINT_T* pBaseAddressOffset, UINT_T* pSize, UINT_T* pPermission);

#endif

