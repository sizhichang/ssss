#include "module.h"
#include "app.h"
/*******************************************************/
extern uint16_t meter_rec_num; //记录中断中接收的数据的个数
extern uint8_t led_flag; //灯的状态
/*******************************************************/

static uint16_t rec_data_num=0;//ccdebug

uint8_t sim_rec_data[REC_LEN];

//static uint16_t sim_rec_data_num=0;

uint8_t sim_rec_flag=0;

/*******************************************************/
extern TIM_HandleTypeDef   htim6;
extern UART_HandleTypeDef  huart4;//接收上位机数据

extern Meter_Type          meter;
extern DMA_HandleTypeDef   hdma_uart4_rx;

extern DMA_HandleTypeDef   hdma_usart1_rx;
extern UART_HandleTypeDef  huart1;        //接收上位机数据

/*****************************************************/
/*****************************************************/

//串口开启空闲中断 控制寄存器设置为空闲中断，等待数据发送检测空闲

#ifdef INTERRUPT_DMA_FREE

void UART_RxIdleCallback(UART_HandleTypeDef * huart)
{

   if(huart==&huart4)
  {
    if(__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE))
    {
      __HAL_UART_CLEAR_IDLEFLAG(huart);
      HAL_UART_DMAStop(huart);
      HAL_UART_Abort(huart);
      rec_data_num  = hdma_uart4_rx.Instance->CNDTR;// 获取DMA中未传输的数据个数，NDTR寄存器分析见下面
      rec_data_num =  REC_LEN - rec_data_num;       //总计数减去未传输的数据个数，得到已经接收的数据个数    
      sim_rec_flag=1;    
    }
     
  }
   
  if(huart==&huart1)
  {      
    if(__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE))
    {
      __HAL_UART_CLEAR_IDLEFLAG(huart);
      HAL_UART_DMAStop(huart);

      meter_rec_num  = hdma_usart1_rx.Instance->CNDTR;// 获取DMA中未传输的数据个数，NDTR寄存器分析见下面
      meter_rec_num =  REC_LEN - meter_rec_num;       //总计数减去未传输的数据个数，得到已经接收的数据个数
      meter.rec_finish = true; 
      
    }
  }                                                   //ccdebug
   
}
#else

void module_Rec_Callback(void)
{
   htim6.Instance->CNT = 0; 
  if((htim6.Instance->CR1&1)==0)
  {
    HAL_TIM_Base_Start_IT(&htim6);   
  }
}
 
//定时器回调 藩属接受
 void modu433_Time_Callback()
 {
  
 }
#endif
 


/*******************************************************
//初始化 void sim_initial(void)
*******************************************************/

void sim_initial(void)
{  
#ifdef INTERRUPT_DMA_FREE
     __HAL_UART_ENABLE_IT(&huart4,UART_IT_IDLE);//开启空闲中断
    
      HAL_UART_Receive_DMA(&huart4,sim_rec_data ,REC_LEN);
#else

    HAL_UART_Receive_IT(&huart4, sim_rec_data , REC_LEN);       
#endif   
    ENABLE_INT();                                          //开全局中断
 }



//判断是否已经从wifi模块中获取一个数据包的数据头：包括 +IPD,1400: 到':'的数据。
uint8_t * JudgeHttpIntegrity(uint8_t * indata,uint16_t * headCount,uint16_t * msgCount,uint16_t receivedLength)
{
    uint16_t i = 0;
    uint16_t j = 0;
    uint8_t * databegin;
    uint16_t num = 0;
    bool hasFound = false;
    
    //找到连续的+IPD，
    for(i=0;i<receivedLength;i++)
    {
        if((indata[i]==0x68)&&(indata[i+5]==0x68))
        {
            hasFound = true; //找到数据
            break;
        }        
    }
    
    //如果没有找到数据，则返回指向0的指针。
    if(hasFound == false)
        return (uint8_t *)0;
       
    hasFound = false; //来寻找
    for(j=i;j<receivedLength;j++)
    {
      if(indata[j] == 0x16)
      {
          hasFound = true;
          break;
      }          
    }   
    //到这里找到了完整的头
    if(hasFound == false)    
        return (uint8_t *)0;
    
   * headCount=i;        
    num = j - i;           // 获取接收到的数据量字符。 “20”  data[i] 指向数字的第一个 ，data[j] 指向‘：’

    *msgCount= num;
    databegin = &indata[i];  
    return databegin;
}


//data 指向接收的数据  ,   index ，+ 相对的偏移量
void Move_To_Command_Cache(uint8_t * data,uint16_t rec_data_num,uint8_t index)
{    
//   uint16_t  i = 0;    
//    bool flag = (sim_cache.end_index+1)%CACHE_SIZE!=sim_cache.start_index ? true:false;
//    
//    if(flag)
//    {    
//       for(i =0;i<rec_data_num;i++)
//        {
//           sim_cache.cache[sim_cache.end_index][i] = data[index+i];   
//        }
//        sim_cache.rece_data_num[sim_cache.end_index]=rec_data_num;
//        rec_data_num=0;          
//        sim_cache.end_index = (sim_cache.end_index+1)%CACHE_SIZE;
//    }          
}

void Deinit_mode()
{
    uint32_t ticket_record,ticketing;
    ticketing=HAL_GetTick();//记录当下时间
    uint8_t flag=FALSE;
 
  if(((HAL_GPIO_ReadPin(MODE_CHICE_PORT,MODE_ERASURE)==GPIO_PIN_RESET))&&(flag==FALSE))
  {
      ticket_record=HAL_GetTick();//记录当下时间
      flag=TRUE;
  }
  
   if((HAL_GPIO_ReadPin(MODE_CHICE_PORT,MODE_ERASURE)==GPIO_PIN_RESET)&&flag)
   {
     if((ticketing- ticket_record)>5000)
     { 
        if(HAL_GPIO_ReadPin(MODE_CHICE_PORT,MODE_ERASURE)==GPIO_PIN_RESET)
        {
          FLASH_Erasure();
          led_flag=3;	
          for(;;);
        }
     }
   }
 
}


void Process_Command_Cache(void)
{
  while( sim_rec_flag==true)
  {     
     Process_Command_To_Meter(sim_rec_data,rec_data_num,3000);
     rec_data_num=0;
     sim_rec_flag=false;
     __HAL_UART_ENABLE_IT(&huart4,UART_IT_IDLE);//开启空闲中断   
     HAL_UART_Receive_DMA(&huart4,sim_rec_data ,REC_LEN);
  }
     Deinit_mode();
}

















