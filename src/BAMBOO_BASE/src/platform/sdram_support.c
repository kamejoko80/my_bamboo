/******************************************************************************
**  COPYRIGHT  2007 Marvell Inernational Ltd.
**  All Rights Reserved
******************************************************************************/


#include "sdram_support.h"
#include "uart.h"

// ReadBitField
// Read a bit field within a 32bit group to a specified value.
unsigned long ReadBitField (unsigned volatile long * pReg,
	                        unsigned long bf_lsb, // lsb location of bit field
	                        unsigned long bf_len  // number of bits in the fields
)
{
	unsigned long	val;

	val = *pReg;
	val = val<<(32-(bf_lsb+bf_len));
	val = val>>(32-bf_len);

	return val;
}


// ConfigRecordCount:
// Convert the NumBytes field of an xxxSpecList to a number of xxxSpec records value.
int ConfigRecordCount (void *l)
{
	int	c;

	c = ( ((DDRGSpecList_T*)l)->NumBytes - 2 * sizeof( unsigned long ) ) / sizeof( DDRGSpec_T );
	return c;
}


// CheckMemoryReliability:
// Do a simple read/modify/write/read-back/validate across the specified range.
// Input:
//	       startaddr: address to start checking
//	       length:    number of bytes to check.
//         faultaddr: address of any read-mismatch
// Return value:
//   0  = no errors
//   !0 = error
unsigned long CheckMemoryReliability (unsigned long startaddr, unsigned long length, unsigned long *faultaddr)
{
	unsigned long			original, modified, readverify;
	volatile unsigned long *pMem;
	pMem = (volatile unsigned long*)startaddr;

	while(length)
	{
 #if 1
		original = *pMem;					// get what's currently there.
		modified = original^0xffffffff;	// invert every bit
		*pMem = modified;				// write it back
		readverify = *pMem;				// read it back for comparison with what was written.

		if(readverify != modified)
		{
			*faultaddr = (unsigned long)pMem;
			PutStr("Init DDR2 RAM Failed\r\n");
			return 1;					// error code
		}

#else
       #define PATTERN 0xAAAAAAAA
       *pMem = PATTERN;
        readverify = *pMem;
        if(readverify!=PATTERN)
        {
            *faultaddr = (unsigned long)pMem;
             PutStr("Init DDR2 RAM Failed\r\n");
             return 1;					// error code
        }
#endif
		++pMem;							// increases by sizeof(unsigned long)
		length-=sizeof(unsigned long);
	}

	PutStr("Init DDR2 RAM successfully!\r\n");
	return 0;
}
