#include "PlatformConfig.h"
#include "uart_regs.h"
#include "predefines.h"
#include "PMUA.h"
#include "PMUM.h"
#include "APBC.h"

CS_REGISTER_PAIR_S uart_pins[]=
{
  		{ (int *) (APPS_PAD_BASE | 0x01AC), 0x4881, 0x0 },	// MFP_107 UART1_TXD
  		{ (int *) (APPS_PAD_BASE | 0x01B0), 0x4881, 0x0 },	// MFP_108 UART1_RXD
  		{ 0x0, 0x0, 0x0}                                    // termination
};

CS_REGISTER_PAIR_S alt_uart_pins[]=
{
  		{(int *) (APPS_PAD_BASE | 0x0160), 0x4882, 0x0},	// MFP_88 UART2_RXD
  		{(int *) (APPS_PAD_BASE | 0x0164), 0x4882, 0x0},	// MFP_89 UART2_TXD
  		{0x0, 0x0, 0x0}                                     //termination
};

CS_REGISTER_PAIR_S spi_pins[]=
{
  		(int *) (APPS_PAD_BASE | 0x01AC), 0x884, 0x0,	//SSP2 RXDIN  (GPIO 107)
  		(int *) (APPS_PAD_BASE | 0x01B0), 0x884, 0x0,	//SSP2 TXDIN  (GPIO 108)
  		(int *) (APPS_PAD_BASE | 0x01B8), 0x880, 0x0,	//SSP2 CS	  (GPIO 110)
		(int *) (APPS_PAD_BASE | 0x01BC), 0x884, 0x0,	//SSP_SCLK	  (GPIO 111)
		(int *) (APPS_PAD_BASE | 0x01C0), 0x880, 0x0,	//~SSP2_FRM	  (GPIO 112)
  		0x0,0x0,0x0 //termination
};


static void ConfigRegSetup( P_CS_REGISTER_PAIR_S regPtr)
{
    UINT32_T tmp;

    UINT32_T addr;
    UINT32_T value;

	while(regPtr->registerAddr != 0x0)
    {
	  tmp = *(regPtr->registerAddr);
	  tmp &= 0x000003F8;

       addr = regPtr->registerAddr;
       value = regPtr->regValue | tmp;

      *(regPtr->registerAddr) = regPtr->regValue | tmp;
	  tmp = *(regPtr->registerAddr);  // ensure write complete

      regPtr++;
    }
}

UINT_T PlatformUARTConfig(void)
{

   ConfigRegSetup(uart_pins);
   *(VUINT_T *)APBC_UART1_CLK_RST = (BIT4 | BIT1 | BIT0);
   return (NoError);
}

UINT_T PlatformAltUARTConfig(void)
{
   	ConfigRegSetup(alt_uart_pins);
   	*(VUINT_T *)APBC_UART2_CLK_RST = (BIT4 | BIT1 | BIT0);
	return (NoError);
}

void InitUartPort(void)
{
    UINT_T Retval = NoError;

#ifdef USE_UART1
    Retval = PlatformUARTConfig();
#else /* UART2 */
    Retval = PlatformAltUARTConfig();
#endif

    if(Retval != NoError)
    {
        while(1);
    }
}

//-----------------------------------------------------------------------
// Setting up SSP for SPI Flash
//
//
//-----------------------------------------------------------------------

void ChipSelectSPI( void )
{
	//need to turn on both 52 and 6.5 MHz clocks
   	*(volatile unsigned int *)PMUM_ACGR |= PMUM_ACGR_AP_52M | PMUM_ACGR_AP_6P5M;

	//enabled SSP2 clock, then take out of reset
  	*(APBC_SSP2_CLK_RST) = 0x7;
  	*(APBC_SSP2_CLK_RST) = 0x3;
	//bring GPIO unit OOR
	reg_bit_set((APBC_BASE | 0x8), 0x7);
	reg_bit_clr((APBC_BASE | 0x8), 0x4);

	ConfigRegSetup(spi_pins);
	return;
}

