#include "stm32f10x_conf.h"
#include "exti.h"


void nvicInit(void)
{
#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
}

#ifdef NVIC_NOT_USED_BY_FREERTOS
/**
 * @brief  This function handles SysTick Handler.
 */
void SysTick_Handler(void)
{
}

/**
  * @brief  This function handles SVCall exception.
  */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles PendSV_Handler exception.
 */
void PendSV_Handler(void)
{
}
#endif

/**
  * @brief  This function handles NMI exception.
  */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 */
void HardFault_Handler(void)
{
  //http://www.st.com/mcu/forums-cat-6778-23.html
  //****************************************************
  //To test this application, you can use this snippet anywhere:
  // //Let's crash the MCU!
  // asm (" MOVS r0, #1 \n"
  // " LDM r0,{r1-r2} \n"
  // " BX LR; \n");
  asm( "TST LR, #4 \n"
  "ITE EQ \n"
  "MRSEQ R0, MSP \n"
  "MRSNE R0, PSP \n"
  "B printHardFault");
}

void printHardFault(uint32_t* hardfaultArgs)
{
  unsigned int stacked_r0;
  unsigned int stacked_r1;
  unsigned int stacked_r2;
  unsigned int stacked_r3;
  unsigned int stacked_r12;
  unsigned int stacked_lr;
  unsigned int stacked_pc;
  unsigned int stacked_psr;

  stacked_r0 = ((unsigned long) hardfaultArgs[0]);
  stacked_r1 = ((unsigned long) hardfaultArgs[1]);
  stacked_r2 = ((unsigned long) hardfaultArgs[2]);
  stacked_r3 = ((unsigned long) hardfaultArgs[3]);

  stacked_r12 = ((unsigned long) hardfaultArgs[4]);
  stacked_lr = ((unsigned long) hardfaultArgs[5]);
  stacked_pc = ((unsigned long) hardfaultArgs[6]);
  stacked_psr = ((unsigned long) hardfaultArgs[7]);

//  uartPrintf("[Hard fault handler]\n");
//  uartPrintf("R0 = %x\n", stacked_r0);
//  uartPrintf("R1 = %x\n", stacked_r1);
//  uartPrintf("R2 = %x\n", stacked_r2);
//  uartPrintf("R3 = %x\n", stacked_r3);
//  uartPrintf("R12 = %x\n", stacked_r12);
//  uartPrintf("LR = %x\n", stacked_lr);
//  uartPrintf("PC = %x\n", stacked_pc);
//  uartPrintf("PSR = %x\n", stacked_psr);
//  uartPrintf("BFAR = %x\n", (*((volatile unsigned int *)(0xE000ED38))));
//  uartPrintf("CFSR = %x\n", (*((volatile unsigned int *)(0xE000ED28))));
//  uartPrintf("HFSR = %x\n", (*((volatile unsigned int *)(0xE000ED2C))));
//  uartPrintf("DFSR = %x\n", (*((volatile unsigned int *)(0xE000ED30))));
//  uartPrintf("AFSR = %x\n", (*((volatile unsigned int *)(0xE000ED3C))));

  while (1)
  {}
}
/**
 * @brief  This function handles Memory Manage exception.
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
 */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
 * @brief  This function handles Debug Monitor exception.
 */
void DebugMon_Handler(void)
{
}

void DMA1_Channel1_IRQHandler(void)
{
//  adcInterruptHandler();
}

void DMA1_Channel2_IRQHandler(void)
{
#if defined(UART_OUTPUT_TRACE_DATA) || defined(ADC_OUTPUT_RAW_DATA)
  uartDmaIsr();
#endif
}

void DMA1_Channel4_IRQHandler(void)
{
//  i2cDmaInterruptHandlerI2c2();
}

void DMA1_Channel5_IRQHandler(void)
{
//  i2cDmaInterruptHandlerI2c2();
}

void DMA1_Channel6_IRQHandler(void)
{
//  i2cDmaInterruptHandlerI2c1();
}

void DMA1_Channel7_IRQHandler(void)
{
//  i2cDmaInterruptHandlerI2c1();
}


void EXTI9_5_IRQHandler(void)
{
  extiInterruptHandler();
}

void USART3_IRQHandler(void)
{
//  uartIsr();
}

void TIM1_UP_IRQHandler(void)
{
//  extern uint32_t traceTickCount;

//  TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//  traceTickCount++;
}

void I2C1_EV_IRQHandler(void)
{
//  i2cInterruptHandlerI2c1();
}

void I2C1_ER_IRQHandler(void)
{
//  i2cErrorInterruptHandlerI2c1();
}

void I2C2_EV_IRQHandler(void)
{

}

void I2C2_ER_IRQHandler(void)
{
//  I2C_ClearFlag(I2C2, 0x1000FFFF);
}
