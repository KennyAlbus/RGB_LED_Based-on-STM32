/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include <stdio.h>

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}
#if 0
///**
//  * @brief  This function handles Hard Fault exception.
//  * @param  None
//  * @retval None
//  */
//void HardFault_Handler(void)
//{
//  /* Go to infinite loop when Hard Fault exception occurs */
//  while (1)
//  {
//  }
//}
#else
/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
__asm void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
	IMPORT HardFault_Handler_C
	TST    lr, #4
	ITE    EQ
	MRSEQ  r0, MSP
	MRSNE  r0, PSP
	B      HardFault_Handler_C

}

void HardFault_Handler_C(uint32_t *hardfault_args)
{
  uint32_t stacked_r0 = hardfault_args[0];
	uint32_t stacked_r1 = hardfault_args[1];
	uint32_t stacked_r2 = hardfault_args[2];
	uint32_t stacked_r3 = hardfault_args[3];
	uint32_t stacked_r12 = hardfault_args[4];
	uint32_t stacked_lr = hardfault_args[5];
	uint32_t stacked_pc = hardfault_args[6];
	uint32_t stacked_psr = hardfault_args[7];
	
	char msg[160];
	snprintf(msg,sizeof(msg),
		"Hard_Fault!\r\n"
	  "R0 = 0x%08X\r\n"
	  "R1 = 0x%08X\r\n"
	  "R2 = 0x%08X\r\n"
	  "R3 = 0x%08X\r\n"
	  "R12 = 0x%08X\r\n"
	  "LR = 0x%08X\r\n"
	  "PC = 0x%08X\r\n"
	  "PSR = 0x%08X\r\n",
	stacked_r0,stacked_r1,stacked_r2,stacked_r3,
	stacked_r12,stacked_lr,stacked_pc,stacked_psr);
	printf("%s",msg);
	while(1);
}
#endif
/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
__weak void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
