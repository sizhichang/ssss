#include "module.h"
#include "app.h"
/*******************************************************/
extern uint16_t meter_rec_num; //��¼�ж��н��յ����ݵĸ���
extern uint8_t led_flag; //�Ƶ�״̬
/*******************************************************/

static uint16_t rec_data_num=0;//ccdebug

uint8_t sim_rec_data[REC_LEN];

//static uint16_t sim_rec_data_num=0;

uint8_t sim_rec_flag=0;

/*******************************************************/
extern TIM_HandleTypeDef   htim6;
extern UART_HandleTypeDef  huart4;//������λ������

extern Meter_Type          meter;
extern DMA_HandleTypeDef   hdma_uart4_rx;

extern DMA_HandleTypeDef   hdma_usart1_rx;
extern UART_HandleTypeDef  huart1;        //������λ������

/*****************************************************/
/*****************************************************/

//���ڿ��������ж� ���ƼĴ�������Ϊ�����жϣ��ȴ����ݷ��ͼ�����

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
      rec_data_num  = hdma_uart4_rx.Instance->CNDTR;// ��ȡDMA��δ��������ݸ�����NDTR�Ĵ�������������
      rec_data_num =  REC_LEN - rec_data_num;       //�ܼ�����ȥδ��������ݸ������õ��Ѿ����յ����ݸ���    
      sim_rec_flag=1;    
    }
     
  }
   
  if(huart==&huart1)
  {      
    if(__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE))
    {
      __HAL_UART_CLEAR_IDLEFLAG(huart);
      HAL_UART_DMAStop(huart);

      meter_rec_num  = hdma_usart1_rx.Instance->CNDTR;// ��ȡDMA��δ��������ݸ�����NDTR�Ĵ�������������
      meter_rec_num =  REC_LEN - meter_rec_num;       //�ܼ�����ȥδ��������ݸ������õ��Ѿ����յ����ݸ���
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
 
//��ʱ���ص� ��������
 void modu433_Time_Callback()
 {
  
 }
#endif
 


/*******************************************************
//��ʼ�� void sim_initial(void)
*******************************************************/

void sim_initial(void)
{  
#ifdef INTERRUPT_DMA_FREE
     __HAL_UART_ENABLE_IT(&huart4,UART_IT_IDLE);//���������ж�
    
      HAL_UART_Receive_DMA(&huart4,sim_rec_data ,REC_LEN);
#else

    HAL_UART_Receive_IT(&huart4, sim_rec_data , REC_LEN);       
#endif   
    ENABLE_INT();                                          //��ȫ���ж�
 }



//�ж��Ƿ��Ѿ���wifiģ���л�ȡһ�����ݰ�������ͷ������ +IPD,1400: ��':'�����ݡ�
uint8_t * JudgeHttpIntegrity(uint8_t * indata,uint16_t * headCount,uint16_t * msgCount,uint16_t receivedLength)
{
    uint16_t i = 0;
    uint16_t j = 0;
    uint8_t * databegin;
    uint16_t num = 0;
    bool hasFound = false;
    
    //�ҵ�������+IPD��
    for(i=0;i<receivedLength;i++)
    {
        if((indata[i]==0x68)&&(indata[i+5]==0x68))
        {
            hasFound = true; //�ҵ�����
            break;
        }        
    }
    
    //���û���ҵ����ݣ��򷵻�ָ��0��ָ�롣
    if(hasFound == false)
        return (uint8_t *)0;
       
    hasFound = false; //��Ѱ��
    for(j=i;j<receivedLength;j++)
    {
      if(indata[j] == 0x16)
      {
          hasFound = true;
          break;
      }          
    }   
    //�������ҵ���������ͷ
    if(hasFound == false)    
        return (uint8_t *)0;
    
   * headCount=i;        
    num = j - i;           // ��ȡ���յ����������ַ��� ��20��  data[i] ָ�����ֵĵ�һ�� ��data[j] ָ�򡮣���

    *msgCount= num;
    databegin = &indata[i];  
    return databegin;
}


//data ָ����յ�����  ,   index ��+ ��Ե�ƫ����
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
    ticketing=HAL_GetTick();//��¼����ʱ��
    uint8_t flag=FALSE;
 
  if(((HAL_GPIO_ReadPin(MODE_CHICE_PORT,MODE_ERASURE)==GPIO_PIN_RESET))&&(flag==FALSE))
  {
      ticket_record=HAL_GetTick();//��¼����ʱ��
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
     __HAL_UART_ENABLE_IT(&huart4,UART_IT_IDLE);//���������ж�   
     HAL_UART_Receive_DMA(&huart4,sim_rec_data ,REC_LEN);
  }
     Deinit_mode();
}

















