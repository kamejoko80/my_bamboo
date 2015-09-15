/******************************************************************************
**  COPYRIGHT  2007 Marvell Inernational Ltd.
**  All Rights Reserved
******************************************************************************/

#ifndef __PROCESSOR_ENUMS_H__
#define __PROCESSOR_ENUMS_H__

typedef enum ASPEN_CLOCK_ID_E
{
	ASPEN_CLOCK_ID_MCU,
	ASPEN_CLOCK_ID_I2C
} ASPEN_CLOCK_ID_T;

typedef enum ASPEN_FREQ_ID_E
{
	ASPEN_FREQ_ID_MCU
} ASPEN_FREQ_ID_T;

typedef enum ASPEN_OPMODE_E
{
	ASPEN_OPMODE_CONSUMER_ID,	// will be BootROM or BootLoader. If omitted, processed by both.
	ASPEN_OPMODE0_ID,			// 156,  PLL1 based
	ASPEN_OPMODE1_ID,			// 400,  PLL2 based
	ASPEN_OPMODE2_ID,			// 624,  PLL1 based - requires higher voltage
	ASPEN_OPMODE3_ID,			// 800,  PLL2 based - requires higher voltage
	ASPEN_OPMODE4_ID,			// 1066, PLL2 based - requires higher voltage
	ASPEN_OPMODE5_ID,			// 1200, PLL2 based - requires higher voltage
	ASPEN_OPMODE6_ID,			// 1600, PLL2 based - requires higher voltage <= not officially supported.


	ASPEN_OPMODE2_1_ID,			// 624,  PLL2 based
	ASPEN_OPMODE2_2_ID,			// 624,  PLL1 based
	ASPEN_OPMODE2_3_ID,			// 624,  PLL1 based

	ASPEN_OPMODE3_1_ID,			// 800,  PLL2 based
	ASPEN_OPMODE3_2_ID,			// 800,  PLL2 based
	ASPEN_OPMODE3_3_ID,			// 800,  PLL2 based

	// new modes
	ASPEN_OPMODE0_3_ID,			// 156,  PLL2 based, tight voltage tolerance
	ASPEN_OPMODE2_4_ID,			// 624,  PLL2 based
	ASPEN_OPMODE4_1_ID,			// 1066, PLL2 based
} ASPEN_OPMODE_T;

typedef enum ASPEN_OPDIV_E
{
	ASPEN_OPDIV_CONSUMER_ID,
	ASPEN_OPDIV_PLLSEL,
	ASPEN_OPDIV_AXI_PLLSEL,
	ASPEN_OPDIV_PLL2_REFDIV,
	ASPEN_OPDIV_PLL2_FBDIV,
	ASPEN_OPDIV_PCLK_DIV,
	ASPEN_OPDIV_DCLK_DIV,
	ASPEN_OPDIV_XPCLK_DIV,
	ASPEN_OPDIV_BACLK_DIV,
	ASPEN_OPDIV_ACLK_DIV,
	ASPEN_OPDIV_ACLK2_DIV,
	ASPEN_OPDIV_PLL2_REG1,
	ASPEN_OPDIV_MV,
	ASPEN_OPDIV_GO
} ASPEN_OPDIV_T;

typedef enum ASPEN_MCU_REGID_E
{
	ASPEN_SDRREVREG_ID,			// revision
	ASPEN_SDRADCREG_ID,			// address decode
	ASPEN_SDRCFGREG0_ID,		// sdram config reg 0
	ASPEN_SDRCFGREG1_ID,		// sdram config reg 1
	ASPEN_SDRTMGREG1_ID,		// sdram timing reg 1
	ASPEN_SDRTMGREG2_ID,		// sdram timing reg 2
	ASPEN_SDRTMGREG3_ID,		// sdram timing reg 3
	ASPEN_SDRTMGREG4_ID,		// sdram timing reg 4
	ASPEN_SDRTMGREG5_ID,		// sdram timing reg 5
	ASPEN_SDRCTLREG1_ID,		// sdram control reg 1
	ASPEN_SDRCTLREG2_ID,		// sdram control reg 2
	ASPEN_SDRCTLREG3_ID,		// sdram control reg 3
	ASPEN_SDRCTLREG4_ID,		// sdram control reg 4
	ASPEN_SDRCTLREG5_ID,		// sdram control reg 5
	ASPEN_SDRCTLREG6_ID,		// sdram control reg 6
	ASPEN_SDRCTLREG7_ID,		// sdram control reg 7
	ASPEN_SDRCTLREG13_ID,		// sdram control reg 13
	ASPEN_SDRCTLREG14_ID,		// sdram control reg 14
	ASPEN_SDRERRREG_ID,			// sdram error status
	ASPEN_ADRMAPREG0_ID,		// address map cs0
	ASPEN_ADRMAPREG1_ID,		// address map cs1
	ASPEN_USRCMDREG0_ID,		// user initiated command registers
	ASPEN_SDRSTAREG_ID,			// sdram status register
	ASPEN_PHYCTLREG3_ID,		// phy control reg 3
	ASPEN_PHYCTLREG7_ID,		// phy control reg 7
	ASPEN_PHYCTLREG8_ID,		// phy control reg 8
	ASPEN_PHYCTLREG9_ID,		// phy control reg 9
	ASPEN_PHYCTLREG10_ID,		// phy control reg 10
	ASPEN_PHYCTLREG11_ID,		// phy control reg 11
	ASPEN_PHYCTLREG12_ID,		// phy control reg 12
	ASPEN_PHYCTLREG13_ID,		// phy control reg 13
	ASPEN_PHYCTLREG14_ID,		// phy control reg 14
	ASPEN_DLLCTLREG1_ID,		// dll control reg 1
	ASPEN_TSTMODREG0_ID,		// test mode reg 0
	ASPEN_TSTMODREG1_ID,		// test mode reg 1
	ASPEN_MCBCTLREG1_ID,		// mcb control reg 1
	ASPEN_MCBCTLREG2_ID,		// mcb control reg 2
	ASPEN_MCBCTLREG3_ID,		// mcb control reg 3
	ASPEN_MCBCTLREG4_ID,		// mcb control reg 4
	ASPEN_PRFCTLREG0_ID,		// perf control reg 0
	ASPEN_PRFCTLREG1_ID,		// perf control reg 1
	ASPEN_PRFSTAREG_ID,			// perf status reg
	ASPEN_PRFSELREG_ID,			// perf select reg
	ASPEN_PRFCNTREG_ID,			// perf count reg
	ASPEN_SDRTMGREG6_ID,		// sdram timing reg 6
	ASPEN_PHYCTLREGTST_ID,		// phy control reg test mode
	ASPEN_PRF_COUNTER_ID,		// performance counter reg
	ASPEN_OPDELAY_ID,			// operation delay reg
	ASPEN_OPREAD_ID				// operation read reg

} ASPEN_MCU_REGID_T;

#endif

