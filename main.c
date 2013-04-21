#include "stm32f10x.h"
#include "rfm22.h"
#include "nvic.h"

static void vRCCInit(void);

int main(void)
{
	vRCCInit();
	nvicInit();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;

    //Cannot start the main oscillator: red/green LED of death...
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

    GPIO_Init(GPIOF, &GPIO_InitStructure);

	if(RF22init()){
		GPIO_SetBits(GPIOB, GPIO_Pin_6);
	}else{
		GPIO_SetBits(GPIOF, GPIO_Pin_7);
	}

    while(1)
    {
    }
}
/* ------------------------------------------------------------------------ */
static void vRCCInit(void)
{
	ErrorStatus HSEStartUpStatus;

	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
//	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	if (HSEStartUpStatus == SUCCESS)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		/* PCLK2 = HCLK/2 */
		RCC_PCLK2Config(RCC_HCLK_Div2);

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);

		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

		/* Enable PLL */
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while (RCC_GetSYSCLKSource() != 0x08);
	} else {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
		GPIO_InitTypeDef GPIO_InitStructure;

	    //Cannot start the main oscillator: red/green LED of death...
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

	    GPIO_Init(GPIOF, &GPIO_InitStructure);

	    GPIO_ResetBits(GPIOF, GPIO_Pin_5);
	    GPIO_ResetBits(GPIOF, GPIO_Pin_7);

	    //Cannot start xtal oscillator!
	    while(1);
	}
}
