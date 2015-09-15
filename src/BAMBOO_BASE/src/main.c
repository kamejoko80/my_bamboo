#include "PlatformConfig.h"
#include "sdram_config.h"
#include "uart.h"
#include "gpio.h"

extern unsigned long SetOperationMode(void);

// MPMU Register definition
#define MPMU_BASE         (0xD4050000)
#define MPMU_FCCR        *(volatile unsigned int*)(MPMU_BASE + 0x0008)
#define MPMU_POCR        *(volatile unsigned int*)(MPMU_BASE + 0x000C)
#define MPMU_POSR        *(volatile unsigned int*)(MPMU_BASE + 0x0010)
#define MPMU_SUCCR       *(volatile unsigned int*)(MPMU_BASE + 0x0014)
#define MPMU_OHCR        *(volatile unsigned int*)(MPMU_BASE + 0x001C)
#define MPMU_PLL2CR      *(volatile unsigned int*)(MPMU_BASE + 0x0034)
#define MPMU_PLL1_REG1   *(volatile unsigned int*)(MPMU_BASE + 0x0050)
#define MPMU_PLL1_REG2   *(volatile unsigned int*)(MPMU_BASE + 0x0054)
#define MPMU_PLL2_REG1   *(volatile unsigned int*)(MPMU_BASE + 0x0060)
#define MPMU_PLL2_REG2   *(volatile unsigned int*)(MPMU_BASE + 0x0064)
#define MPMU_PLL2_SSC    *(volatile unsigned int*)(MPMU_BASE + 0x0068)
#define MPMU_TS          *(volatile unsigned int*)(MPMU_BASE + 0x0080)
#define MPMU_WDTPCR      *(volatile unsigned int*)(MPMU_BASE + 0x0200)
#define MPMU_APCR        *(volatile unsigned int*)(MPMU_BASE + 0x1000)
#define MPMU_APSR        *(volatile unsigned int*)(MPMU_BASE + 0x1004)
#define MPMU_APRR        *(volatile unsigned int*)(MPMU_BASE + 0x1020)
#define MPMU_ACGR        *(volatile unsigned int*)(MPMU_BASE + 0x1024)
#define MPMU_ARSR        *(volatile unsigned int*)(MPMU_BASE + 0x1028)
#define MPMU_AWUCRS      *(volatile unsigned int*)(MPMU_BASE + 0x1048)
#define MPMU_AWUCRM      *(volatile unsigned int*)(MPMU_BASE + 0x104C)
#define MPMU_ASYSDR      *(volatile unsigned int*)(MPMU_BASE + 0x1050)
#define MPMU_SSPDR       *(volatile unsigned int*)(MPMU_BASE + 0x1054)

// APMU Register definition
#define APMU_BASE               (0xD4282800)
#define APMU_PCR               *(volatile unsigned int*)(APMU_BASE+0x0000)
#define APMU_CCR               *(volatile unsigned int*)(APMU_BASE+0x0004)
#define APMU_CCSR              *(volatile unsigned int*)(APMU_BASE+0x000C)
#define APMU_FC_TIMER          *(volatile unsigned int*)(APMU_BASE+0x0010)
#define APMU_IDLE_CFG          *(volatile unsigned int*)(APMU_BASE+0x0018)
#define APMU_SYNC_MODE_BYPASS  *(volatile unsigned int*)(APMU_BASE+0x00C8)
#define APMU_LCD_CLK_RES_CTRL  *(volatile unsigned int*)(APMU_BASE+0x004C)
#define APMU_CCIC_CLK_RES_CTRL *(volatile unsigned int*)(APMU_BASE+0x0050)
#define APMU_SD1_CLK_RES_CTRL  *(volatile unsigned int*)(APMU_BASE+0x0054)
#define APMU_SD2_CLK_RES_CTRL  *(volatile unsigned int*)(APMU_BASE+0x0058)
#define APMU_USB_CLK_RES_CTRL  *(volatile unsigned int*)(APMU_BASE+0x005C)
#define APMU_NFC_CLK_RES_CTRL  *(volatile unsigned int*)(APMU_BASE+0x0060)
#define APMU_DMA_CLK_RES_CTRL  *(volatile unsigned int*)(APMU_BASE+0x0064)
#define APMU_BUS_CLK_RES_CTRL  *(volatile unsigned int*)(APMU_BASE+0x006C)
#define APMU_WAKE_CLR          *(volatile unsigned int*)(APMU_BASE+0x007C)
#define APMU_CORE_STATUS       *(volatile unsigned int*)(APMU_BASE+0x0090)
#define APMU_RES_FRM_SLP_CLR   *(volatile unsigned int*)(APMU_BASE+0x0094)
#define APMU_IMR               *(volatile unsigned int*)(APMU_BASE+0x0098)
#define APMU_IRWC              *(volatile unsigned int*)(APMU_BASE+0x009C)
#define APMU_ISR               *(volatile unsigned int*)(APMU_BASE+0x00A0)
#define APMU_MC_HW_SLP_TYPE    *(volatile unsigned int*)(APMU_BASE+0x00B0)
#define APMU_MC_SLP_REQ        *(volatile unsigned int*)(APMU_BASE+0x00B4)
#define APMU_MC_SW_SLP_TYPE    *(volatile unsigned int*)(APMU_BASE+0x00C0)
#define APMU_PLL_SEL_STATUS    *(volatile unsigned int*)(APMU_BASE+0x00C4)
#define APMU_SYNC_MODE_BYPASS  *(volatile unsigned int*)(APMU_BASE+0x00C8)
#define APMU_GC_CLK_RES_CTRL   *(volatile unsigned int*)(APMU_BASE+0x00CC)
#define APMU_SMC_CLK_RES_CTRL  *(volatile unsigned int*)(APMU_BASE+0x00D4)
#define APMU_XD_CLK_RES_CTRL   *(volatile unsigned int*)(APMU_BASE+0x00DC)
#define APMU_SD3_CLK_RES_CTRL  *(volatile unsigned int*)(APMU_BASE+0x00E0)
#define APMU_SD4_CLK_RES_CTRL  *(volatile unsigned int*)(APMU_BASE+0x00E4)
#define APMU_CF_CLK_RES_CTRL   *(volatile unsigned int*)(APMU_BASE+0x00F0)
#define APMU_MSP_CLK_RES_CTRL  *(volatile unsigned int*)(APMU_BASE+0x00F4)
#define APMU_CMU_CLK_RES_CTRL  *(volatile unsigned int*)(APMU_BASE+0x00F8)
#define APMU_FE_CLK_RES_CTRL   *(volatile unsigned int*)(APMU_BASE+0x00FC)
#define APMU_PCIE_CLK_RES_CTRL *(volatile unsigned int*)(APMU_BASE+0x100 )
#define APMU_EPD_CLK_RES_CTRL  *(volatile unsigned int*)(APMU_BASE+0x104 )

#define	APBC_BASE	            (0xD4015000)
#define	APBC_TIMERS_CLK_RST	   *(volatile unsigned int*)(APBC_BASE+0x0034)

#define TIM_BASE                (0xD4014000)
#define TMR_CCR                *(volatile unsigned int*)(TIM_BASE+0x0000)
#define TMR_T1_M0              *(volatile unsigned int*)(TIM_BASE+0x0004)
#define TMR_T1_M1              *(volatile unsigned int*)(TIM_BASE+0x0008)
#define TMR_T1_M2              *(volatile unsigned int*)(TIM_BASE+0x000C)
#define TMR_T2_M0              *(volatile unsigned int*)(TIM_BASE+0x0010)
#define TMR_T2_M1              *(volatile unsigned int*)(TIM_BASE+0x0014)
#define TMR_T2_M2              *(volatile unsigned int*)(TIM_BASE+0x0018)
#define TMR_T3_M0              *(volatile unsigned int*)(TIM_BASE+0x001C)
#define TMR_T3_M1              *(volatile unsigned int*)(TIM_BASE+0x0020)
#define TMR_T3_M2              *(volatile unsigned int*)(TIM_BASE+0x0024)
#define TMR_CR1                *(volatile unsigned int*)(TIM_BASE+0x0028)
#define TMR_CR2                *(volatile unsigned int*)(TIM_BASE+0x002C)
#define TMR_CR3                *(volatile unsigned int*)(TIM_BASE+0x0030)
#define TMR_SR1                *(volatile unsigned int*)(TIM_BASE+0x0034)
#define TMR_SR2                *(volatile unsigned int*)(TIM_BASE+0x0038)
#define TMR_SR3                *(volatile unsigned int*)(TIM_BASE+0x003C)
#define TMR_IER1               *(volatile unsigned int*)(TIM_BASE+0x0040)
#define TMR_IER2               *(volatile unsigned int*)(TIM_BASE+0x0044)
#define TMR_IER3               *(volatile unsigned int*)(TIM_BASE+0x0048)
#define TMR_PLVR1              *(volatile unsigned int*)(TIM_BASE+0x004C)
#define TMR_PLVR2              *(volatile unsigned int*)(TIM_BASE+0x0050)
#define TMR_PLVR3              *(volatile unsigned int*)(TIM_BASE+0x0054)
#define TMR_PLCR1              *(volatile unsigned int*)(TIM_BASE+0x0058)
#define TMR_PLCR2              *(volatile unsigned int*)(TIM_BASE+0x005C)
#define TMR_PLCR3              *(volatile unsigned int*)(TIM_BASE+0x0060)
#define TMR_WMER               *(volatile unsigned int*)(TIM_BASE+0x0064)
#define TMR_WMR                *(volatile unsigned int*)(TIM_BASE+0x0068)
#define TMR_WVR                *(volatile unsigned int*)(TIM_BASE+0x006C)
#define TMR_WSR                *(volatile unsigned int*)(TIM_BASE+0x0070)
#define TMR_ICR1               *(volatile unsigned int*)(TIM_BASE+0x0074)
#define TMR_ICR2               *(volatile unsigned int*)(TIM_BASE+0x0078)
#define TMR_ICR3               *(volatile unsigned int*)(TIM_BASE+0x007C)
#define TMR_WICR               *(volatile unsigned int*)(TIM_BASE+0x0080)
#define TMR_CER                *(volatile unsigned int*)(TIM_BASE+0x0084)
#define TMR_CMR                *(volatile unsigned int*)(TIM_BASE+0x0088)
#define TMR_ILR1               *(volatile unsigned int*)(TIM_BASE+0x008C)
#define TMR_ILR3               *(volatile unsigned int*)(TIM_BASE+0x0094)
#define TMR_WCR                *(volatile unsigned int*)(TIM_BASE+0x0098)
#define TMR_WFAR               *(volatile unsigned int*)(TIM_BASE+0x009C)
#define TMR_WSAR               *(volatile unsigned int*)(TIM_BASE+0x00A0)
#define TMR_CVWR               *(volatile unsigned int*)(TIM_BASE+0x00A4)

#define ICU_BASE                   (0xD4282000)
#define ICU_BASE                   (0xD4282000)
#define ICU_INT_CONF0             *(volatile unsigned int*)(ICU_BASE+0x0000)
#define ICU_INT_CONF1             *(volatile unsigned int*)(ICU_BASE+0x0004)
#define ICU_INT_CONF2             *(volatile unsigned int*)(ICU_BASE+0x0008)
#define ICU_INT_CONF3             *(volatile unsigned int*)(ICU_BASE+0x000C)
#define ICU_INT_CONF4             *(volatile unsigned int*)(ICU_BASE+0x0010)
#define ICU_INT_CONF5             *(volatile unsigned int*)(ICU_BASE+0x0014)
#define ICU_INT_CONF6             *(volatile unsigned int*)(ICU_BASE+0x0018)
#define ICU_INT_CONF7             *(volatile unsigned int*)(ICU_BASE+0x001C)
#define ICU_INT_CONF8             *(volatile unsigned int*)(ICU_BASE+0x0020)
#define ICU_INT_CONF9             *(volatile unsigned int*)(ICU_BASE+0x0024)
#define ICU_INT_CONF10            *(volatile unsigned int*)(ICU_BASE+0x0028)
#define ICU_INT_CONF11            *(volatile unsigned int*)(ICU_BASE+0x002C)
#define ICU_INT_CONF12            *(volatile unsigned int*)(ICU_BASE+0x0030)
#define ICU_INT_CONF13            *(volatile unsigned int*)(ICU_BASE+0x0034)
#define ICU_INT_CONF14            *(volatile unsigned int*)(ICU_BASE+0x0038)
#define ICU_INT_CONF15            *(volatile unsigned int*)(ICU_BASE+0x003C)
#define ICU_INT_CONF16            *(volatile unsigned int*)(ICU_BASE+0x0040)
#define ICU_INT_CONF17            *(volatile unsigned int*)(ICU_BASE+0x0044)
#define ICU_INT_CONF18            *(volatile unsigned int*)(ICU_BASE+0x0048)
#define ICU_INT_CONF19            *(volatile unsigned int*)(ICU_BASE+0x004C)
#define ICU_INT_CONF20            *(volatile unsigned int*)(ICU_BASE+0x0050)
#define ICU_INT_CONF21            *(volatile unsigned int*)(ICU_BASE+0x0054)
#define ICU_INT_CONF22            *(volatile unsigned int*)(ICU_BASE+0x0058)
#define ICU_INT_CONF23            *(volatile unsigned int*)(ICU_BASE+0x005C)
#define ICU_INT_CONF24            *(volatile unsigned int*)(ICU_BASE+0x0060)
#define ICU_INT_CONF25            *(volatile unsigned int*)(ICU_BASE+0x0064)
#define ICU_INT_CONF26            *(volatile unsigned int*)(ICU_BASE+0x0068)
#define ICU_INT_CONF27            *(volatile unsigned int*)(ICU_BASE+0x006C)
#define ICU_INT_CONF28            *(volatile unsigned int*)(ICU_BASE+0x0070)
#define ICU_INT_CONF29            *(volatile unsigned int*)(ICU_BASE+0x0074)
#define ICU_INT_CONF30            *(volatile unsigned int*)(ICU_BASE+0x0078)
#define ICU_INT_CONF31            *(volatile unsigned int*)(ICU_BASE+0x007C)
#define ICU_INT_CONF32            *(volatile unsigned int*)(ICU_BASE+0x0080)
#define ICU_INT_CONF33            *(volatile unsigned int*)(ICU_BASE+0x0084)
#define ICU_INT_CONF34            *(volatile unsigned int*)(ICU_BASE+0x0088)
#define ICU_INT_CONF35            *(volatile unsigned int*)(ICU_BASE+0x008C)
#define ICU_INT_CONF36            *(volatile unsigned int*)(ICU_BASE+0x0090)
#define ICU_INT_CONF37            *(volatile unsigned int*)(ICU_BASE+0x0094)
#define ICU_INT_CONF38            *(volatile unsigned int*)(ICU_BASE+0x0098)
#define ICU_INT_CONF39            *(volatile unsigned int*)(ICU_BASE+0x009C)
#define ICU_INT_CONF40            *(volatile unsigned int*)(ICU_BASE+0x00A0)
#define ICU_INT_CONF41            *(volatile unsigned int*)(ICU_BASE+0x00A4)
#define ICU_INT_CONF42            *(volatile unsigned int*)(ICU_BASE+0x00A8)
#define ICU_INT_CONF43            *(volatile unsigned int*)(ICU_BASE+0x00AC)
#define ICU_INT_CONF44            *(volatile unsigned int*)(ICU_BASE+0x00B0)
#define ICU_INT_CONF45            *(volatile unsigned int*)(ICU_BASE+0x00B4)
#define ICU_INT_CONF46            *(volatile unsigned int*)(ICU_BASE+0x00B8)
#define ICU_INT_CONF47            *(volatile unsigned int*)(ICU_BASE+0x00BC)
#define ICU_INT_CONF48            *(volatile unsigned int*)(ICU_BASE+0x00C0)
#define ICU_INT_CONF49            *(volatile unsigned int*)(ICU_BASE+0x00C4)
#define ICU_INT_CONF50            *(volatile unsigned int*)(ICU_BASE+0x00C8)
#define ICU_INT_CONF51            *(volatile unsigned int*)(ICU_BASE+0x00CC)
#define ICU_INT_CONF52            *(volatile unsigned int*)(ICU_BASE+0x00D0)
#define ICU_INT_CONF53            *(volatile unsigned int*)(ICU_BASE+0x00D4)
#define ICU_INT_CONF54            *(volatile unsigned int*)(ICU_BASE+0x00D8)
#define ICU_INT_CONF55            *(volatile unsigned int*)(ICU_BASE+0x00DC)
#define ICU_INT_CONF56            *(volatile unsigned int*)(ICU_BASE+0x00E0)
#define ICU_INT_CONF57            *(volatile unsigned int*)(ICU_BASE+0x00E4)
#define ICU_INT_CONF58            *(volatile unsigned int*)(ICU_BASE+0x00E8)
#define ICU_INT_CONF59            *(volatile unsigned int*)(ICU_BASE+0x00EC)
#define ICU_INT_CONF60            *(volatile unsigned int*)(ICU_BASE+0x00F0)
#define ICU_INT_CONF61            *(volatile unsigned int*)(ICU_BASE+0x00F4)
#define ICU_INT_CONF62            *(volatile unsigned int*)(ICU_BASE+0x00F8)
#define ICU_INT_CONF63            *(volatile unsigned int*)(ICU_BASE+0x00FC)
#define ICU_FIQ_SEL_INT_NUM       *(volatile unsigned int*)(ICU_BASE+0x0108)
#define ICU_IRQ_SEL_INT_NUM       *(volatile unsigned int*)(ICU_BASE+0x010C)
#define ICU_GBL_IRQ_MSK           *(volatile unsigned int*)(ICU_BASE+0x0114)
#define ICU_DMA_INT_MSK           *(volatile unsigned int*)(ICU_BASE+0x011C)
#define ICU_DMA_INT_STATUS        *(volatile unsigned int*)(ICU_BASE+0x0124)
#define ICU_INT_STATUS_0          *(volatile unsigned int*)(ICU_BASE+0x0128)
#define ICU_INT_STATUS_1          *(volatile unsigned int*)(ICU_BASE+0x012C)
#define ICU_DDR_ARM_L2_INT_MSK    *(volatile unsigned int*)(ICU_BASE+0x0130)
#define ICU_DDR_ARM_L2_INT_STATUS *(volatile unsigned int*)(ICU_BASE+0x0134)


#define MFP_88_CFG             *(volatile unsigned int*)(0xD401E160)
#define MFP_89_CFG             *(volatile unsigned int*)(0xD401E164)
//#define APBC_UART2_CLK_RST  *(volatile unsigned int*)(0xD4015004)

#define MFPR_96                *(volatile unsigned int*)(0xD401E180)
#define MFPR_88                *(volatile unsigned int*)(0xD401E160) // MFP_88 UART2_RXD
#define MFPR_89                *(volatile unsigned int*)(0xD401E164) // MFP_89 UART2_TXD

#define PWM3_BASE               (0xD401A800)
#define APBC_PWM3_CLK_RST      *(volatile unsigned int*)(0xD4015000 + 0x0014)
#define PWM3_CR                *(volatile unsigned int*)(PWM3_BASE  + 0x0000)
#define PWM3_DCR               *(volatile unsigned int*)(PWM3_BASE  + 0x0004)
#define PWM3_PCR               *(volatile unsigned int*)(PWM3_BASE  + 0x0008)

/**
 * @brief         PWM3Init
 * @param[in]     void
 * @param[in,out] void
 * @return        void
 */

void PWM3Init(void)
{
    /*
     * T clock input = 13MHz (13Khz) / (Prescale + 1)
     * T output = T clock input (PWMx_PCR + 1)
     *
     */
    // ALT1 select
    MFPR_96 = 0x01;
    APBC_PWM3_CLK_RST = 0x07;
    APBC_PWM3_CLK_RST = 0x03;
    // APBC_PWM3_CLK_RST |= (1<<4); 13KHz
    PWM3_CR = 0x0; // Prescale = 1
    PWM3_DCR = 5;
    PWM3_PCR = 9;
}

/**
 * @brief         Timer1Init
 * @param[in]     void
 * @param[in,out] void
 * @return        void
 */

void Timer1Init(void)
{
    // ICU configuration
    ICU_INT_CONF13 = 0x5f; // Route to IRQ, level = 15

    // Global mask interrupt
    ICU_GBL_IRQ_MSK = 0;

    // Enable clock input 32KHz
    APBC_TIMERS_CLK_RST = 0x17;
    APBC_TIMERS_CLK_RST = 0x13;

    // Fast clock select
    TMR_CCR = 0;
    // Clear interrupt
    TMR_ICR1 = 0x01;
	// allow matche register 0 to assert interrupts
	TMR_IER1 = 0x01;
    // Reset counter
    TMR_CR1 = 0;
    // Setup match 0 register 10Hz
    TMR_T1_M0 = 32000/10;
    // Preload register
    TMR_PLVR1 = 0;
    // Enable preload with comparator 0
    TMR_PLCR1 = 1;
    // Enable counter timer 1
    TMR_CER = 1;

}

/**
 * @brief         UartInitPins
 * @param[in]     void
 * @param[in,out] void
 * @return        void
 */

void UartInitPins(void)
{
    MFP_88_CFG = 0x48C2;
    MFP_89_CFG = 0x48C2;
    *(VUINT_T *)APBC_UART2_CLK_RST = 0x13;
}

/**
 * @brief         DelayMs
 * @param[in]     void
 * @param[in,out] void
 * @return        void
 */

void DelayMs(unsigned int n)
{
    unsigned int i, j;

    for (i = 0; i < n; i++)
        for (j=0; j < 1000; j++);

}

#define led_off()  *(volatile unsigned int *) 0xD4019020 = 0x100000
#define led_on()   *(volatile unsigned int *) 0xD401902C = 0x100000

/**
 * @brief         LedConfig
 * @param[in]     void
 * @param[in,out] void
 * @return        void
 */

void LedConfig(void)
{
    unsigned int *reg1 = (volatile unsigned int *) 0xD4015008;
    unsigned int *reg2 = (volatile unsigned int *) 0xD401905C;
    *reg1 = 0x7;
    *reg1 = 0x3;
    *reg2 = 0x100000;
    led_off();
}

/**
 * @brief         BlinkLed
 * @param[in]     void
 * @param[in,out] void
 * @return        void
 */

void BlinkLed(void)
{

    unsigned int *reg3 = (volatile unsigned int *) 0xD4019020;
    unsigned int *reg4 = (volatile unsigned int *) 0xD401902C;

    while(1)
    {
        *reg3 = 0x100000;
         DelayMs(100);
        *reg4 = 0x100000;
         DelayMs(100);
    }

}

/**
 * @brief         main
 * @param[in]     void
 * @param[in,out] void
 * @return        void
 */

void main (void)
{
    unsigned int Error;

    SetOperationMode();

    InitializeFFUART(115200);
    LedConfig();

    printf("BAMBOO REVA BASE PROGRAM\r\n");


#if 1
    Timer1Init();
#else
    BlinkLed();
#endif

    while(1);

}


/**
 * @brief         IrqHandler
 * @param[in]     void
 * @param[in,out] void
 * @return        void
 */

 void IrqHandler (void)
{
    static int on = 0;

    /* Clear interrupt */
    TMR_ICR1 = 0x01;

    if (on)
    {
        led_off();
        on = 0;
    }
    else
    {
        led_on();
        on = 1;
    }

}
