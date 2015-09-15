/******************************************************************************
**  COPYRIGHT  2007 Marvell Inernational Ltd.
**  All Rights Reserved
******************************************************************************/

#ifndef __SDRAM_CONFIG_H__
#define __SDRAM_CONFIG_H__

UINT_T ConfigureMemoryController( void *pTIM, void *pDDRScratchAreaAddress, unsigned long ulDDRScratchAreaLength );
UINT_T GetDDRSize();
#endif

