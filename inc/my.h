#include "stm32f10x_map.h"
#include "stm32f10x_type.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"

#ifndef MY_H_
#define MY_H_

#define BB_S_REF 			0x20000000
#define BB_S_BASE  			0x22000000
#define BB_P_REF   			0x40000000
#define BB_P_BASE  			0x42000000
#define BitBand				*(volatile unsigned long*)
#define BB_S(address,bit) 	(BB_S_BASE + (((unsigned long)address) - BB_S_REF) * 32 + (bit) * 4)
#define BB_P(address,bit)	(BB_P_BASE + (((unsigned long)address) - BB_P_REF) * 32 + (bit) * 4)

#define PWR_DBP_Bit			BitBand BB_P(&PWR->CR, 8)
#define PWR_PVDO_Bit		BitBand BB_P(&PWR->CSR, 2)

#define RCC_CR_PLLRDY_Bit		BitBand BB_P(&RCC->CR,25)
#define RCC_CR_PLLON_Bit		BitBand BB_P(&RCC->CR,24)
#define RCC_CR_CSSON_Bit		BitBand BB_P(&RCC->CR,19)
#define RCC_CR_HSEBYP_Bit		BitBand BB_P(&RCC->CR,18)
#define RCC_CR_HSERDY_Bit		BitBand BB_P(&RCC->CR,17)
#define RCC_CR_HSEON_Bit		BitBand BB_P(&RCC->CR,16)
#define RCC_CR_HSIRDY_Bit		BitBand BB_P(&RCC->CR, 1)
#define RCC_CR_HSION_Bit		BitBand BB_P(&RCC->CR, 0)
#define RCC_CFGR_PLLMUL_Bit		18
#define RCC_CFGR_PLLSRC_Mask    ((u32)0x00010000)
#define RCC_CFGR_SWPLL			((u32)0x00000002)
#define RCC_CFGR_SWS_Mask       ((u32)0x0000000C)
#define RCC_CFGR_SWS_PLL		((u32)0x00000008)
#define RCC_CFGR_PPRE1			8
#define RCC_CFGR_PPRE1_DIV2		( ((u32)(0x00000004)) << RCC_CFGR_PPRE1 )

#endif
