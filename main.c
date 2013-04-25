#include <stdio.h>
#include "stm32f10x.h"
#include "rfm22.h"
#include "nvic.h"
#include "ili9320.h"

static __IO uint32_t TimingDelay;

static void vRCCInit(void);
void Delay(__IO uint32_t nCount);
void LED_Toggle(uint16_t led);


int main(void)
{
	uint8_t data[RF22_MAX_MESSAGE_LEN] = {"Hello World!"};
	uint8_t len = sizeof(data);
	uint8_t rssi = 0;
	char device_str1[20], device_str2[20], device_str3[20];
	uint32_t tCount = 0;
	uint8_t adcVal = 0;
	float tmpf;

	vRCCInit();

	if (SysTick_Config(SystemCoreClock / 1000))
	{
		while(1);
	}

	nvicInit();

	/* Initialize the LCD */
	ili9320_Initializtion();

	/* Clear the LCD */
	ili9320_Clear(Black);
	ili9320_DisplayStringLine(Line0, "                Warning                 ",White,Blue);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;

    //Cannot start the main oscillator: red/green LED of death...
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

    GPIO_Init(GPIOF, &GPIO_InitStructure);

	if(!RF22init()){
		LED_Toggle(GPIO_Pin_6);
		Delay(1000);
	}

    while(1)
    {
    	data[0] = 1;
    	data[1] = rssi;
    	tCount = 32500;

    	send(data, sizeof(data));
    	waitPacketSent();

    	LED_Toggle(GPIO_Pin_8);
    	while(--tCount != 0)
    	{
			if(recv(data, &len))
			{
				data[1] = rssi;
				send(data, sizeof(data));
				//waitPacketSent();
				LED_Toggle(GPIO_Pin_7);
			}
			rssi = rssiRead();
    	}

    	adcVal = temperatureRead(0x00, 0);
    	tmpf = adcVal * 0.1953125;
    	sprintf(device_str1,"RSSI R: %04d",data[0]);
		sprintf(device_str2,"RSSI L: %04d",data[1]);
		sprintf(device_str3,"Temp: %.2f", tmpf);
		ili9320_DisplayStringLine(Line1, device_str1, White, Black);
		ili9320_DisplayStringLine(Line2, device_str2, White, Black);
		ili9320_DisplayStringLine(Line3, device_str3, White, Black);
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
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

	    GPIO_Init(GPIOF, &GPIO_InitStructure);

        //Cannot start xtal oscillator!
	    while(1)
	    {
			LED_Toggle(GPIO_Pin_10);
			uint32_t i = 0x5FFFF;
			while(--i != 0x00);
	    }
	}
}

void LED_Toggle(uint16_t led)
{
  GPIO_WriteBit(GPIOF, led, (BitAction)!GPIO_ReadOutputDataBit(GPIOF, led));
}

void Delay(__IO uint32_t nCount)
{
	TimingDelay = nCount;
	while(TimingDelay != 0);
}

void SysTick_Handler(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}
