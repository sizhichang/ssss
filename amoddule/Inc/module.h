#ifndef __MODULE_H__
#define __MODULE_H__


#include "stm32f1xx_hal.h"
  

#define REC_LEN       300 //接收长度

#define RS485_EN_GPIO_Port GPIOB 
#define RS485_EN_Pin GPIO_PIN_5  

#define LED1_PORT   GPIOB 
#define LED1_PIN    GPIO_PIN_1 

#define RS_485_RX() HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_SET)
#define RS_485_TX() HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_RESET)

#define TRUE   1
#define FALSE  0

#define INTERRUPT_DMA_FREE  1 //定义DMA中断

#ifdef INTERRUPT_DMA_FREE       

#else
void Wifi_Time_Callback(void);
void Wifi_Rec_Callback(void);
#endif
void UART_RxIdleCallback(UART_HandleTypeDef * huart);
void UART4_RxIdleCallback(void);
void process_cmd_tx_rx();
void init(void);
#endif