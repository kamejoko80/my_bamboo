#ifndef PLATFORMCONFIG_H_INCLUDED
#define PLATFORMCONFIG_H_INCLUDED

#include "Typedef.h"
#include "Errors.h"

#define APPS_PAD_BASE   0xd401e000
#define UARTCLK_L       14745600
#define UARTCLK_H       58500000

// Timers
#define APB_CU_ENABLE (*APB_CU_CR = 0x33)
#define TIMER_FREQ(A) (((A)*13) >> 2)

// SPI
#define spi_reg_bit_set(reg, val) reg_bit_set(reg, val)
#define spi_reg_bit_clr(reg, val) reg_bit_clr(reg, val)
#define APBC_SSP2_CLK_RST	(volatile unsigned int *)(APBC_BASE+0x820)
#define APBC_SSP5_CLK_RST	(volatile unsigned int *)(APBC_BASE+0x85C)
//force SSP_BASE_FOR_SPI to the correct SSPx peripheral base
#define SSP_BASE_FOR_SPI		0xD401C000
#define SSP_RX_DMA_DEVICE		DMAC_SSP_1_RX
#define SSP_TX_DMA_DEVICE		DMAC_SSP_1_TX

//
// Multifunction Padding Structures
//
typedef struct
{
  volatile int *    registerAddr;
  unsigned int      regValue;
  unsigned int      defaultValue;
}CS_REGISTER_PAIR_S, *P_CS_REGISTER_PAIR_S;

void InitUartPort(void);

#endif /* PLATFORMCONFIG_H_INCLUDED */
