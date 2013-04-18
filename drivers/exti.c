#include <stdbool.h>

#include "stm32f10x_conf.h"
#include "stm32f10x_exti.h"

#include "nvic.h"
#include "rfm22.h"

#define RADIO_GPIO_IRQ_LINE EXTI_Line9

static bool isInit;

/* Interruption initialisation */
void extiInit()
{
  if (isInit)
    return;

  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_RADIO_PRI;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  isInit = true;
}

bool extiTest(void)
{
  return isInit;
}

void extiInterruptHandler(void)
{
  if (EXTI_GetITStatus(RADIO_GPIO_IRQ_LINE)==SET)
  {
    rfIsr();
    EXTI_ClearITPendingBit(RADIO_GPIO_IRQ_LINE);
  }
}
