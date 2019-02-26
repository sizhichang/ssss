#ifndef __MODULE_H__
#define __MODULE_H__


#include "tool.h"

  



//#define INTERRUPT_DMA_FREE  1 //∂®“ÂDMA÷–∂œ

#ifdef INTERRUPT_DMA_FREE       

#else
void Wifi_Time_Callback(void);
void Wifi_Rec_Callback(void);
#endif
void UART_RxIdleCallback(UART_HandleTypeDef * huart);
void process_cmd_tx_rx();
void init(void);
#endif