/******************************************************************************
**  COPYRIGHT  2007 Marvell Inernational Ltd.
**  All Rights Reserved
******************************************************************************/



#ifndef __ASPN_SDRAM_FIELDS__
#define __ASPN_SDRAM_FIELDS__

// default mcu field values for ASPEN

	
	// SDRAM Configuration Register 0 - CS0, Type 1
	// offset 0x00000020
	// Morona value:	0x00002330
	//					0 000 000000 00 000 000 10 0011 0011 0000
	// Aspen value:		0x00002330 (156mhz ddr)
	//					0 000 000000000 00 0 00 10 0011 0011 0000
	// Aspen value:		0x00046330 (312mhz ddr)
	//					0 000 000000000 10 0 01 10 0011 0011 0000
#define ASPEN_CFGR0_RSVD1						0
#define	ASPEN_CFGR0_PASRn						0
#define ASPEN_CFGR0_RSVD2						0
#define	ASPEN_CFGR0_RTTn						0
#define	ASPEN_CFGR0_RSVD3						0
#define	ASPEN_CFGR0_ODSn						0
#define ASPEN_NumBanks							2	// 8 banks
#define ASPEN_NumRows							3	// 13 row address bits	
#define ASPEN_NumColumns						3	// 10 column address bits
#define	ASPEN_CFGR0_RSVD4						0

	// SDRAM Timing Register 1
	// offset 0x00000050
	// Morona value:	0x49150070
	//					010 010 0100 010101 0000000001110000
	// Aspen value:		0x488905a0 (156mhz ddr)
	//					010 010 0010 001001 0000010110100000
	// Aspen value:		0x4cd20920 (312mhz ddr)
	//					010 011 0011 010010 0000100100100000
#define ASPEN_tCCD								2
#define ASPEN_tRTP								2
#define ASPEN_tWTR								2
#define ASPEN_tRC								0x9
#define ASPEN_tREFI							    0x5a0		// FIXME: different from default. Why? 
															// Answer: refresh clock is not = dclk. it is flck, set to 26MHz.

	// SDRAM Timing Register 2
	// offset 0x00000060
	// Morona value:	0x63330332
	//					0110 0011 0011 0011 000 000110011 0 010
	// Aspen value:		0x32330332 (156mhz ddr)
	//					0011 0010 0011 0011 000 000110011 0 010
	// Aspen value:		0x54550292 (312mhz ddr)
	//					0101 0100 0101 0101 000 000101001 0 010
#define ASPEN_tRP								3
#define ASPEN_tRRD								2
#define ASPEN_tRCD								3
#define ASPEN_tWR								3
#define ASPEN_TMGREG2_RSVD1						0
#define ASPEN_tRFC								0x33
#define ASPEN_TMGREG2_RSVD2						0
#define ASPEN_tMRD								2

	// SDRAM Timing Register 3
	// offset 0x00000190
	// ASPEN value:	0x60C81CFD
	//					0110 0 0 0011001000 00011100 111 11 101
#define ASPEN_tMOD								6
#define ASPEN_tXSRD							0xc8
#define ASPEN_tXSNR							0x1c
#define ASPEN_tXARDS							0x7
#define ASPEN_SDRTR3_RSVD2						3			// FIXME: reserved is non-zero. Why? 
#define ASPEN_tXP								5			// FIXME: different from default. Why?

	// SDRAM Timing Register 4
	// offset 0x000001c0
	// ASPEN value:	0x3282A050
	//					0 011 00101000 001 0101000 0001010000
#define ASPEN_tCKE								3
#define ASPEN_INIT_COUNT						0x28
#define ASPEN_tRWD_EXT_DLY						1
#define ASPEN_RESET_COUNT						0x28
#define ASPEN_INIT_COUNT_NOP					0x50

	// SDRAM Timing Register 5
	// offset 0x00000650
	// ASPEN value:	0x001000A1
	//					0000000000 010000 0 000 00 001010 0 001
#define ASPEN_tRAS								0x10
#define ASPEN_tCCD_CCS_WR_EXT_DLY				0
#define ASPEN_tFAW								0xa
#define ASPEN_tCCD_CCS_EXT_DLY					1

	// SDRAM Control Register 1
	// offset 0x00000080
	// ASPEN value:	0x00000000
#define ASPEN_APS_EN							0
#define ASPEN_APS_TYPE							0
#define ASPEN_APS_VALUE							0
#define ASPEN_ACS_EXIT_DLY						0
#define ASPEN_ACS_EN							0
#define ASPEN_DLL_RESET							0
#define ASPEN_CAS_BT							0
#define ASPEN_OUT_EN							0
#define ASPEN_WRITE_LEVEL_EN					0
#define ASPEN_tW2R_DIS							0

	// SDRAM Control Register 2
	// offset 0x00000090
	// Morona value:	0x00080000
	//					0000 0 000 00 001000 000000 0 0 0 0 0 0 0 0 0 0 
	// Aspen value:		0x0008000
	//					0000 0 000 00 001000 000000 0 0 0 0 0 0 0 0 0 0 
#define ASPEN_REF_POSTED_EN						0
#define ASPEN_REF_POSTED_MAX					0
#define ASPEN_SDRAM_LINE_BOUNDARY				8
#define ASPEN_REFPB_MODE						0
#define ASPEN_PD_MODE							0
#define ASPEN_F2T_MODE							0			// FIXME note: in documentation, called only "2T_MODE". name should start with a letter.
#define ASPEN_RDIMM_MODE						0
#define ASPEN_APRECHARGE						0
#define ASPEN_WRITE_LEVEL_MODE					0
#define ASPEN_INT_SHADOW_MODE					0
#define ASPEN_TEST_MODE							0

	// SDRAM Control Register 3
	// offset 0x000000f0
	// Morona value:	0x80000000
	//					1 0 0 000000000 0 0 0 0 0000 000 000 0 0 0 0 0 0
	// Aspen value:		0xc0000000
	//					1 1 0 000000000 0 0 0 0 0000 000 000 0 0 0 0 0 0
#define ASPEN_EARLY_CMD_EN						1
#define ASPEN_MC_FLOP_REQ_EN					1
#define ASPEN_MC_FLOP_GRANT_EN					0
#define ASPEN_CTLREG3_RSVD1						0
#define ASPEN_WSTR3_EARLY_WRITE_EN				0
#define ASPEN_MSTR2_EARLY_WRITE_EN				0
#define ASPEN_MSTR3_FAST_WRITE_EN				0
#define ASPEN_MSTR2_FAST_WRITE_EN				0
#define ASPEN_CTLREG3_RSVD2						0
#define ASPEN_SB_EARLY_WRITE_USERVALUE			0
#define ASPEN_CPU_EARLY_WRITE_USERVALUE			0
#define ASPEN_SB_EARLY_WRITE_USER				0
#define ASPEN_CPU_EARLY_WRITE_USER				0
#define ASPEN_SB_EARLY_WRITE_EN					0
#define ASPEN_CPU_EARLY_WRITE_EN				0
#define ASPEN_SB_FAST_WRITE_EN					0
#define ASPEN_CPU_FAST_WRITE_EN					0

	// SDRAM Control Register 4
	// offset 0x000001a0
	// Morona value:	0x40810005
	//                  0 1 0 0000 010 000 0 0 100 0 000 0 0 0 0 0 001 01
	// Aspen value:		0x00810004 (156mhz ddr)							// stage 1
	//					0 0 0 0000 010 000 0 0 100 000000000 001 00		// stage 1
	// Aspen value:		0x40810004 (156mhz ddr)							// stage 2		// try using the stage 2 value.
	//					0 1 0 0000 010 000 0 0 100 000000000 001 00		// stage 2
	// Aspen value:		0x00814004 (312mhz ddr)
	//					0 0 0 0000 010 000 0 0 100 000000000 001 00
#define ASPEN_SDRAM_DLL_EN						0
#define ASPEN_DQSB_EN							1
#define ASPEN_FAST_BANK							0
#define ASPEN_CTLRG4_RSVD1						0
#define ASPEN_BURST_LENGTH						2
#define ASPEN_AL_NUMBER							0
#define ASPEN_AL_EN								0
#define ASPEN_RQDS_EN							0
#define ASPEN_CAS_LATENCY						4
#define ASPEN_CTLRG4_RSVD2						0
#define ASPEN_SDRAM_TYPE						1
#define ASPEN_DATA_WIDTH						0	// FIXME: why isn't 1 used for 32 bit internal bus?

	// SDRAM Control Register 5
	// offset 0x00000280
	// ASPEN value:	0x01010101
	//					0000 0001 0000 0001 0000 0001 0000 0001
#define ASPEN_MASTER_3_WEIGHT					1
#define ASPEN_MASTER_2_WEIGHT					1
#define ASPEN_MASTER_1_WEIGHT					1
#define ASPEN_MASTER_0_WEIGHT					1

	// SDRAM Control Register 6
	// offset 0x00000760
	// Morona value:	0x00000000
	// Aspen value:		0x00000000 (156mhz ddr)
	// Aspen value:		0x00000001 (312mhz ddr)
#define ASPEN_ODT3_READ_EN						0
#define ASPEN_ODT3_WRITE_EN						0
#define ASPEN_ODT2_READ_EN						0
#define ASPEN_ODT2_WRITE_EN						0
#define ASPEN_ODT1_READ_EN						0
#define ASPEN_ODT1_WRITE_EN						0
#define ASPEN_ODT0_READ_EN						0
#define ASPEN_ODT0_WRITE_EN						0

	// SDRAM Control Register 7
	// offset 0x00000770
	// Morona value:	0x00000000
	// Aspen value:		0x00000000 (156mhz ddr)
	// Aspen value:		0x02000002 (312mhz ddr)
#define ASPEN_FORCE_ODT							0
#define ASPEN_PAD_TERM_SWITCH_MODE				0
#define ASPEN_ODT3_SWITCH_MODE					0
#define ASPEN_ODT2_SWITCH_MODE					0
#define ASPEN_ODT1_SWITCH_MODE					0
#define ASPEN_ODT0_SWITCH_MODE					0

	// MCB Control Register 1
	// offset 0x00000510
	// ASPEN value:	0x01010101
	//					0000 0001 0000 0001 0000 0001 0000 0001
#define ASPEN_MCB_MASTER3_WEIGHT				1
#define ASPEN_MCB_MASTER2_WEIGHT				1
#define ASPEN_MCB_MASTER1_WEIGHT				1
#define ASPEN_MCB_MASTER0_WEIGHT				1

	// PHY Control Register 3
	// offset 0x00000140
	// Morona value:	0x200050A0
	//					0010000000000000 0 1 0 1 00 001 010 0000
	// Aspen value:		0x20004433
	//					0010000000000000 0 1 0 0 01 000 011 0011
#define ASPEN_PHYCR3_RSVD1						0x2000			// FIXME: reserved is non-zero. Why?
#define ASPEN_PHY_RFIFO_RDRST_EARLY				0
#define ASPEN_PHY_RFIFO_RDRST_EN				1
#define ASPEN_PHYCR3_RSVD2						0
#define ASPEN_DQ_OEN_EXTEND						0
#define ASPEN_DQ_OEN_DLY						1
#define ASPEN_RD_EXT_DLY						0
#define ASPEN_PHY_RFIFO_RPTR_DLY_VAL			3
#define ASPEN_DQ_EXT_DLY						3

	// PHY Control Register 7
	// offset 0x000001d0
	// Morona value:	0x03300771
	//					00 00 0011 0011 0000 0000 0111 0111 00 0 1
	// Aspen value:		0x1ff00779 (156mhz ddr)
	//					00 01 1111 1111 0000 0000 0111 0111 10 0 1
	// Aspen value:		0x27784779 (312mhz ddr)
	//					00 10 0111 0111 1000 0100 0111 0111 10 0 1
#define ASPEN_PHYCTLR7_RSVD1					0
#define ASPEN_PHY_QS_VREF_SEL					1
#define ASPEN_PHY_DQ_ZPDRV						0xf
#define ASPEN_PHY_DQ_ZNDRV						0xf
#define ASPEN_PHY_DQ_ZPTRM						0
#define ASPEN_PHY_DQ_ZNTRM						0
#define ASPEN_PHY_DQ_ZNR						7				// FIXME: different from default. Why?
#define ASPEN_PHY_DQ_ZPR						7				// FIXME: different from default. Why?
#define ASPEN_PHY_DQ_VREF_SEL					2
#define ASPEN_PHY_DQ_ZD							0
#define ASPEN_PHY_DQ_MODE						1

	// PHY Control Register 8
	// offset 0x000001e0
	// ASPEN value:	0x03300770
	//					0000 0011 0011 0000 0000 0111 0111 00 0 0
#define ASPEN_PHY_ADCM_ZPDRV					3
#define ASPEN_PHY_ADCM_ZNDRV					3
#define ASPEN_PHY_ADCM_ZPTRM					0
#define ASPEN_PHY_ADCM_ZNTRM					0
#define ASPEN_PHY_ADCM_ZNR						7
#define ASPEN_PHY_ADCM_ZPR						7
#define ASPEN_PHY_ADCM_VREF_SEL				0
#define ASPEN_PHY_ADCM_ZD						0
#define ASPEN_PHY_ADCM_MODE					0

	// PHY Control Register 9
	// offset 0x000001f0
	// Morona value:	0x00000077
	//					0 0 000 000 00 0 0 000 000 000 000 0111 0111
	// Aspen value:		0xc0000077 (156mhz ddr)
	//					1 1 000 000 00 0 0 000 000 000 000 0111 0111
	// Aspen value:		0x40000077 (312mhz ddr)
	//					0 1 000 000 00 0 0 000 000 000 000 0111 0111
#define ASPEN_PHY_DQ_RCVTYPE					1
#define ASPEN_PHY_QS_RCVTYPE					1
#define ASPEN_PHY_DQ_RCVEP						0
#define ASPEN_PHY_DQ_RCVEN						0
#define ASPEN_PHYCTRLR9_RSVD1					0
#define ASPEN_PHY_CK_ZD							0
#define ASPEN_PHYCTRLR9_RSVD2					0
#define ASPEN_PHY_WCK_DQ_DLY					0
#define ASPEN_PHY_QCK_QS_DLY					0
#define ASPEN_PHY_WCK_AC_DLY					0
#define ASPEN_PHY_WCK_CK_DLY					0
#define ASPEN_PHY_CK_ZNR						7
#define ASPEN_PHY_CK_ZPR						7

	// PHY Control Register 11
	// offset 0x00000210
	// ASPEN value:	0x00300003
	//					0000 00 00 0 011 0 000 0000 00 0 0 00 00 00 1 1
#define ASPEN_PLL_RESERVE						0
#define ASPEN_PLL_VCO_DIV_SEL					0
#define ASPEN_PLL_KVCO							3
#define ASPEN_PLL_ICP							0
#define ASPEN_PLL_TEST_MON						0
#define ASPEN_PLL_VCO_WRNG						0
#define ASPEN_PLL_VCOFBST						0
#define ASPEN_PLL_VDDL							0
#define ASPEN_PLL_VDDM							0
#define ASPEN_PLL_PU_PLL						1
#define ASPEN_MC_SYNC_TYPE						1

	// PHY Control Register 13
	// offset 0x00000230
	// Morona value:	0x00007D04
	//					0000 0 00 000000000 0111110 10000 0 1 0 0
	// Aspen value:		0x00d07d06 (156mhz ddr)
	//					0000 0 00 011010000 0111110 10000 0 1 1 0
	// Aspen value:		0x00507d06 (312mhz ddr)
	//					0000 0 00 001010000 0111110 10000 0 1 1 0
#define ASPEN_DLL_RESET_TIMER					0				// FIXME: Different from default. Why?
#define ASPEN_DLL_UPDATE_STALL_MC_DIS			0
#define ASPEN_PHYCTLR13_RSVD1					0
#define ASPEN_DLL_DELAY_TEST					0xd0
#define ASPEN_PHYCR13_RSVD2						0x3e			// FIXME: Different from default. Why?
#define ASPEN_DLL_PHSEL							0x10
#define ASPEN_PHYCTLR13_RSVD3					0
#define ASPEN_DLL_AUTO_UPDATE_EN				1
#define ASPEN_DLL_TEST_EN						1 
#define ASPEN_DLL_BYPASS_EN						0

	// PHY Control Register 14
	// offset 0x00000240
	// Morona value:	0x00000000
	// Aspen value:		0x80000000
	//					1 0 0 0 0000 0000 0000 00000000 00 0 000 0 0
#define ASPEN_PHY_SYNC_EN						1
#define ASPEN_DLL_UPDATE_EN						0
#define ASPEN_PHY_DLL_RST						0
#define ASPEN_PHY_PLL_RST						0
#define ASPEN_PHYCTLR14_RSVD1					0
#define ASPEN_PHY_CAL_ZPR						0
#define ASPEN_PHY_CAL_ZNR						0
#define ASPEN_DLL_DELAY_OUT						0
#define ASPEN_PHYCTLR14_RSVD2					0
#define ASPEN_DLL_CLK_TST						0
#define ASPEN_PHYCTLR14_RSVD3					0
#define ASPEN_PLL_PIN_TST						0
#define ASPEN_PLL_PLL_LOCK						0 // FIXME: should this be PHY_PLL_LOCK?

	// FIXME: where's the settings for dq[7:0]?
	// PHY DLL Control Register 1
	// offset 0x00000e10
	// Morona value:	0x00007D04
	//					0000000 000000000 0 11111 0 10000 0 1 0 0
	// Aspen value:		0x00d07d06 (156mhz ddr)
	//					0000000 011010000 0 11111 0 10000 0 1 1 0
	// Aspen value:		0x00507d06
	//					0000000 001010000 0 11111 0 10000 0 1 1 0
#define ASPEN_DLLCTLR1_RSVD1					0
#define ASPEN_PHYDLLCR1_DLL_DELAY_TEST			0xd0
#define ASPEN_DLLCTLR1_RSVD2					0
#define ASPEN_PHYDLLCR1_DLL_PHSEL_BYPASS		0x1f
#define ASPEN_DLLCTLR1_RSVD3					0
#define ASPEN_PHYDLLCR1_DLL_PHSEL				0x10
#define ASPEN_DLLCTLR1_RSVD4					0
#define ASPEN_PHYDLLCR1_DLL_AUTO_UPDATE_EN		1
#define ASPEN_PHYDLLCR1_DLL_TEST_EN				1
#define ASPEN_PHYDLLCR1_DLL_BYPASS_EN			0

	// PHY DLL Control Register 2
	// offset 0x00000e20
	// ASPEN value:	0x00007D04
	//					0000000 000000000 0 11111 0 10000 0 1 0 0
#define ASPEN_PHYDLLCR2_DLL_DELAY_TEST			0
#define ASPEN_PHYDLLCR2_DLL_PHSEL_BYPASS		0x1f
#define ASPEN_PHYDLLCR2_DLL_PHSEL				0x10
#define ASPEN_PHYDLLCR2_DLL_AUTO_UPDATE_EN		1
#define ASPEN_PHYDLLCR2_DLL_TEST_EN			0
#define ASPEN_PHYDLLCR2_DLL_BYPASS_EN			0

	// PHY DLL Control Register 3
	// offset 0x00000e30
	// ASPEN value:	0x00007D04
	//					0000000 000000000 0 11111 0 10000 0 1 0 0
#define ASPEN_PHYDLLCR3_DLL_DELAY_TEST			0
#define ASPEN_PHYDLLCR3_DLL_PHSEL_BYPASS		0x1f
#define ASPEN_PHYDLLCR3_DLL_PHSEL				0x10
#define ASPEN_PHYDLLCR3_DLL_AUTO_UPDATE_EN		1
#define ASPEN_PHYDLLCR3_DLL_TEST_EN			0
#define ASPEN_PHYDLLCR3_DLL_BYPASS_EN			0

	// Mememory Address Map Register 0
	// offset 0x00000100
	// Morona value:	0x000C0001
	//					000000000 000 1100 000000000 000000 1
	// Aspen value:		0x000b0001
	//					000000000 000 1011 000000000 000000 1
#define ASPEN_MEMADDRMAPR0_START_ADDR			0
#define ASPEN_MEMADDRMAPR0_RSVD1				0
#define ASPEN_MEMADDRMAPR0_AREA_LENGTH			0xb
#define ASPEN_MEMADDRMAPR0_ADDR_MASK			0
#define ASPEN_MEMADDRMAPR0_CS_VALID				1

	// Mememory Address Map Register 1
	// offset 0x00000110
	// ASPEN value: 0x00000000
#define ASPEN_MEMADDRMAPR1_START_ADDR			0
#define ASPEN_MEMADDRMAPR1_RSVD1				0
#define ASPEN_MEMADDRMAPR1_AREA_LENGTH			0
#define ASPEN_MEMADDRMAPR1_RSVD1				0
#define ASPEN_MEMADDRMAPR1_ADDR_RSVD2			0
#define ASPEN_MEMADDRMAPR1_CS_VALID				0

	// User Initiated Command Register 0
	// offset 0x00000120
	// Morona value:	0x00000001
	//					00 00 0000 0000000000 0 0 0 0 0 0 00 0 0 000 1
	// Aspen value:		0x000000001
	//					00 00 0000 0000000000 0 0 0 0 0 0 00 0 0 000 1
#define ASPEN_USER_DPD_REQ						0
#define ASPEN_CHIP_SELECT						0
#define ASPEN_USER_ZQ_SHORT						0
#define ASPEN_USER_ZQ_LONG						0
#define ASPEN_USER_LMR3_REQ						0
#define ASPEN_USER_LMR2_REQ						0
#define ASPEN_USER_LMR1_REQ						0
#define ASPEN_USER_LMR0_REQ						0
#define ASPEN_USER_SR_REQ						0
#define ASPEN_USER_PRE_PS_REQ					0
#define ASPEN_USER_ACT_PS_REQ					0
#define ASPEN_SDRAM_INIT_REQ					1

#endif
