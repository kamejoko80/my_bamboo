/******************************************************************************
**  COPYRIGHT  2007 Marvell Inernational Ltd.
**  All Rights Reserved
******************************************************************************/

/*
Sample extended reserved area to specify operating mode 3, 800_400, at 1050 mV is below:
*
      Extended Reserved Data:
      Processor Type: PXA168
      Frequency:
      ASPEN_OPMODE_3_ID: 1050
      End Extended Reserved Data:
*

Specifying the voltage is optional. Use 0 to select the POR default.
*/


#include "predefines.h"
#include "mcu.h"
#include "mcu_extras.h"
#include "processor_enums.h"
#include "sdram_specs.h"
#include "sdram_support.h"
#include "sdram_registers_defaults.h"
#include "tim.h"
#include "Errors.h"

#include "PMUM.h"
// missing from PMUM.h:
#define	PMUM_PLL2REG1	(PMUM_BASE+0x0060)	/* 32 bit	PLL2 Register 1: KVCO, etc. */
#define	PMUM_PLL2REG2	(PMUM_BASE+0x0064)	/* 32 bit	PLL2 Register 2: DIFFCLK, etc */

#include "PMUA.h"

#include "BootLoader.h"

// FIXME: move these to an include file....

// Operating Mode Specifier
typedef struct OPMDSpec_S
{
	unsigned long	KeyId;
	unsigned long	KeyValue;
} OPMDSpec_T;

typedef struct OPMDSpecList_S
{
	unsigned long	PID;			// package id: 'OPMD' 0x4f504d44
	int				NumBytes;
	OPMDSpec_T		OPMDSpecs[0];
} OPMDSpecList_T;




// Operating Mode Divider Specifier
typedef struct OPDVSpec_S
{
	unsigned long	KeyId;
	unsigned long	KeyValue;
} OPDVSpec_T;

typedef struct OPDVSpecList_S
{
	unsigned long	PID;			// package id: 'OPDV' 0x4f504456
	int				NumBytes;
	OPDVSpec_T		OPDVSpecs[0];
} OPDVSpecList_T;




// this is used to provide operating mode parameters specified with dividers.
typedef struct OPMODE_DIV_S
{
	unsigned long	pll_sel;		// MPMU_FCCR:		0=pll1, 1=pll2
	unsigned long	axipll_sel;		// MPMU_FCCR:		0=pll1, 1=pll2
	unsigned long	aclk2_div;		// APMU_CCR:		axi2 clock, aka bus2_clk_div
	unsigned long	aclk_div;		// APMU_CCR:		is this hclk, also? axi clock, aka bus_clk_div
	unsigned long	dclk_div;		// APMU_CCR:		ddr clock
	unsigned long	xpclk_div;		// APMU_CCR:		aka L2clk - L2 clock
	unsigned long	baclk_div;		// APMU_CCR:		aka BIUclk - bus interface clock
	unsigned long	pclk_div;		// APMU_CCR:		processor clock
	unsigned long	pll2_refdiv;	// MPMU_PLL2CR:
	unsigned long	pll2_fbdiv;		// MPMU_PLL2CR:
	unsigned long	pll2_reg1;		// MPMU_PLL2REG1
	unsigned long	mV;
} OPMODE_DIV_T;







// forward decs...defined in this file.
void SetVoltage(unsigned long mV);
void SetOpMode( unsigned long om);
unsigned long SetOpModeByDividers( OPMODE_DIV_T *om, unsigned long mV );
void DelayEx(unsigned long d);


// new values determined vai JIRA sighting DPF-790
// values for use when configuring by mode. the voltage can be overridden.                                                l2clk bus  a/h
//										pll apl a2  a   d   xp  ba  p  ref   fb	 pll2 reg1    mV			//	 pcl  dcl  xp   ba   a    a2    mv    vrng kvco pdiv pll  mode
	OPMODE_DIV_T	opmode_div_5 	= { 2,  0,  0,  1,  0,  2,  3,  0,   1, 650, 0x900C0664, 1100 };		//  1200, 400, 400, 300, 156, 312, 1100    5,   7,   1,  1213 5
	OPMODE_DIV_T	opmode_div_4_1	= { 2,  0,  0,  1,  0,  2,  3,  0,   1, 635, 0x900C0664, 1050 };		//  1066, 355, 355, 266, 156, 312, 1050    4,   6,   0,  1083 4_1
	OPMODE_DIV_T	opmode_div_4 	= { 2,  0,  0,  1,  0,  1,  3,  0,   1, 635, 0x90040664, 1100 };		//  1066, 533, 533, 265, 156, 312, 1100    4,   6,   0,  1083 4
	OPMODE_DIV_T	opmode_div_3_3	= { 2,  0,  0,  1,  1,  1,  3,  0,   1, 604, 0x90020464,  970 };		//   800, 200, 400, 200, 156, 312,  970    2,   3,   0,  806  3_3
	OPMODE_DIV_T	opmode_div_3_2	= { 2,  0,  0,  1,  0,  1,  3,  0,   1, 604, 0x90020464, 1050 };		//   800, 400, 400, 200, 156, 312, 1050    2,   3,   0,  806  3_2
	OPMODE_DIV_T	opmode_div_3_1	= { 2,  0,  0,  1,  1,  1,  3,  0,   1, 604, 0x90020464, 1050 };		//   800, 200, 400, 200, 156, 312, 1050    2,   3,   0,  806  3_1
	OPMODE_DIV_T	opmode_div_3 	= { 2,  0,  0,  1,  0,  1,  3,  0,   1, 604, 0x90020464, 1050 };		//   800, 400, 400, 200, 156, 312, 1050    2,   3,   0,  806  3
	OPMODE_DIV_T	opmode_div_2_4	= { 2,  0,  1,  1,  1,  1,  3,  0,   1, 656, 0x91140664, 1050 };		//   624, 156, 312, 156, 156, 156, 1050                  624  2_4
	OPMODE_DIV_T	opmode_div_2_3	= { 2,  0,  1,  1,  1,  1,  3,  0,   1, 656, 0x91140664,  970 };		//   624, 156, 312, 156, 156, 156,  970                  624  2_3
	OPMODE_DIV_T	opmode_div_2_2	= { 1,  0,  1,  1,  0,  1,  3,  0,   0,   0, 0x00000000, 1050 };		//   624, 312, 312, 156, 156, 156, 1050                  624  2_2
	OPMODE_DIV_T	opmode_div_2_1	= { 2,  0,  1,  1,  0,  1,  3,  0,   1, 656, 0x91240664, 1050 };		//   624, 208, 312, 156, 156, 156, 1050	   4,   6,   0,  xxxx 2_1
	OPMODE_DIV_T	opmode_div_2 	= { 1,  0,  1,  1,  1,  1,  3,  0,   0,   0, 0x00000000, 1050 };		//   624, 312, 312, 156, 156, 156, 1050                  624  2
	OPMODE_DIV_T	opmode_div_1 	= { 2,  0,  1,  1,  0,  1,  1,  0,   1, 604, 0x91120464,  900 };		//   400, 200, 200, 200, 156, 156,  900	   0,   1,   0,  404  1
	OPMODE_DIV_T	opmode_div_0_3	= { 0,  0,  1,  1,  0,  0,  0,  1,   0,   0, 0x00000000,  970 };		//   156, 156, 156, 156, 156, 156,  970                  312  0_3
	OPMODE_DIV_T	opmode_div_0 	= { 0,  0,  1,  1,  0,  0,  0,  1,   0,   0, 0x00000000,  900 };		//   156, 156, 156, 156, 156, 156,  900                  312  0


//	OPMODE_DIV_T	opmode_div_6 	= { 2,  0,  0,  1,  0,  3,  5,  0,   1, 184, 0x900D0764, 1150 };		//  1600, 533, 533, 320, 156, 312, 1150    5,   7,   1,  1612 6


#define REFCLK	26000000	// reference clock for PLL: Aspen

unsigned long CalculateOperatingModePLL(OPMODE_DIV_T *pOM)
{
	unsigned long	pll_freq;

	switch(pOM->pll_sel)
	{
	case	0:	pll_freq = 312000000; break;
	case	1:	pll_freq = 624000000; break;
	case	2:	pll_freq = REFCLK / (pOM->pll2_refdiv+2) * (pOM->pll2_fbdiv+2) ; break;
	default:	pll_freq = 312000000; break; // this is the power on default.
	}
	return pll_freq;
}

void GetCurrentOpMode(OPMODE_DIV_T *om)
{
    unsigned long a[11];
	om->pll_sel		= ( *(volatile unsigned long*)PMUM_FCCR >> 29 ) & 0x7;		// 0=pll1_312, 1=pll1_624, 2=pll2
	om->axipll_sel	= ( *(volatile unsigned long*)PMUM_FCCR >> 23 ) & 0x7;		// 0=pll1_312, 1=pll1_624, 2=pll2
	om->aclk2_div	= ( *(volatile unsigned long*)PMUA_CC_AP >> 18 ) & 0x7;		// axi2 clock
	om->aclk_div	= ( *(volatile unsigned long*)PMUA_CC_AP >> 15 ) & 0x7;		// is this hclk, also? axi clock
	om->dclk_div	= ( *(volatile unsigned long*)PMUA_CC_AP >> 12 ) & 0x7;		// ddr clock
	om->xpclk_div	= ( *(volatile unsigned long*)PMUA_CC_AP >>  9 ) & 0x7;		// aka L2clk - L2 clock
	om->baclk_div	= ( *(volatile unsigned long*)PMUA_CC_AP >>  6 ) & 0x7;		// aka busclk - bus interface clock
	om->pclk_div	= ( *(volatile unsigned long*)PMUA_CC_AP >>  0 ) & 0x7;		// processor clock
	om->pll2_refdiv	= ( *(volatile unsigned long*)PMUM_PLL2CR >> 19 ) & 0x1f;	// pll2 refdiv
	om->pll2_fbdiv	= ( *(volatile unsigned long*)PMUM_PLL2CR >> 10 ) & 0x1ff;	// pll2 fbdiv
	om->pll2_reg1	= *(volatile unsigned long*)PMUM_PLL2REG1;					// pll2 reg1 (has kvco, post divider & vco rng info)


    a[0] = om->pll_sel;      // PLL1 312
    a[1] = om->axipll_sel;   // PLL1 312
    a[2] = om->aclk2_div;    //
    a[3] = om->aclk_div;
    a[4] = om->dclk_div;
    a[5] = om->xpclk_div;
    a[6] = om->baclk_div;
    a[7] = om->pclk_div	;
    a[8] = om->pll2_refdiv;
    a[9] = om->pll2_fbdiv;
    a[10]= om->pll2_reg1;


}

unsigned long EnableSWDFC()
{
	volatile unsigned long	mpmu_acgr;
	volatile unsigned long	apmu_ccr;

	// some clocks are needed to allow DFC. Ensure they are enabled.
	mpmu_acgr = *(volatile unsigned long*)PMUM_ACGR;
	mpmu_acgr |= (0u<<20) |		// 52MHz for APB2 (1u would select APB2@26MHz)
				 (1u<<15) |		// 624MHz
				 (1u<<14) |		// PLL2
				 (1u<<13) |		// 312MHz
				 (1u<< 9) |		// GPC
				 (1u<< 4); 		// 26Mhz to APB
	*(volatile unsigned long*)PMUM_ACGR = mpmu_acgr;
	mpmu_acgr = *(volatile unsigned long*)PMUM_ACGR;	// read back to ensure write completes.



	// SWDFC Enable/Disabe: this is a new feature on A0.
	// Bit 21 is an nEnable to allow SW to initiate DFC.
	// Must make sure the bit is clear, so SW can initiate DFC.
	apmu_ccr  = *(volatile unsigned long*)PMUA_CC_AP;
	apmu_ccr &= ~(1u<<21);								// clear bit 21: enable SW DFC
	*(volatile unsigned long*)PMUA_CC_AP = apmu_ccr;

	// read-back, to ensure write completes.
	apmu_ccr = *(volatile unsigned long*)PMUA_CC_AP;
	return apmu_ccr;
}


SetOpModePLLandDividers(OPMODE_DIV_T *om)
{
	volatile unsigned long	apmu_ccr;
	volatile unsigned long	mpmu_fccr;
	volatile unsigned long	mpmu_pll2cr;
	volatile unsigned long	mpmu_pll2reg1;
	volatile unsigned long	mpmu_pll2reg2;

	// if this is a pll2 mode, set up the refdiv, fbdiv set, kvco, vrng and post dividers.
	if( om->pll_sel == 2 )
	{
		// before changing pll2 frequencies, disable pll2...
		mpmu_pll2cr = *(volatile unsigned long*)PMUM_PLL2CR;
		mpmu_pll2cr |=  (1u<<9);									// first must allow software to control pll2 activation
		*(volatile unsigned long*)PMUM_PLL2CR = mpmu_pll2cr;		// update that field
		mpmu_pll2cr = *(volatile unsigned long*)PMUM_PLL2CR;		// read back to ensure write completes.

		mpmu_pll2cr &= ~(1u<<8);									// now software can clear the pll2 enable bit
		*(volatile unsigned long*)PMUM_PLL2CR = mpmu_pll2cr;		// update that field
		mpmu_pll2cr = *(volatile unsigned long*)PMUM_PLL2CR;		// read back to ensure write completes.



		// set the new pll2 frequencies.
		mpmu_pll2cr = *(volatile unsigned long*)PMUM_PLL2CR;
		mpmu_pll2cr &= ~( ( 0x1f  << 19 ) | 						// masking out refdiv and
						  ( 0x1ff << 10 ) );						// masking out fbdiv.
		mpmu_pll2cr |=  ( ( om->pll2_refdiv << 19 ) |				// set new refdiv and
						  ( om->pll2_fbdiv  << 10 ) );				// set new fbdiv.
		*(volatile unsigned long*)PMUM_PLL2CR = mpmu_pll2cr;
		mpmu_pll2cr = *(volatile unsigned long*)PMUM_PLL2CR;		// read back to ensure write completes.



		// set up the kvco, vrng and post divider values.
		*(volatile unsigned long*)PMUM_PLL2REG1 = om->pll2_reg1;
		mpmu_pll2reg1 = *(volatile unsigned long*)PMUM_PLL2REG1;	// read back to ensure write completes.

		// ensure differential clock mode is selected. that matches the post divider configuration above.
		mpmu_pll2reg2 = *(volatile unsigned long*)PMUM_PLL2REG2;
		mpmu_pll2reg2 |= (1u<<6);									// enable differential output
		mpmu_pll2reg2 |= (1u<<5);									// sel_vco_clk_se
		mpmu_pll2reg2 |= (1u<<4);									// sel_vco_clk_diff
		*(volatile unsigned long*)PMUM_PLL2REG2 = mpmu_pll2reg2;
		mpmu_pll2reg2 = *(volatile unsigned long*)PMUM_PLL2REG2;	// read back to ensure write completes.



		// PLL2 is configured. Enable it.
		mpmu_pll2cr = *(volatile unsigned long*)PMUM_PLL2CR;
		mpmu_pll2cr |=  (1u<<8);									// pll2 enable bit
		*(volatile unsigned long*)PMUM_PLL2CR = mpmu_pll2cr;		// update that field
		mpmu_pll2cr = *(volatile unsigned long*)PMUM_PLL2CR;		// read back to ensure write completes.

	}

	// select the PLL sources, including for the axi fabrics.
	mpmu_fccr = *(volatile unsigned long*)PMUM_FCCR;
	mpmu_fccr &= ~( 7u << 29 );							// clear current pll select field
	mpmu_fccr |=  ( om->pll_sel << 29 );				// set new pll selection.
	mpmu_fccr &= ~( 7u << 23 );							// clear current pll select field
	mpmu_fccr |=  ( om->axipll_sel << 23 );				// set new pll selection.
														// note: PLL1 refdiv & fbdiv unchanged.
	*(volatile unsigned long*)PMUM_FCCR = mpmu_fccr;	// update those fields.
	mpmu_fccr = *(volatile unsigned long*)PMUM_FCCR;	// read-back to ensure completes.




	apmu_ccr = *(volatile unsigned long*)PMUA_CC_AP;
	apmu_ccr &= 0xFFF00000;		// clear out current divider settings.
	apmu_ccr |= ( om->aclk2_div << 18 ) |
		        ( om->aclk_div  << 15 ) |
				( om->dclk_div  << 12 ) |
				( om->xpclk_div <<  9 ) |
				( om->baclk_div <<  6 ) |
				( om->pclk_div  <<  0 );
	*(volatile unsigned long*)PMUA_CC_AP = apmu_ccr;
	apmu_ccr = *(volatile unsigned long*)PMUA_CC_AP;	// read back to ensure write completes.

}


InitiateDFC()
{
	volatile unsigned long	apmu_ccr;		// for enabling and initiating the DFC
	volatile unsigned long	apmu_icr;		// for monitoring the DFC progress
	volatile unsigned long	apmu_isr;		// for monitoring the DFC progress
	int						moreloops;		// limit the amount of waiting to do.

	// enable notification of dynamic frequency change events.
	apmu_icr = *(volatile unsigned long*)PMUA_AP_IMR;
	apmu_icr |= 0x3a;									// enabling DFC done notification for pclk, dclk and aclk
	*(volatile unsigned long*)PMUA_AP_IMR = apmu_icr;
	apmu_icr = *(volatile unsigned long*)PMUA_AP_IMR;	// read back to ensure write completes

	// clear out any status bits left over from previous events.
	apmu_isr =  *(volatile unsigned long*)PMUA_AP_ISR;
	*(volatile unsigned long*)PMUA_AP_ISR = apmu_isr;
	apmu_isr =  *(volatile unsigned long*)PMUA_AP_ISR;	// read back to ensure write completes.


	// To initiate a dynamic frequency change:
	// 1. The frequency change bits [31:28] must be written into the clock control register.
	// 2. Then the "allow" and "request" bits must [27:24] be written.
	// At this point, the DFC begins.

	apmu_ccr = *(volatile unsigned long*)PMUA_CC_AP;
	apmu_ccr |= (0xfu << 28);							// identify which buses are eligible for changing
	apmu_ccr &= ~(0xfu<< 24);							// clear the initiate bits during this stage.
	*(volatile unsigned long*)PMUA_CC_AP = apmu_ccr;
	apmu_ccr = *(volatile unsigned long*)PMUA_CC_AP;	// read back to ensure write completes.

	apmu_ccr |= (0xfu << 24);							// initiate the DFC
	*(volatile unsigned long*)PMUA_CC_AP = apmu_ccr;
	apmu_ccr = *(volatile unsigned long*)PMUA_CC_AP;	// read back to ensure write completes.

	// wait for the DFC to complete
	moreloops=100;
	while(moreloops--)
	{
		apmu_isr =  *(volatile unsigned long*)PMUA_AP_ISR;
		if( ( apmu_isr & 0x3a ) == 0x3a )	// all interesting bits have asserted.
			break;
	}

	// clear out the status bits
	apmu_isr =  *(volatile unsigned long*)PMUA_AP_ISR;
	*(volatile unsigned long*)PMUA_AP_ISR = apmu_isr;
	apmu_isr =  *(volatile unsigned long*)PMUA_AP_ISR;	// read back to ensure write completes.


}

ExecuteDMFC(OPMODE_DIV_T *pOM_prev,	OPMODE_DIV_T *pOM_next)
{
	unsigned long	pll_new;
	unsigned long	pclk_new;
	OPMODE_DIV_T	*pOM_temp;

	// This routine is called because PLL2 will be brought on-line,
	// or is being changed. While PLL2 is being changed, the processor
	// needs to switch over to PLL1.
	// The current and next operating modes are provided so that the
	// a PLL1-based operating mode that is closest to the final configuration
	// can be selected. This will prevent thrashing of the operating mode.
	// Caution: cannot select an operating point that will demand more
	// power than the next one - because the voltage may not have been
	// raised high enough to handle it.
	pll_new  = CalculateOperatingModePLL( pOM_next );
	pclk_new = pll_new / ( pOM_next->pclk_div + 1 );

	// Find a PLL1 operating mode that is closest to, but does
	// not exceed, the next operating mode.
	if( pclk_new <= 400000000 )
		pOM_temp = &opmode_div_0;			// mode 0 is the only PLL1 mode < 400
	else
		pOM_temp = &opmode_div_2;			// mode 2 is the closest to all the other PLL2 modes.

	SetOpModePLLandDividers( pOM_temp );
	InitiateDFC();

	// now the processor is safely running using PLL1.
	// PLL2 can be reconfigured.
}

ShutdownPLL2()
{
	unsigned long	mpmu_pll2cr;

	// before changing pll2 frequencies, disable pll2...
	mpmu_pll2cr  = *(volatile unsigned long*)PMUM_PLL2CR;
	mpmu_pll2cr &= ~(1u<<8);									// now software can clear the pll2 enable bit
	*(volatile unsigned long*)PMUM_PLL2CR = mpmu_pll2cr;		// update that field
	mpmu_pll2cr = *(volatile unsigned long*)PMUM_PLL2CR;		// read back to ensure write completes.
}

unsigned long SetOpModeByDividers(OPMODE_DIV_T	*pOM_new, unsigned long	mV)
{
	OPMODE_DIV_T	OM_orig;

	unsigned long	pll_orig;
	unsigned long	pclk_orig;
	unsigned long	dclk_orig;
	unsigned long	power_orig;

	unsigned long	pll_new;
	unsigned long	pclk_new;
	unsigned long	dclk_new;
	unsigned long	power_new;


	EnableSWDFC();	// make sure the processor is enabled for SW initiated DFC.

	GetCurrentOpMode(&OM_orig);

	// need to determine if voltage needs to be set before or after the DFC (Dynamic Frequency Change).
	// if the new operating mode demands more power than the current one, need to raise the voltage before changing the frequency.
	// if the new operating mode demands less power than the current one, need to lower the frequency before changing the voltage.
	// use the pclk and dclk to compare operating mode power demands.
	pll_orig   = CalculateOperatingModePLL( &OM_orig );
	pclk_orig  = pll_orig / ( OM_orig.pclk_div + 1 );
	dclk_orig  = pll_orig / ( OM_orig.dclk_div + 1 );
	power_orig = pclk_orig + dclk_orig;

	pll_new    = CalculateOperatingModePLL( pOM_new );
	pclk_new   = pll_new / ( pOM_new->pclk_div + 1 );
	dclk_new   = pll_new / ( pOM_new->dclk_div + 1 );
	power_new  = pclk_new + dclk_new;

#if 0
	// going to a higher power demand operating mode? if so, get the voltage up first...
	if( power_new > power_orig )
	{
		SetVoltage( mV? mV : pOM_new->mV );		// if a mV was passed in, use it. if not, get the default from the OM structure.
	}
#endif

	// now set the frequency...

	// Check if a dynamic master frequency change operation needs to take place first.
	// That would be the case if the operating mode is already using PLL2 and the new
	// operating mode will be in PLL2. Presumbably either fbdiv or refdiv is changing.
	if( ( pOM_new->pll_sel == 2 ) && ( OM_orig.pll_sel == 2 ) )
	{
		ExecuteDMFC( &OM_orig, pOM_new );
	}

	// now just set the PLL, set the dividers and the initiate the DFC.
	SetOpModePLLandDividers( pOM_new );
	InitiateDFC();

	// Does PLL2 need to be shut down?
	// That would be the case if the original operating mode
	// was using PLL2 and the new operating mode is not PLL2.
	if( ( pOM_new->pll_sel != 2 ) && ( OM_orig.pll_sel == 2 ) )
	{
		ShutdownPLL2();
	}


	// if went from a higher power demand operating mode to a lower power demand operating mode,
	// it is now OK to reduce the voltage.
	if( power_new < power_orig )
	{
		SetVoltage( mV? mV : pOM_new->mV );		// if a mV was passed in, use it. if not, get the default from the OM structure.
	}

    GetCurrentOpMode(&OM_orig);

	return TRUE;	// TRUE means operating mode set.
}


// This routine will set different operating modes.
// It has the ability to select different operating modes dependent on TIM records.
// Note some operating modes require specific voltage.

unsigned long ConfigureOperatingModeByMode( void *pTIM, unsigned long *pulModeSet )
{

	// variables for walking through the TIM records
	OPMDSpecList_T	*pOPMDSpecs;		// operating mode specs
	OPMDSpec_T		*pOPMDSpec;

	int				nRecs;
	int				r;
	UINT_T			status;

	OPMDSpec_T		*pConsumerId;
	OPMODE_DIV_T	*pOpModeDiv = NULL;
	UINT_T			mode_set = 0;

	*pulModeSet = 0;	// default is to indicate that no mode set took place.

	// does an operating mode package exist in the TIM?
	//pOPMDSpecs = (OPMDSpecList_T*)FindPackageInReserved(&status, pTIM, OPMODEID);		// 'MODE'
	if( status != NoError )
	{									// no operating mode package
		return NoError;					// that's OK. the operating mode package is optional.
	}									// return No Error so booting / boot-loading continues.

	// an operating mode package exists.

	// the package id will be the desired operating mode.
	// the package value will be the desired voltage.
	// voltage == 0 means use the POR default voltage

	//nRecs = ConfigRecordCount(pOPMDSpecs);
	//pOPMDSpec=pOPMDSpecs->OPMDSpecs;
	//for(r=0; !mode_set && r<nRecs; r++, pOPMDSpec++)
	//{
		//switch(pOPMDSpec->KeyId)
		//{
		//case	ASPEN_OPMODE0_ID:	pOpModeDiv = &opmode_div_0;   break;
		//case	ASPEN_OPMODE0_3_ID:	pOpModeDiv = &opmode_div_0_3; break;
		//case	ASPEN_OPMODE1_ID:	pOpModeDiv = &opmode_div_1;   break;
		//case	ASPEN_OPMODE2_ID:	pOpModeDiv = &opmode_div_2;   break;
		//case	ASPEN_OPMODE2_1_ID:	pOpModeDiv = &opmode_div_2_1; break;
		//case	ASPEN_OPMODE2_2_ID:	pOpModeDiv = &opmode_div_2_2; break;
		//case	ASPEN_OPMODE2_3_ID:	pOpModeDiv = &opmode_div_2_3; break;
		//case	ASPEN_OPMODE2_4_ID:	pOpModeDiv = &opmode_div_2_4; break;
		//case	ASPEN_OPMODE3_ID:	pOpModeDiv = &opmode_div_3;   break;
		//case	ASPEN_OPMODE3_1_ID:	pOpModeDiv = &opmode_div_3_1; break;
		//case	ASPEN_OPMODE3_2_ID:	pOpModeDiv = &opmode_div_3_2; break;
		//case	ASPEN_OPMODE3_3_ID:	pOpModeDiv = &opmode_div_3_3; break;
		//case	ASPEN_OPMODE4_ID:	pOpModeDiv = &opmode_div_4;   break;
		//case	ASPEN_OPMODE4_1_ID:	pOpModeDiv = &opmode_div_4_1; break;
		//case	ASPEN_OPMODE5_ID:	pOpModeDiv = &opmode_div_5;   break;
		//case	ASPEN_OPMODE6_ID:	pOpModeDiv = &opmode_div_6;   break;
		//}


		pOpModeDiv = &opmode_div_0; /* Using ASPEN_OPMODE0_ID */

		if( pOpModeDiv )
		{
			mode_set = SetOpModeByDividers( pOpModeDiv, 0);	// a voltage override can be included in the TIM record.
		}
	//}

	*pulModeSet = mode_set;		// return an indicator as to whether or not the mode was set.
	return NoError;
}

unsigned long ConfigureOperatingModeByDividers( void *pTIM, unsigned long *pulModeSet )
{

	// variables for walking through the TIM records
	OPDVSpecList_T	*pOPDVSpecs;		// operating configuration divider specs
	OPDVSpec_T		*pOPDVSpec;

	int				nRecs;
	int				r;
	UINT_T			status;

	UINT_T			*pConsumerId;
	UINT_T			mode_set = 0;
	OPMODE_DIV_T	OpModeDiv;

	*pulModeSet = 0;	// default is to indicate that no mode set took place.

	// does an operating mode package exist in the TIM?
	// pOPDVSpecs = (OPDVSpecList_T*)FindPackageInReserved(&status, pTIM, OPDIVID);		// 'OPDV'
	if( status != NoError )
	{									// no operating mode package
		return NoError;					// that's OK. the operating mode package is optional.
	}									// return No Error so booting / boot-loading continues.

	// the package ids will contain the specific configuration values.

	nRecs = ConfigRecordCount(pOPDVSpecs);
	pOPDVSpec=pOPDVSpecs->OPDVSpecs;
	for(r=0; !mode_set && r<nRecs; r++, pOPDVSpec++)
	{
		switch(pOPDVSpec->KeyId)
		{
		case	ASPEN_OPDIV_PLLSEL:			OpModeDiv.pll_sel		= pOPDVSpec->KeyValue; break;
		case	ASPEN_OPDIV_AXI_PLLSEL:		OpModeDiv.axipll_sel	= pOPDVSpec->KeyValue; break;
		case	ASPEN_OPDIV_PLL2_REFDIV:	OpModeDiv.pll2_refdiv	= pOPDVSpec->KeyValue; break;
		case	ASPEN_OPDIV_PLL2_FBDIV:		OpModeDiv.pll2_fbdiv	= pOPDVSpec->KeyValue; break;
		case	ASPEN_OPDIV_PLL2_REG1:		OpModeDiv.pll2_reg1		= pOPDVSpec->KeyValue; break;
		case	ASPEN_OPDIV_PCLK_DIV:		OpModeDiv.pclk_div		= pOPDVSpec->KeyValue; break;
		case	ASPEN_OPDIV_DCLK_DIV:		OpModeDiv.dclk_div		= pOPDVSpec->KeyValue; break;
		case	ASPEN_OPDIV_XPCLK_DIV:		OpModeDiv.xpclk_div		= pOPDVSpec->KeyValue; break;
		case	ASPEN_OPDIV_BACLK_DIV:		OpModeDiv.baclk_div		= pOPDVSpec->KeyValue; break;
		case	ASPEN_OPDIV_ACLK_DIV:		OpModeDiv.aclk_div		= pOPDVSpec->KeyValue; break;
		case	ASPEN_OPDIV_ACLK2_DIV:		OpModeDiv.aclk2_div		= pOPDVSpec->KeyValue; break;
		case	ASPEN_OPDIV_MV:				OpModeDiv.mV			= pOPDVSpec->KeyValue; break;
		case	ASPEN_OPDIV_GO:				mode_set = SetOpModeByDividers( &OpModeDiv, pOPDVSpec->KeyValue );   break;
		}
	}

	*pulModeSet = mode_set;		// return an indicator as to whether or not the mode was set.
	return NoError;
}

unsigned long ConfigureOperatingMode( void *pTIM, unsigned long *pulModeSet )
{
	unsigned long	retval;
	unsigned long	mode_set;

	// try configuring by "mode" first. if that doesn't work, try looking for divider values.
	retval = ConfigureOperatingModeByMode( pTIM, &mode_set );			// configure by mode
	if( !mode_set )
	{
		retval = ConfigureOperatingModeByDividers( pTIM, &mode_set );	// configure by divider values
	}

	return retval;
}

#if I2C

#include "I2C.h"
#include "platform_i2c.h"
I2C_CONTEXT_T	I2CContext;
int				I2CContextInitialized = 0;

void SetVoltage(unsigned long mV)
{
	unsigned long	databyte;
	unsigned char	txdata[8];
	unsigned int	txlen;
	unsigned int	PmicI2cAddress;

	unsigned long	*pMfprSCLaddr;
	unsigned long	*pMfprSDAaddr;
	unsigned long	MfprSCLval;
	unsigned long	MfprSDAval;
	unsigned long	I2CBaseRegs;

	//
	// convert the mV to a databyte value acceptable for the Aspenite PMIC
	// the formula is: databyte = (x_mV - 390mV)/0.028.
	// !!! only applies to Aspenite with latest ECOs!
	// use appropriate scaling to avoid floating point

#if WAYLAND
	databyte = ( mV - 725 ) / 25;
#else
	databyte = ( mV - 390 ) / 28;
#endif
	// sample values:
	//  1230 mV: databyte = 30
	//  1258 mV: databyte = 31


	// For correct configuration and programming,
	// the following processor and board specific information is required:
	//	PMIC used for voltage control: determines "programming" sequence
	//	pads used for SCL and SDA: determines twsi base reg addr as well as mfpr addr & vals

	// This is targetted for an Aspenite Rev 2 platform.

	// I2C voltage control on the Aspenite uses the following resources:
	//	PMIC:		Max8660, slave address 0x68
	//	PI2C_SCL:	connected to pwr_scl on the processor. (aka gpio88 when af=6)
	//	PI2C_SDA:	connected to pwr_sda on the processor. (aka gpio87 when af=6)

	// that information results in the following configuration values:
	//	scl pad addr:	d401e1ec
	//	sda pad addr:	d401e1f0
	//	scl pad val:	8c0 (alternate function 0)
	//	sda pad val:	8c0 (alternate function 0)
	//	i2c base regs:	TWSIPWR_BASE_REGS (d4025000)

	// set up the variables based on the processor and board specific configuration noted above:
	pMfprSCLaddr	= (unsigned long*)0xd401e1ec;
	pMfprSDAaddr	= (unsigned long*)0xd401e1f0;
	MfprSCLval		= 0x08c0;
	MfprSDAval		= 0x08c0;
	I2CBaseRegs		= TWSIPWR_BASE_REGS;
	PmicI2cAddress	= 0x68;

	// the Max8660 requires the following "programming" sequence:
	//	1. program the voltage value register: slave addr, reg id (23), value (computed)
	//  2. program the change-voltage-now reg: slave addr, reg id (20), value (1)

	// create the command sequence based on the pmic specific info noted above:
	// note that we're combining the two programming sequences into one transaction.
	txdata[0] = 0x23;
	txdata[1] = databyte;
	txdata[2] = 0x20;
	txdata[3] = 1;
	txlen     = 4;

	if(!I2CContextInitialized)
	{
		I2CInit(
			&I2CContext,
			TWSIPWR_BASE_REGS,
			pMfprSCLaddr,
			pMfprSDAaddr,
			MfprSCLval,
			MfprSDAval,
			0x45
			);
		I2CContextInitialized = 1;
	}

	I2CMasterSendDataDirect( &I2CContext, txdata, txlen, PmicI2cAddress );

}

#else

// This routine is based on an XDB script used to set the voltage.
// FIXME: add the I2C driver back in and make use of it.
void SetVoltage(unsigned long mV)
{
	unsigned long databyte;

	// convert the mV to a databyte value acceptable for the Aspenite PMIC
	// the formula is: databyte = (x_mV - 390mV)/0.028.
	// !!! only applies to Aspenite with latest ECOs!
	// use appropriate scaling to avoid floating point
#if WAYLAND
	databyte = ( mV - 725 ) / 25;
#else
	databyte = ( mV - 390 ) / 28;
#endif
	// sample values:
	//  1230 mV: databyte = 30
	//  1258 mV: databyte = 31



	/*
	!
	!       Init the Power I2C interface - set up clocks
	!
	set value /size=long *(unsigned long *)0xD4051024=0xDFFFFFFE
	set value /size=long *(unsigned long *)0xD401506C=0x4
	set value /size=long *(unsigned long *)0xD401506C=0x7
	set value /size=long *(unsigned long *)0xD401506C=0x3
	*/
	*(volatile unsigned long*)0xD4051024=0xDFFFFFFE; DelayEx(10000);
	*(volatile unsigned long*)0xD401506C=0x4; DelayEx(10000);
	*(volatile unsigned long*)0xD401506C=0x7; DelayEx(10000);
	*(volatile unsigned long*)0xD401506C=0x3; DelayEx(10000);

	/*
	!
	!       Init the Power I2C interface - reset the controller
	!
	set value /size=long *(volatile unsigned long*)0xD4025010=0x4060
	set value /size=long *(volatile unsigned long*)0xD4025010=0x0060
	set value /size=long *(volatile unsigned long*)0xD4025020=0x0
	*/
	*(volatile unsigned long*)0xD4025010=0x4060; DelayEx(10000);
	*(volatile unsigned long*)0xD4025010=0x0060; DelayEx(10000);
	*(volatile unsigned long*)0xD4025020=0x0; DelayEx(10000);

	/*
	!
	!       CODE SEQUNECE TO WRITE REGISTER 0X23 WITH THE VALUE 0X1F
	!
	! 1) Program Slave Address (0x68) into IDBR with start bit
	!
	!
	set value /size=long *(volatile unsigned long*)0xD4025008=0x68
	set value /size=long *(volatile unsigned long*)0xD4025010=0x00000069
	*/
	*(volatile unsigned long*)0xD4025008=0x68; DelayEx(10000);
	*(volatile unsigned long*)0xD4025010=0x00000069; DelayEx(10000);

	/*
	! 2) Send PMIC Address Tx data=0x23
	!
	!
	set value /size=long *(volatile unsigned long*)0xD4025008=0x23
	set value /size=long *(volatile unsigned long*)0xD4025010=0x00000068
	*/
	*(volatile unsigned long*)0xD4025008=0x23; DelayEx(10000);
	*(volatile unsigned long*)0xD4025010=0x00000068; DelayEx(10000);

	/*
	! 3) Data = computed from input parameter, with a stop
	!
	set value /size=long *(volatile unsigned long*)0xD4025008=0x1F
	set value /size=long *(volatile unsigned long*)0xD4025010=0x0000006A
	*/
	*(volatile unsigned long*)0xD4025008=databyte; DelayEx(10000);
	*(volatile unsigned long*)0xD4025010=0x0000006A; DelayEx(10000);

	/*
	!
	!       CODE SEQUNECE TO WRITE REGISTER 0X20 WITH THE VALUE 0X01
	!
	! 1) Program Slave Address (0x68) into IDBR with start bit
	!
	!
	set value /size=long *(volatile unsigned long*)0xD4025008=0x68
	set value /size=long *(volatile unsigned long*)0xD4025010=0x00000069
	*/
	*(volatile unsigned long*)0xD4025008=0x68; DelayEx(10000);
	*(volatile unsigned long*)0xD4025010=0x00000069; DelayEx(10000);

	/*
	! 2) Send PMIC Address Tx data=0x20
	!
	!
	set value /size=long *(volatile unsigned long*)0xD4025008=0x20
	set value /size=long *(volatile unsigned long*)0xD4025010=0x00000068
	*/
	*(volatile unsigned long*)0xD4025008=0x20; DelayEx(10000);
	*(volatile unsigned long*)0xD4025010=0x00000068; DelayEx(10000);

	/*
	! 3) Data 0=1 (sets the GO bit), with a stop
	!
	set value /size=long *(volatile unsigned long*)0xD4025008=0x01
	set value /size=long *(volatile unsigned long*)0xD4025010=0x0000006A
	*/
	*(volatile unsigned long*)0xD4025008=0x01; DelayEx(10000);
	*(volatile unsigned long*)0xD4025010=0x0000006A; DelayEx(10000);
}

#endif


void DelayEx( unsigned long d )
{
	volatile unsigned long l = d;
	while(l) --l;
}

// Phuong added
unsigned long SetOperationMode(void)
{
    //OPMODE_DIV_T *pOM_new = &opmode_div_0;   // CPU 156 MHz, DDR2 156 MHz
    OPMODE_DIV_T *pOM_new = &opmode_div_3_1; // CPU 800 MHz, DDR2 200 MHz
    //OPMODE_DIV_T *pOM_new = &opmode_div_3_2;   // CPU 800 MHz, DDR2 400 MHz
    //OPMODE_DIV_T *pOM_new = &opmode_div_5;   // CPU 1200 MHZ, DDR2 400 MHz

    return SetOpModeByDividers(pOM_new, 0);
}
