#ifndef __GPIO_H__
#define __GPIO_H__

#include "PlatformConfig.h"

#define APB1_CLOCK_BASE       0xD4015000
#define APBC_GPIO_CLK_RST   *((volatile int *)(APB1_CLOCK_BASE + 0x0008))
#define APBC_AIB_CLK_RST    *((volatile int *)(APB1_CLOCK_BASE + 0x003C))

/* Common APB clock register bit definitions */
#define APBC_APBCLK     (1<<0)  /* APB Bus Clock Enable */
#define APBC_FNCLK      (1<<1)  /* Functional Clock Enable */
#define APBC_RST        (1<<2)  /* Reset Generation */
/* Functional Clock Selection Mask */
#define APBC_FNCLKSEL(x)        (((x) & 0xf) << 4)

/* Register Base Addresses */
#define ARMD1_DRAM_BASE		0xB0000000
#define ARMD1_FEC_BASE		0xC0800000
#define ARMD1_TIMER_BASE	0xD4014000
#define ARMD1_APBC1_BASE	0xD4015000
#define ARMD1_APBC2_BASE	0xD4015800
#define ARMD1_UART1_BASE	0xD4017000
#define ARMD1_UART2_BASE	0xD4018000
#define ARMD1_GPIO_BASE		0xD4019000
#define ARMD1_SSP1_BASE		0xD401B000
#define ARMD1_SSP2_BASE		0xD401C000
#define ARMD1_MFPR_BASE		0xD401E000
#define ARMD1_SSP3_BASE		0xD401F000
#define ARMD1_SSP4_BASE		0xD4020000
#define ARMD1_SSP5_BASE		0xD4021000
#define ARMD1_UART3_BASE	0xD4026000
#define ARMD1_MPMU_BASE		0xD4050000
#define ARMD1_USB_HOST_BASE	0xD4209000
#define ARMD1_APMU_BASE		0xD4282800
#define ARMD1_CPU_BASE		0xD4282C00

#define GPIO_HIGH		1
#define GPIO_LOW		0

#define GPIO_TO_REG(gp)		(gp >> 5)
#define GPIO_TO_BIT(gp)		(1 << (gp & 0x1F))
#define GPIO_VAL(gp, val)	((val >> (gp & 0x1F)) & 0x01)


#define MFP(_off, _pull, _pF, _drv, _dF, _edge, _eF, _afn, _aF) ( \
	/* bits 31..16 - MFP Register Offset */	(((_off) & 0xffff) << 16) | \
	/* bits 15..13 - Run Mode Pull State */	(((_pull) & 0x7) << 13) | \
	/* bit  12     - Unused */ \
	/* bits 11..10 - Driver Strength */	(((_drv) & 0x3) << 10) | \
	/* bit  09     - Pull State flag */	(((_pF) & 0x1) << 9) | \
	/* bit  08     - Drv-strength flag */	(((_dF) & 0x1) << 8) | \
	/* bit  07     - Edge-det flag */	(((_eF) & 0x1) << 7) | \
	/* bits 06..04 - Edge Detection */	(((_edge) & 0x7) << 4) | \
	/* bits 03..00 - Alt-fun flag */	(((_aF) & 0x1) << 3) | \
	/* bits Alternate-fun select */		((_afn) & 0x7))


#define MFP_OFFSET_MASK		     MFP(0xffff,    0,0,    0,0,     0,0,   0,0)
#define MFP_REG(x)		         MFP(x,         0,0,    0,0,     0,0,   0,0)
#define MFP_REG_GET_OFFSET(x)	((x & MFP_OFFSET_MASK) >> 16)

#define MFP_AF_FLAG		    MFP(0x0000,    0,0,    0,0,     0,0,   0,1)
#define MFP_DRIVE_FLAG		MFP(0x0000,    0,0,    0,1,     0,0,   0,0)
#define MFP_EDGE_FLAG		MFP(0x0000,    0,0,    0,0,     0,1,   0,0)
#define MFP_PULL_FLAG		MFP(0x0000,    0,1,    0,0,     0,0,   0,0)

#define MFP_AF0			    MFP(0x0000,    0,0,    0,0,     0,0,   0,1)
#define MFP_AF1			    MFP(0x0000,    0,0,    0,0,     0,0,   1,1)
#define MFP_AF2			    MFP(0x0000,    0,0,    0,0,     0,0,   2,1)
#define MFP_AF3			    MFP(0x0000,    0,0,    0,0,     0,0,   3,1)
#define MFP_AF4			    MFP(0x0000,    0,0,    0,0,     0,0,   4,1)
#define MFP_AF5			    MFP(0x0000,    0,0,    0,0,     0,0,   5,1)
#define MFP_AF6			    MFP(0x0000,    0,0,    0,0,     0,0,   6,1)
#define MFP_AF7			    MFP(0x0000,    0,0,    0,0,     0,0,   7,1)
#define MFP_AF_MASK		    MFP(0x0000,    0,0,    0,0,     0,0,   7,0)

#define MFP_LPM_EDGE_NONE	MFP(0x0000,    0,0,    0,0,     0,1,   0,0)
#define MFP_LPM_EDGE_RISE	MFP(0x0000,    0,0,    0,0,     1,1,   0,0)
#define MFP_LPM_EDGE_FALL	MFP(0x0000,    0,0,    0,0,     2,1,   0,0)
#define MFP_LPM_EDGE_BOTH	MFP(0x0000,    0,0,    0,0,     3,1,   0,0)
#define MFP_LPM_EDGE_MASK	MFP(0x0000,    0,0,    0,0,     3,0,   0,0)

#define MFP_DRIVE_VERY_SLOW	MFP(0x0000,    0,0,    0,1,     0,0,   0,0)
#define MFP_DRIVE_SLOW		MFP(0x0000,    0,0,    1,1,     0,0,   0,0)
#define MFP_DRIVE_MEDIUM	MFP(0x0000,    0,0,    2,1,     0,0,   0,0)
#define MFP_DRIVE_FAST		MFP(0x0000,    0,0,    3,1,     0,0,   0,0)
#define MFP_DRIVE_MASK		MFP(0x0000,    0,0,    3,0,     0,0,   0,0)

#define MFP_PULL_NONE		MFP(0x0000,    0,1,    0,0,     0,0,   0,0)
#define MFP_PULL_LOW		MFP(0x0000,    1,1,    0,0,     0,0,   0,0)
#define MFP_PULL_HIGH		MFP(0x0000,    2,1,    0,0,     0,0,   0,0)
#define MFP_PULL_BOTH		MFP(0x0000,    3,1,    0,0,     0,0,   0,0)
#define MFP_PULL_FLOAT		MFP(0x0000,    4,1,    0,0,     0,0,   0,0)
#define MFP_PULL_MASK		MFP(0x0000,    7,0,    0,0,     0,0,   0,0)

#define MV_MAX_GPIO	       128

#define writeb(b, addr) (void)((*(volatile unsigned char *) (addr)) = (b))
#define writew(b, addr) (void)((*(volatile unsigned short *) (addr)) = (b))
#define writel(b, addr) (void)((*(volatile unsigned int *) (addr)) = (b))

#define readb(a)        (*(volatile unsigned char  *)(a))
#define readw(a)        (*(volatile unsigned short *)(a))
#define readl(a)        (*(volatile unsigned int   *)(a))


/*
 *
 *	THE BASE ADDRESSES
 *
 */
#define	GPIO0_BASE	0xD4019000
#define	GPIO1_BASE	0xD4019004
#define	GPIO2_BASE	0xD4019008
#define	GPIO3_BASE	0xD4019100

/*
 *
 *	THE REGISTER DEFINES
 *
 */
#define	GPIO_PLR	(0x0000)	/* 32 bit	GPIO Pin-Level Register */
#define	GPIO_PDR	(0x000C)	/* 32 bit	GPIO Pin Direction Register */
#define	GPIO_PSR	(0x0018)	/* 32 bit	GPIO Pin Output Set Register */
#define	GPIO_PCR	(0x0024)	/* 32 bit	GPIO Pin Output Clear
										 *			Register
										 */
#define	GPIO_RER	(0x0030)	/* 32 bit	GPIO Rising-Edge Detect
										 *			Enable Register
										 */
#define	GPIO_FERX	(0x003C)	/* 32 bit	GPIO Falling-Edge Detect
										 *			Enable Register
										 */
#define	GPIO_EDR	(0x0048)	/* 32 bit	GPIO Edge Detect Status
										 *			Register
										 */
#define	GPIO_SDR	(0x0054)	/* 32 bit	Bit-wise Set of GPIO
										 *			Direction Register
										 */
#define	GPIO_CDR	(0x0060)	/* 32 bit	Bit-wise Clear of GPIO
										 *			Direction Register
										 */
#define	GPIO_SRERX	(0x006c)	/* 32 bit	Bit-wise Set of GPIO Rising
										 *			Edge Detect Enable
										 *			Register
										 */
#define	GPIO_CRERX	(0x0078)	/* 32 bit	Bit-wise Clear of GPIO
										 *			Rising Edge Detect Enable
										 *			Register
										 */
#define	GPIO_SFER	(0x0084)	/* 32 bit	Bit-wise Set of GPIO
										 *			Falling Edge Detect
										 *			Enable Register
										 */
#define	GPIO_CFER	(0x0090)	/* 32 bit	Bit-wise Clear of GPIO
										 *			Falling Edge Detect
										 *			Enable Register
										 */
#define	APMASK		(0x009c)	/* 32 bit	AP Bit-wise Mask of GPIO
										 *			Edge Detect Register
										 */
#define	CPMASK		(0x00a8)	/* 32 bit	CP Bit-wise Mask of GPIO
										 *			Edge Detect Register
										 */
/*
 *
 *	THE BIT DEFINES
 *
 */
/*	GPIO_PLR	0x0000	GPIO Pin-Level Register */
#define	GPIO_PLR_PLX_MSK		SHIFT0(0xffffffff)	/* PL{n} */
#define	GPIO_PLR_PLX_BASE		0

/*	GPIO_PDR	0x000C	GPIO Pin Direction Register */
#define	GPIO_PDR_PDX_MSK		SHIFT0(0xffffffff)	/* PD{n} */
#define	GPIO_PDR_PDX_BASE		0

/*	GPIO_PSR	0x0018	GPIO Pin Output Set Register */
#define	GPIO_PSR_PSX_MSK		SHIFT0(0xffffffff)	/* PS{n} */
#define	GPIO_PSR_PSX_BASE		0

/*	GPIO_PCR	0x0024	GPIO Pin Output Clear Register */
#define	GPIO_PCR_PCX_MSK		SHIFT0(0xffffffff)	/* PC{n} */
#define	GPIO_PCR_PCX_BASE		0

/*	GPIO_RER	0x0030	GPIO Rising-Edge Detect Enable Register */
#define	GPIO_RER_REX_MSK		SHIFT0(0xffffffff)	/* RE{n} */
#define	GPIO_RER_REX_BASE		0

/*	GPIO_FERx	0x003C	GPIO Falling-Edge Detect Enable Register */
#define	GPIO_FERX_FEX_MSK		SHIFT0(0xffffffff)	/* FE{n} */
#define	GPIO_FERX_FEX_BASE		0

/*	GPIO_EDR	0x0048	GPIO Edge Detect Status Register */
#define	GPIO_EDR_EDX_MSK		SHIFT0(0xffffffff)	/* ED{n} */
#define	GPIO_EDR_EDX_BASE		0

/*	GPIO_SDR	0x0054	Bit-wise Set of GPIO Direction Register */
#define	GPIO_SDR_PDX_MSK		SHIFT0(0xffffffff)	/* PD{n} */
#define	GPIO_SDR_PDX_BASE		0

/*	GPIO_CDR	0x0060	Bit-wise Clear of GPIO Direction Register */
#define	GPIO_CDR_PDX_MSK		SHIFT0(0xffffffff)	/* PD{n} */
#define	GPIO_CDR_PDX_BASE		0

/*	GPIO_SRERx	0x006c	Bit-wise Set of GPIO Rising Edge Detect Enable Register */
#define	GPIO_SRERX_PDX_MSK		SHIFT0(0xffffffff)	/* PD{n} */
#define	GPIO_SRERX_PDX_BASE		0

/*	GPIO_CRERx	0x0078	Bit-wise Clear of GPIO Rising Edge Detect Enable
 *						Register
 */
#define	GPIO_CRERX_PDX_MSK		SHIFT0(0xffffffff)	/* PD{n} */
#define	GPIO_CRERX_PDX_BASE		0

/*	GPIO_SFER	0x0084	Bit-wise Set of GPIO Falling Edge Detect Enable
 *						Register
 */
#define	GPIO_SFER_PDX_MSK		SHIFT0(0xffffffff)	/* PD{n} */
#define	GPIO_SFER_PDX_BASE		0

/*	GPIO_CFER	0x0090	Bit-wise Clear of GPIO Falling Edge Detect Enable
 *						Register
 */
#define	GPIO_CFER_PDX_MSK		SHIFT0(0xffffffff)	/* PD{n} */
#define	GPIO_CFER_PDX_BASE		0

/*	APMASK		0x009c	AP Bit-wise Mask of GPIO Edge Detect Register */
#define	APMASK_PDX_MSK		SHIFT0(0xffffffff)	/* PD{n} */
#define	APMASK_PDX_BASE		0

/*	CPMASK		0x00a8	CP Bit-wise Mask of GPIO Edge Detect Register */
#define	CPMASK_PDX_MSK		SHIFT0(0xffffffff)	/* PD{n} */
#define	CPMASK_PDX_BASE		0

struct gpio_reg {
	UINT32 gplr;	/* Pin Level Register - 0x0000 */
	UINT32 pad0[2];
	UINT32 gpdr;	/* Pin Direction Register - 0x000C */
	UINT32 pad1[2];
	UINT32 gpsr;	/* Pin Output Set Register - 0x0018 */
	UINT32 pad2[2];
	UINT32 gpcr;	/* Pin Output Clear Register - 0x0024 */
	UINT32 pad3[2];
	UINT32 grer;	/* Rising-Edge Detect Enable Register - 0x0030 */
	UINT32 pad4[2];
	UINT32 gfer;	/* Falling-Edge Detect Enable Register - 0x003C */
	UINT32 pad5[2];
	UINT32 gedr;	/* Edge Detect Status Register - 0x0048 */
	UINT32 pad6[2];
	UINT32 gsdr;	/* Bitwise Set of GPIO Direction Register - 0x0054 */
	UINT32 pad7[2];
	UINT32 gcdr;	/* Bitwise Clear of GPIO Direction Register - 0x0060 */
	UINT32 pad8[2];
	UINT32 gsrer;	/* Bitwise Set of Rising-Edge Detect Enable
			           Register - 0x006C */
	UINT32 pad9[2];
	UINT32 gcrer;	/* Bitwise Clear of Rising-Edge Detect Enable
			           Register - 0x0078 */
	UINT32 pad10[2];
	UINT32 gsfer;	/* Bitwise Set of Falling-Edge Detect Enable
			           Register - 0x0084 */
	UINT32 pad11[2];
	UINT32 gcfer;	/* Bitwise Clear of Falling-Edge Detect Enable
			           Register - 0x0090 */
	UINT32 pad12[2];
	UINT32 apmask;	/* Bitwise Mask of Edge Detect Register - 0x009C */
};

#endif /* __GPIO_H__ */
