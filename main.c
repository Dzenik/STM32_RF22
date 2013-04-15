#include "stm32f10x.h"
#include "rfm22.h"

static void vRCC_Configuration(void);
static void vGPIO_Configuration(void);
static void vNVIC_Configuration(void);

int main(void)
{
	vRCC_Configuration();
	vNVIC_Configuration();
	vGPIO_Configuration();


    while(1)
    {
    }
}
/* ------------------------------------------------------------------------ */
static void vRCC_Configuration(void)
{
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

	/* Enable Prefetch Buffer */
//	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

	/* Flash 2 wait state */
//	FLASH_SetLatency(FLASH_Latency_2);

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

	/* Enable peripheral clocks --------------------------------------------------*/
	/* GPIOA, GPIOB and SPI1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
						 RCC_APB2Periph_SPI1, ENABLE);

	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
}

static void vGPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure SPI2 pins: SCK, MISO and MOSI ---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void vNVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
}


//static void vGPIOInitialise( void )
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	// запускаем тактирование GPIO порта F
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
//
//	// Устанавливаем пины PF6, PF7, PF8, PF9 как выходные
//	// использовать будем PF6 и PF7, но уж проинициализируем все четыре LED
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8	| GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOF, &GPIO_InitStructure);
//}
