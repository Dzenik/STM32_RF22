#ifndef NVIC_H_
#define NVIC_H_

#define NVIC_NO_GROUPS  NVIC_PriorityGroup_0

#define NVIC_I2C_PRI          5
#define NVIC_UART_PRI         6
#define NVIC_TRACE_TIM_PRI    7
#define NVIC_ADC_PRI          12
#define NVIC_RADIO_PRI        13

/**
 * Setup and initialize the NVIC
 */
void nvicInit(void);

#endif /* NVIC_H_ */
