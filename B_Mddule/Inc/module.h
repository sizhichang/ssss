#ifndef __MODULE_H__
#define __MODULE_H__


#include "tool.h"

  
void Process_Command_Cache(void);


void sim_initial(void);

bool sim_wait_answer(void);




#ifdef INTERRUPT_DMA_FREE   

void UART_RxIdleCallback(UART_HandleTypeDef * huart);
#else
void modu433_Time_Callback();
void module_Rec_Callback(void);  
#endif



#endif