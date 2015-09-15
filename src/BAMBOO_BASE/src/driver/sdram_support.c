/******************************************************************************
**  COPYRIGHT  2007 Marvell Inernational Ltd.
**  All Rights Reserved
******************************************************************************/


#include "sdram_support.h"

// SetBitField:
//  Force a bit field within a 32bit group to a specified value.
unsigned long
SetBitField
(
	unsigned long *pReg,
	unsigned long val,			// values to be stored in field
	unsigned long bf_lsb,		// lsb location of bit field
	unsigned long bf_len		// number of bits in the fields
)
{
	unsigned long	reg = *pReg;
	unsigned long	clear_mask;

	// clear out the current field contents.
	clear_mask = (0xffffffff<<(bf_lsb+bf_len)) | (0xffffffff>>(32-bf_lsb));	// create a sequences of 0s with 1s to the left and right
	reg &= clear_mask;										// the 0s will clear out whatever's currently there

	// mask val to bf_len just in case it's bigger than expected -
	// we don't want to overwrite adjacent fields
	val = (val<<(32-bf_len))>>(32-bf_len);

	// put in the new value
	reg |= (val<<bf_lsb);

	// update the input variable
	*pReg = reg;

	return reg;
}
// SetSplitBitField:
// Force two bit fields within a 32bit group to a specified value.
unsigned long
SetSplitBitField
(
	unsigned long *pReg,
	unsigned long val,			// values to be stored in field
	unsigned long bf_lsb_low,	// lsb location of bit field
	unsigned long bf_len_low,	// number of bits in the fields
	unsigned long bf_lsb_hi,	// lsb location of bit field
	unsigned long bf_len_hi		// number of bits in the fields
)
{
	unsigned long	reg = *pReg;
	unsigned long	clear_mask,clear_mask_hi, clear_mask_low, temp;

	// clear out the current field contents.
	clear_mask_low = (0xffffffff<<(bf_lsb_low+bf_len_low)) | (0xffffffff>>(32-bf_lsb_low));	// create a sequences of 0s with 1s to the left and right
	clear_mask_hi = (0xffffffff<<(bf_lsb_hi+bf_len_hi)) | (0xffffffff>>(32-bf_lsb_hi));	    // create a sequences of 0s with 1s to the left and right
	clear_mask = clear_mask_hi & clear_mask_low;
	reg &= clear_mask;										// the 0s will clear out whatever's currently there

	// mask val to the total number of bits availabel just in case it's bigger than expected -
	// we don't want to overwrite adjacent fields
	val = (val<<(32-(bf_len_hi+bf_len_low)))>>(32-(bf_len_hi+bf_len_low));

    // OR in the low part of the new value
	temp = (val<<(32-bf_len_low))>>(32-bf_len_low); // extract least significant portion of val
	reg |= (temp<<bf_lsb_low);

   // OR in the hi part of the new value
	temp = val>>(32-bf_len_low); // extract most significant portion of val
	reg |= (temp<<bf_lsb_hi);

	// update the input variable
	*pReg = reg;

	return reg;
}

// ReadBitField
// Read a bit field within a 32bit group to a specified value.

unsigned long ReadBitField
(
	unsigned volatile long * pReg,
	unsigned long bf_lsb,		// lsb location of bit field
	unsigned long bf_len		// number of bits in the fields
)
{
	unsigned long	val;

	val = *pReg;
	val = val<<(32-(bf_lsb+bf_len));
	val = val>>(32-bf_len);

	return val;
}

// CR_banks: convert banks to a bit field
//	input: args[0] = banks value from SDRAM DDR spec.
//	output: banks register field value
//           this routine may have to be moved to the platform dependent code
//           if the translation algorithm changes in the future.
//
unsigned long
CR_banks(
	unsigned long *args
	)
{
	return args[0]>>2;
}

// CR_rows: convert rowsto a bit field
//	input: args[0] = rows value from SDRAM DDR spec.
//	output: rows register field value
//           this routine may have to be moved to the platform dependent code
//           if the translation algorithm changes in the future.
//
unsigned long
CR_rows(
	unsigned long *args
	)
{
	return args[0]-10;
}

// CR_columns: convert banks to a bit field
//	input: args[0] = columns value from SDRAM DDR spec.
//	output: columns register field value
//           this routine may have to be moved to the platform dependent code
//           if the translation algorithm changes in the future.
//
unsigned long
CR_columns(
	unsigned long *args
	)
{
	return args[0]-7;
}

// CR_megabytes: convert megabytes to a bit field
//	input: args[0] = megabytes value from SDRAM DDR spec. Eg, 128 for 128 megabyte part.
//	output: megabytes register field value
//           this routine may have to be moved to the platform dependent code
//           if the translation algorithm changes in the future.
//
// Implement the following table:
//	0x0 to 0x6 = Reserved
//	0x7 = 8 MB
//	0x8 = 16 MB
//	0x9 = 32 MB
//	0xA = 64 MB
//	0xB = 128 MB
//	0xC = 256 MB
//	0xD = 512 MB
//	0xE = 1024 MB
//	0xF = 2048 MB

unsigned long
CR_megabytes(
	unsigned long *args
	)
{
	unsigned long bitpos=3;
	unsigned long arg;

	// assume the size is a power of two, find the exponent.
	arg=args[0]>>bitpos;
	while(arg!=1)
	{
		arg>>=1;
		++bitpos;
	}
	return bitpos+4;
}

// CR_none: no conversion
//	input: args[0] = any value.
//	output: input value
//
unsigned long
CR_none(
	unsigned long *args
	)
{
	return args[0];
}

// CR_ns_to_clk: convert a number of ns to a number of tCK units.
//	input: args[0] = ns,
//         args[1] = clk
//	output: tCK
//
// note: tCK = ps * f / 1000000000.
//       do main calculations scaled up by a 1000 so floating
//       point is unnecessary. round up and unscale for final result.
unsigned long
CR_ns_to_clk(
	unsigned long *args
	)
{
	return ( args[1] / 1000000 * args[0] + 999 ) / 1000;
}

// CR_us_to_clk: convert a number of us to a number of tCK units.
//	input: args[0] = us,
//         args[1] = clk
//	output: tCK
//
// note: tCK = us * f / 1000000.
//       do main calculations scaled up by a 1000 so floating
//       point is unnecessary. round up and unscale for final result.

unsigned long
CR_us_to_clk(
	unsigned long *args
	)
{
	return ( args[1]  / 1000 * args[0] + 999 ) / 1000;
}


// ConfigRecordCount:
// Convert the NumBytes field of an xxxSpecList to a number of xxxSpec records value.
int
ConfigRecordCount(
	void *l
	)
{
	int	c;

	c = ( ((DDRGSpecList_T*)l)->NumBytes - 2 * sizeof( unsigned long ) ) / sizeof( DDRGSpec_T );
	return c;
}


// CheckMemoryReliability:
// Do a simple read/modify/write/read-back/validate across the specified range.
// Input:
//	startaddr: address to start checking
//	length:    number of bytes to check.
//  faultaddr: address of any read-mismatch
// Return value:
//   0  = no errors
//   !0 = error
unsigned long
CheckMemoryReliability(
	unsigned long	startaddr,
	unsigned long	length,
	unsigned long	*faultaddr
	)
{
	unsigned long			original, modified, readverify;
	volatile unsigned long *pMem;

	pMem = (volatile unsigned long*)startaddr;
	while(length)
	{
		original=*pMem;					// get what's currently there.
		modified=original^0xffffffff;	// invert every bit
		*pMem = modified;				// write it back
		readverify=*pMem;				// read it back for comparison with what was written.

		if(readverify!=modified)
		{
			*faultaddr = (unsigned long)pMem;
			return 1;					// error code
		}

		++pMem;							// increases by sizeof(unsigned long)
		length-=sizeof(unsigned long);
	}
	return 0;
}
