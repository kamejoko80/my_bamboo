/******************************************************************************
**  COPYRIGHT  2007 Marvell Inernational Ltd.
**  All Rights Reserved
******************************************************************************/



#ifndef __SDRAM_REGISTERS_DEFAULTS_H__
#define __SDRAM_REGISTERS_DEFAULTS_H__

// default mcu register values for ASPEN
#include "mcu.h"
#include "sdram_fields_defaults.h"


// group the fields so there is one definition per register:

	
	// SDRAM Configuration Register 0 - CS0, Type 1
	// offset 0x00000020
	// ASPEN value:	0x00002330
	//					0 000 000000 00 000 000 10 0011 0011 0000
#define ASPEN_SDRCFGREG0														 \
			(																	 \
			( ASPEN_NumBanks    << MCU_SDRAM_CONFIG_0_CS0_BANK_NUMBER_BASE  ) | \
			( ASPEN_NumRows     << MCU_SDRAM_CONFIG_0_CS0_NO_OF_ROW_BASE    ) | \
			( ASPEN_NumColumns  << MCU_SDRAM_CONFIG_0_CS0_NO_OF_COL_BASE    )   \
			)

	// SDRAM Configuration Register 0 - CS0, Type 1
	// offset 0x00000030
	// ASPEN value:	0
#define ASPEN_SDRCFGREG1 0														 

	// SDRAM Timing Register 1
	// offset 0x00000050
	// ASPEN value:	0x49150070
	//					010 010 0100 010101 0000000001110000
#define ASPEN_SDRTMGREG1												\
			(															\
			( ASPEN_tCCD		<< MCU_SDRAM_TIMING_1_TCCD_BASE)	|	\
			( ASPEN_tRTP		<< MCU_SDRAM_TIMING_1_TRTP_BASE)	|	\
			( ASPEN_tWTR		<< MCU_SDRAM_TIMING_1_TWTR_BASE)	|	\
			( ASPEN_tRC		<< MCU_SDRAM_TIMING_1_TRC_BASE)		|	\
			( ASPEN_tREFI	    << MCU_SDRAM_TIMING_1_TREFI_BASE)		\
			)

	// SDRAM Timing Register 2
	// offset 0x00000060
	// ASPEN value:	0x63330332
	//					0110 0011 0011 0011 000 000110011 0 010
#define ASPEN_SDRTMGREG2												\
			(															\
			( ASPEN_tRP	    << MCU_SDRAM_TIMING_2_TRP_BASE)		|	\
			( ASPEN_tRRD	    << MCU_SDRAM_TIMING_2_TRRD_BASE)	|	\
			( ASPEN_tRCD	    << MCU_SDRAM_TIMING_2_TRCD_BASE)	|	\
			( ASPEN_tWR	    << 16)								|	\
			( ASPEN_tRFC	    << MCU_SDRAM_TIMING_2_TRFC_BASE)	|	\
			( ASPEN_tMRD	    << MCU_SDRAM_TIMING_2_TMRD_BASE)		\
			)

	// SDRAM Timing Register 3
	// offset 0x00000190
	// ASPEN value:	0x60C81CFD
	//					0110 0 0 0011001000 00011100 111 11 101
#define ASPEN_SDRTMGREG3												\
			(															\
			( ASPEN_tMOD			<< 28)							|	\
			( ASPEN_tXSRD			<< 16)							|	\
			( ASPEN_tXSNR			<< 16)							|	\
			( ASPEN_tXARDS			<< 5)							|	\
			( ASPEN_SDRTR3_RSVD2	<< 3)							|	\
			( ASPEN_tXP			<< 0)								\
			)


	// SDRAM Timing Register 4
	// offset 0x000001c0
	// ASPEN value:	0x3282A050
	//					0 011 00101000 001 0101000 0001010000
#define ASPEN_SDRTMGREG4															\
			(																		\
			( ASPEN_tCKE			<< MCU_SDRAM_TIMING_4_TCKE_BASE)			|	\
			( ASPEN_INIT_COUNT		<< 20)										|	\
			( ASPEN_tRWD_EXT_DLY	<< MCU_SDRAM_TIMING_4_TRWD_EXT_DLY_BASE)	|	\
			( ASPEN_RESET_COUNT	<< 10)										|	\
			( ASPEN_INIT_COUNT_NOP	<< 0)											\
			)

	// SDRAM Timing Register 5
	// offset 0x00000650
	// ASPEN value:	0x001000A1
	//					0000000000 010000 0 000 00 001010 0 001
#define ASPEN_SDRTMGREG5															\
			(																		\
			( ASPEN_tRAS					<< MCU_SDRAM_TIMING_5_TRAS_BASE)	|	\
			( ASPEN_tCCD_CCS_WR_EXT_DLY	<< 12)								|	\
			( ASPEN_tFAW					<< 4)								|	\
			( ASPEN_tCCD_CCS_EXT_DLY		<< 0)									\
			)

	// SDRAM Control Register 1
	// offset 0x00000080
	// ASPEN value:	0x00000000
#define ASPEN_SDRCTLREG1													    		\
			(																			\
			( ASPEN_APS_EN			    << 31 )										|	\
			( ASPEN_APS_TYPE			<< MCU_SDRAM_CONTROL_1_APS_TYPE_BASE )		|	\
			( ASPEN_APS_VALUE		    << MCU_SDRAM_CONTROL_1_APS_VALUE_BASE )		|	\
			( ASPEN_ACS_EXIT_DLY		<< MCU_SDRAM_CONTROL_1_ACS_EXIT_DLY_BASE )	|	\
			( ASPEN_ACS_EN			    << 7 )										|	\
			( ASPEN_DLL_RESET		    << 6 )										|	\
			( ASPEN_CAS_BT			    << 4 )										|	\
			( ASPEN_OUT_EN			    << 3 )										|	\
			( ASPEN_WRITE_LEVEL_EN	    << 2 )										|	\
			( ASPEN_tW2R_DIS			<< 1 )											\
			)

	// SDRAM Control Register 2
	// offset 0x00000090
	// ASPEN value:	0x00080000
	//					0000 0 000 00 001000 000000 0 0 0 0 0 0 0 0 0 0 
#define ASPEN_SDRCTLREG2																	\
			(																				\
			( ASPEN_REF_POSTED_EN		    << 27)										|	\
			( ASPEN_REF_POSTED_MAX		    << MCU_SDRAM_CONTROL_2_REF_POSTED_MAX_BASE)	|	\
			( ASPEN_SDRAM_LINE_BOUNDARY	<< 16)										| 	\
			( ASPEN_REFPB_MODE			    << 9) 										| 	\
			( ASPEN_PD_MODE				<< 8) 										| 	\
			( ASPEN_F2T_MODE				<< 6) 										| 	\
			( ASPEN_RDIMM_MODE			    << 5) 										| 	\
			( ASPEN_APRECHARGE			    << 4) 										| 	\
			( ASPEN_WRITE_LEVEL_MODE		<< 2) 										| 	\
			( ASPEN_INT_SHADOW_MODE		<< 1) 										| 	\
			( ASPEN_TEST_MODE			    << 0) 											\
			)

	// SDRAM Control Register 3
	// offset 0x000000f0
	// ASPEN value:	0x80000000
	//					1 0 0 000000000 0 0 0 0 0000 000 000 0 0 0 0 0 0
#define ASPEN_SDRCTLREG3														 							\
			(																	 							\
			( ASPEN_EARLY_CMD_EN				<< 31) 													| 	\
			( ASPEN_MC_FLOP_REQ_EN				<< 30) 													| 	\
			( ASPEN_MC_FLOP_GRANT_EN			<< 29) 													| 	\
			( ASPEN_WSTR3_EARLY_WRITE_EN		<< 19) 													| 	\
			( ASPEN_MSTR2_EARLY_WRITE_EN		<< 18) 													| 	\
			( ASPEN_MSTR3_FAST_WRITE_EN		<< 17) 													| 	\
			( ASPEN_MSTR2_FAST_WRITE_EN		<< 16) 													| 	\
			( ASPEN_SB_EARLY_WRITE_USERVALUE	<< MCU_SDRAM_CONTROL_3_SB_EARLY_WRITE_USERVALUE_BASE)	| 	\
			( ASPEN_CPU_EARLY_WRITE_USERVALUE	<< MCU_SDRAM_CONTROL_3_CPU_EARLY_WRITE_USERVALUE_BASE)	| 	\
			( ASPEN_SB_EARLY_WRITE_USER		<< 5) 													| 	\
			( ASPEN_CPU_EARLY_WRITE_USER		<< 4) 													| 	\
			( ASPEN_SB_EARLY_WRITE_EN			<< 3) 													| 	\
			( ASPEN_CPU_EARLY_WRITE_EN			<< 2) 													| 	\
			( ASPEN_SB_FAST_WRITE_EN			<< 1) 													| 	\
			( ASPEN_CPU_FAST_WRITE_EN			<< 0) 														\
			)

	// SDRAM Control Register 4
	// offset 0x000001a0
	// ASPEN value:	0x40810005
	//                  0 1 0 0000 010 000 0 0 100 0 000 0 0 0 0 0 001 01
#if 0
#define ASPEN_SDRCTLREG4								\
			(											\
			( ASPEN_SDRAM_DLL_EN			<< 31)	| 	\
			( ASPEN_DQSB_EN				<< 30)	| 	\
			( ASPEN_FAST_BANK				<< 29)	| 	\
			( ASPEN_BURST_LENGTH			<< 22)	| 	\
			( ASPEN_AL_NUMBER			    << 19)	| 	\
			( ASPEN_AL_EN				    << 18)	| 	\
			( ASPEN_RQDS_EN				<< 17)	| 	\
			( ASPEN_CAS_LATENCY			<< 14)	| 	\
			( ASPEN_CAS_LATENCY_LOWER	    << 13)	| 	\
			( ASPEN_CWL					<< 10)	| 	\
			( ASPEN_S4_TYPE				<< 8)	| 	\
			( ASPEN_ASR					<< 7)	| 	\
			( ASPEN_SRT					<< 6)	| 	\
			( ASPEN_MPR					<< 5)	| 	\
			( ASPEN_SDRAM_TYPE			    << 2)	| 	\
			( ASPEN_DATA_WIDTH			    << 0)		\
			)
#endif
#define ASPEN_SDRCTLREG4								\
			(											\
			( ASPEN_SDRAM_DLL_EN			<< 31)	| 	\
			( ASPEN_DQSB_EN				<< 30)	| 	\
			( ASPEN_FAST_BANK				<< 29)	| 	\
			( ASPEN_BURST_LENGTH			<< 22)	| 	\
			( ASPEN_AL_NUMBER			    << 19)	| 	\
			( ASPEN_AL_EN				    << 18)	| 	\
			( ASPEN_RQDS_EN				<< 17)	| 	\
			( ASPEN_CAS_LATENCY			<< 14)	| 	\
			( ASPEN_SDRAM_TYPE			    << 2)	| 	\
			( ASPEN_DATA_WIDTH			    << 0)		\
			)

	// SDRAM Control Register 5
	// offset 0x00000280
	// ASPEN value:	0x01010101
	//					0000 0001 0000 0001 0000 0001 0000 0001
#define ASPEN_SDRCTLREG5																		\
			(																					\
			( ASPEN_MASTER_3_WEIGHT	    << MCU_SDRAM_CONTROL_5_MASTER_3_WEIGHT_BASE)	|	\
			( ASPEN_MASTER_2_WEIGHT	    << MCU_SDRAM_CONTROL_5_MASTER_2_WEIGHT_BASE)	|	\
			( ASPEN_MASTER_1_WEIGHT	    << MCU_SDRAM_CONTROL_5_MASTER_1_WEIGHT_BASE)	|	\
			( ASPEN_MASTER_0_WEIGHT	    << MCU_SDRAM_CONTROL_5_MASTER_0_WEIGHT_BASE)		\
			)

	// SDRAM Control Register 6
	// offset 0x00000760
	// ASPEN value:	0x00000000
#define ASPEN_SDRCTLREG6							\
			(										\
			( ASPEN_ODT3_READ_EN		<< 28)	|	\
			( ASPEN_ODT3_WRITE_EN	    << 24)	|	\
			( ASPEN_ODT2_READ_EN		<< 20)	|	\
			( ASPEN_ODT2_WRITE_EN	    << 16)	|	\
			( ASPEN_ODT1_READ_EN		<< 12)	|	\
			( ASPEN_ODT1_WRITE_EN	    << 8) 	|	\
			( ASPEN_ODT0_READ_EN		<< 4) 	|	\
			( ASPEN_ODT0_WRITE_EN	    << 0) 		\
			)

	// SDRAM Control Register 7
	// offset 0x00000770
	// ASPEN value: 0x00000000
#define ASPEN_SDRCTLREG7								\
			(											\
			( ASPEN_FORCE_ODT				<< 28)	|	\
			( ASPEN_PAD_TERM_SWITCH_MODE	<< 24)	|	\
			( ASPEN_ODT3_SWITCH_MODE		<< 6)	|	\
			( ASPEN_ODT2_SWITCH_MODE		<< 4)	|	\
			( ASPEN_ODT1_SWITCH_MODE		<< 2)	|	\
			( ASPEN_ODT0_SWITCH_MODE		<< 0)		\
			)

	// MCB Control Register 1
	// offset 0x00000510
	// ASPEN value:	0x01010101
	//					0000 0001 0000 0001 0000 0001 0000 0001
#define ASPEN_MCBCTLREG1														 \
			(																	 \
			( ASPEN_MCB_MASTER3_WEIGHT	    << MCU_MCB_CONTROL_1_MCB_MASTER_3_WEIGHT_BASE) | \
			( ASPEN_MCB_MASTER2_WEIGHT	    << MCU_MCB_CONTROL_1_MCB_MASTER_2_WEIGHT_BASE) | \
			( ASPEN_MCB_MASTER1_WEIGHT	    << MCU_MCB_CONTROL_1_MCB_MASTER_1_WEIGHT_BASE) | \
			( ASPEN_MCB_MASTER0_WEIGHT	    << MCU_MCB_CONTROL_1_MCB_MASTER_0_WEIGHT_BASE)   \
			)

	// PHY Control Register 3
	// offset 0x00000140
	// ASPEN value:	0x200050A0
	//					0010000000000000 0 1 0 1 00 001 010 0000
#define ASPEN_PHYCTLREG3									\
			(												\
			( ASPEN_PHYCR3_RSVD1				<< 16)	|	\
			( ASPEN_PHY_RFIFO_RDRST_EARLY	    << 15)	|	\
			( ASPEN_PHY_RFIFO_RDRST_EN		    << 14)	|	\
			( ASPEN_DQ_OEN_EXTEND			    << 12)	|	\
			( ASPEN_DQ_OEN_DLY				    << 10)	|	\
			( ASPEN_RD_EXT_DLY				    << 7)	|	\
			( ASPEN_PHY_RFIFO_RPTR_DLY_VAL	    << 4)	|	\
			( ASPEN_DQ_EXT_DLY				    << 0)		\
			)

	// PHY Control Register 7
	// offset 0x000001d0
	// ASPEN value:	0x03300771
	//					00 00 0011 0011 0000 0000 0111 0111 00 0 1
#define ASPEN_PHYCTLREG7																	\
			(																				\
			( ASPEN_PHY_QS_VREF_SEL	    << MCU_PHY_CONTROL_7_PHY_QS_VREF_SEL_BASE)	|	\
			( ASPEN_PHY_DQ_ZPDRV		    << 24)										|	\
			( ASPEN_PHY_DQ_ZNDRV		    << 20)										|	\
			( ASPEN_PHY_DQ_ZPTRM		    << MCU_PHY_CONTROL_7_PHY_DQ_ZPTRM_BASE)		|	\
			( ASPEN_PHY_DQ_ZNTRM		    << MCU_PHY_CONTROL_7_PHY_DQ_ZNTRM_BASE)		|	\
			( ASPEN_PHY_DQ_ZNR				<< MCU_PHY_CONTROL_7_PHY_DQ_ZNR_BASE)		|	\
			( ASPEN_PHY_DQ_ZPR				<< MCU_PHY_CONTROL_7_PHY_DQ_ZPR_BASE)		|	\
			( ASPEN_PHY_DQ_VREF_SEL	    << MCU_PHY_CONTROL_7_PHY_DQ_VREF_SEL_BASE)	|	\
			( ASPEN_PHY_DQ_ZD				<< 1)										|	\
			( ASPEN_PHY_DQ_MODE		    << 0)											\
			)

	// PHY Control Register 8
	// offset 0x000001e0
	// ASPEN value:	0x03300770
	//					0000 0011 0011 0000 0000 0111 0111 00 0 0
#define ASPEN_PHYCTLREG8																		\
			(																					\
			( ASPEN_PHY_ADCM_ZPDRV		    << 24)											|	\
			( ASPEN_PHY_ADCM_ZNDRV		    << 20)											|	\
			( ASPEN_PHY_ADCM_ZPTRM		    << MCU_PHY_CONTROL_8_PHY_ADCM_ZPTRM_BASE)		|	\
			( ASPEN_PHY_ADCM_ZNTRM		    << MCU_PHY_CONTROL_8_PHY_ADCM_ZNTRM_BASE)		|	\
			( ASPEN_PHY_ADCM_ZNR			<< MCU_PHY_CONTROL_8_PHY_ADCM_ZNR_BASE)			|	\
			( ASPEN_PHY_ADCM_ZPR			<< MCU_PHY_CONTROL_8_PHY_ADCM_ZPR_BASE)			|	\
			( ASPEN_PHY_ADCM_VREF_SEL	    << MCU_PHY_CONTROL_8_PHY_ADCM_VREF_SEL_BASE)	|	\
			( ASPEN_PHY_ADCM_ZD			<< 1)											|	\
			( ASPEN_PHY_ADCM_MODE		    << 0)												\
			)

	// PHY Control Register 9
	// offset 0x000001f0
	// ASPEN value:	0x00000077
	//					0 0 000 000 00 0 0 000 000 000 000 0111 0111
#define ASPEN_PHYCTLREG9																\
			(																			\
			( ASPEN_PHY_DQ_RCVTYPE	    << 31)										|	\
			( ASPEN_PHY_QS_RCVTYPE	    << 30)										|	\
			( ASPEN_PHY_DQ_RCVEP		<< MCU_PHY_CONTROL_9_PHY_DQ_RCVEP_BASE)		|	\
			( ASPEN_PHY_DQ_RCVEN		<< MCU_PHY_CONTROL_9_PHY_DQ_RCVEN_BASE)		|	\
			( ASPEN_PHY_CK_ZD		    << 21)										|	\
			( ASPEN_PHY_WCK_DQ_DLY	    << MCU_PHY_CONTROL_9_PHY_WCK_DQ_DLY_BASE)	|	\
			( ASPEN_PHY_QCK_QS_DLY	    << MCU_PHY_CONTROL_9_PHY_WCK_QS_DLY_BASE)	|	\
			( ASPEN_PHY_WCK_AC_DLY	    << MCU_PHY_CONTROL_9_PHY_WCK_AC_DLY_BASE)	|	\
			( ASPEN_PHY_WCK_CK_DLY	    << MCU_PHY_CONTROL_9_PHY_WCK_CK_DLY_BASE)	|	\
			( ASPEN_PHY_CK_ZNR		    << MCU_PHY_CONTROL_9_PHY_CK_ZNR_BASE)		|	\
			( ASPEN_PHY_CK_ZPR			<< MCU_PHY_CONTROL_9_PHY_CK_ZPR_BASE)			\
			)

	// PHY Control Register 11
	// offset 0x00000210
	// ASPEN value:	0x00300003
	//					0000 00 00 0 011 0 000 0000 00 0 0 00 00 00 1 1
#define ASPEN_PHYCTLREG11									\
			(												\
			( ASPEN_PLL_RESERVE			    << 26) 	| 	\
			( ASPEN_PLL_VCO_DIV_SEL		    << 24) 	| 	\
			( ASPEN_PLL_KVCO				    << 20) 	| 	\
			( ASPEN_PLL_ICP				    << 16) 	| 	\
			( ASPEN_PLL_TEST_MON			    << 12) 	| 	\
			( ASPEN_PLL_VCO_WRNG			    << 10) 	| 	\
			( ASPEN_PLL_VCOFBST			    <<  8) 	| 	\
			( ASPEN_PLL_VDDL				    <<  6) 	| 	\
			( ASPEN_PLL_VDDM				    <<  4) 	| 	\
			( ASPEN_PLL_PU_PLL					<<  1) 	| 	\
			( ASPEN_MC_SYNC_TYPE			    <<  0)		\
			)

	// PHY Control Register 13
	// offset 0x00000230
	// ASPEN value:	0x00007D04
	//					0000 0 00 000000000 0111110 10000 0 1 0 0
#define ASPEN_PHYCTLREG13																		\
			(																					\
			( ASPEN_DLL_RESET_TIMER			<< MCU_PHY_CONTROL_13_DLL_RESET_TIMER_BASE)	|	\
			( ASPEN_DLL_UPDATE_STALL_MC_DIS	<< 27)										|	\
			( ASPEN_DLL_DELAY_TEST			    << MCU_PHY_CONTROL_13_DLL_DELAY_TEST_BASE)	|	\
			( ASPEN_PHYCR13_RSVD2			    << 9)										|	\
			( ASPEN_DLL_PHSEL				    << MCU_PHY_CONTROL_13_DLL_PHSEL_BASE)		|	\
			( ASPEN_DLL_AUTO_UPDATE_EN		    << 2)										|	\
			( ASPEN_DLL_TEST_EN				<< 1)										|	\
			( ASPEN_DLL_BYPASS_EN			    << 0)											\
			)

	// PHY Control Register 14
	// offset 0x00000240
	// ASPEN value:	0x00000000
#define ASPEN_PHYCTLREG14																	\
			(																				\
			( ASPEN_PHY_SYNC_EN		    << 31) 										| 	\
			( ASPEN_DLL_UPDATE_EN			<< 30) 										| 	\
			( ASPEN_PHY_DLL_RST		    << 29) 										| 	\
			( ASPEN_PHY_PLL_RST		    << 28) 										| 	\
			( ASPEN_PHY_CAL_ZPR		    << MCU_PHY_CONTROL_14_PHY_CAL_ZPR_BASE)		| 	\
			( ASPEN_PHY_CAL_ZNR		    << MCU_PHY_CONTROL_14_PHY_CAL_ZNR_BASE)		| 	\
			( ASPEN_DLL_DELAY_OUT			<< MCU_PHY_CONTROL_14_DLL_DELAY_OUT_BASE)	| 	\
			( ASPEN_DLL_CLK_TST		    << 5) 										| 	\
			( ASPEN_PLL_PIN_TST		    << 1) 										| 	\
			( ASPEN_PLL_PLL_LOCK			<< 0) 											\
			)

	// FIXME: where's the settings for dq[7:0]?
	// PHY DLL Control Register 1
	// offset 0x00000e10
	// ASPEN value:	0x00007D04
	//					0000000 000000000 0 11111 0 10000 0 1 0 0
#define ASPEN_DLLCTLREG1																				\
			(																							\
			( ASPEN_PHYDLLCR1_DLL_DELAY_TEST		<< MCU_PHY_DLL_CONTROL_1_DLL1_DELAY_TEST_BASE)	|	\
			( ASPEN_PHYDLLCR1_DLL_PHSEL_BYPASS		<< MCU_PHY_DLL_CONTROL_1_DLL1_PHSEL_BYASS_BASE)	|	\
			( ASPEN_PHYDLLCR1_DLL_PHSEL			<< MCU_PHY_DLL_CONTROL_1_DLL1_PHSEL_BASE)		|	\
			( ASPEN_PHYDLLCR1_DLL_AUTO_UPDATE_EN	<< 2) 											|	\
			( ASPEN_PHYDLLCR1_DLL_TEST_EN			<< 1) 											|	\
			( ASPEN_PHYDLLCR1_DLL_BYPASS_EN		<< 0) 											\
			)

	// PHY DLL Control Register 2
	// offset 0x00000e20
	// ASPEN value:	0x00007D04
	//					0000000 000000000 0 11111 0 10000 0 1 0 0
#define ASPEN_DLLCTLREG2																				\
			(																							\
			( ASPEN_PHYDLLCR2_DLL_DELAY_TEST		<< MCU_PHY_DLL_CONTROL_2_DLL2_DELAY_TEST_BASE)	|	\
			( ASPEN_PHYDLLCR2_DLL_PHSEL_BYPASS		<< MCU_PHY_DLL_CONTROL_2_DLL2_PHSEL_BYASS_BASE) |	\
			( ASPEN_PHYDLLCR2_DLL_PHSEL			<< MCU_PHY_DLL_CONTROL_2_DLL2_PHSEL_BASE)		|	\
			( ASPEN_PHYDLLCR2_DLL_AUTO_UPDATE_EN	<< 2)											|	\
			( ASPEN_PHYDLLCR2_DLL_TEST_EN			<< 1)											|	\
			( ASPEN_PHYDLLCR2_DLL_BYPASS_EN		<< 0)												\
			)

	// PHY DLL Control Register 3
	// offset 0x00000e30
	// ASPEN value:	0x00007D04
	//					0000000 000000000 0 11111 0 10000 0 1 0 0
#define ASPEN_DLLCTLREG3																				\
			(																							\
			( ASPEN_PHYDLLCR3_DLL_DELAY_TEST		<< MCU_PHY_DLL_CONTROL_3_DLL3_DELAY_TEST_BASE)	|	\
			( ASPEN_PHYDLLCR3_DLL_PHSEL_BYPASS		<< MCU_PHY_DLL_CONTROL_3_DLL3_PHSEL_BYASS_BASE)	|	\
			( ASPEN_PHYDLLCR3_DLL_PHSEL			<< MCU_PHY_DLL_CONTROL_3_DLL3_PHSEL_BASE)		|	\
			( ASPEN_PHYDLLCR3_DLL_AUTO_UPDATE_EN	<< 2) 											|	\
			( ASPEN_PHYDLLCR3_DLL_TEST_EN			<< 1) 											|	\
			( ASPEN_PHYDLLCR3_DLL_BYPASS_EN		<< 0) 												\
			)

	// Mememory Address Map Register 0
	// offset 0x00000100
	// ASPEN value:	0x000C0001
	//					000000000 000 1100 000000000 000000 1
#define ASPEN_ADRMAPREG0																	\
			(																				\
			( ASPEN_MEMADDRMAPR0_START_ADDR		<< MMU_MMAP0_START_ADDRESS_BASE)	|	\
			( ASPEN_MEMADDRMAPR0_AREA_LENGTH		<< MMU_MMAP0_AREA_LENGTH_BASE)		|	\
			( ASPEN_MEMADDRMAPR0_ADDR_MASK			<< MMU_MMAP0_ADDRESS_MASK_BASE)		|	\
			( ASPEN_MEMADDRMAPR0_CS_VALID			<< 0)									\
			)

	// Mememory Address Map Register 1
	// offset 0x00000110
	// ASPEN value: 0x00000000
#define ASPEN_ADRMAPREG1																	\
			(																				\
			( ASPEN_MEMADDRMAPR1_START_ADDR		<< MMU_MMAP1_START_ADDRESS_BASE)	|	\
			( ASPEN_MEMADDRMAPR1_AREA_LENGTH		<< MMU_MMAP1_AREA_LENGTH_BASE)		|	\
			( ASPEN_MEMADDRMAPR1_ADDR_MASK			<< MMU_MMAP1_ADDRESS_MASK_BASE)		|	\
			( ASPEN_MEMADDRMAPR1_CS_VALID			<< 0)									\
			)

	// User Initiated Command Register 0
	// offset 0x00000120
	// ASPEN value:	0x00000001
	//					00 00 0000 0000000000 0 0 0 0 0 0 00 0 0 000 1
#define ASPEN_USRCMDREG0																	\
			(																				\
			( ASPEN_USER_DPD_REQ		<< 28) 											| 	\
			( ASPEN_CHIP_SELECT		<< 24) 											| 	\
			( ASPEN_USER_ZQ_SHORT	    << 13) 											| 	\
			( ASPEN_USER_ZQ_LONG		<< 12) 											| 	\
			( ASPEN_USER_LMR3_REQ	    << 11) 											| 	\
			( ASPEN_USER_LMR2_REQ	    << 10) 											| 	\
			( ASPEN_USER_LMR1_REQ	    << 9) 											| 	\
			( ASPEN_USER_LMR0_REQ	    << 8) 											| 	\
			( ASPEN_USER_SR_REQ		<< MCU_USER_INITIATED_COMMAND_USER_SR_REQ_BASE)	| 	\
			( ASPEN_USER_PRE_PS_REQ	<< 5) 											|	\
			( ASPEN_USER_ACT_PS_REQ	<< 4) 											|	\
			( ASPEN_SDRAM_INIT_REQ	    << 0) 												\
			)

#endif
