#include "module.h"
/*******************************************************/

/*******************************************************/
uint16_t sim_rec_data_num = 0; //ccdebug
uint16_t rs485_rec_data_num = 0; //ccdebug
uint8_t sim_rec_data[300];
uint8_t rs485_rec_data[300];

uint8_t sim_rec_flag = 0;
uint8_t rs485_rec_flag;
/*******************************************************/
extern TIM_HandleTypeDef   htim6;
extern UART_HandleTypeDef  huart4;//������λ������
extern UART_HandleTypeDef  huart1;//������λ������

extern  DMA_HandleTypeDef  hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_usart1_rx;
/*****************************************************/
static void RS485_Send_Data( uint8_t* buf, uint8_t len );
static void module_send_data( uint8_t* buf, uint8_t len );
/*****************************************************/
//���ڿ��������ж� ���ƼĴ�������Ϊ�����жϣ��ȴ����ݷ��ͼ�����


void UART_RxIdleCallback( UART_HandleTypeDef* huart )
{
    if( huart == &huart1 )
    {
        if( __HAL_UART_GET_FLAG( huart, UART_FLAG_IDLE ) )
        {
            __HAL_UART_CLEAR_IDLEFLAG( huart );
            HAL_UART_DMAStop( huart );
            rs485_rec_data_num  = hdma_usart1_rx.Instance->CNDTR;// ��ȡDMA��δ��������ݸ�����NDTR�Ĵ�������������
            rs485_rec_data_num =  REC_LEN - rs485_rec_data_num;       //�ܼ�����ȥδ��������ݸ������õ��Ѿ����յ����ݸ���
            rs485_rec_flag = TRUE;
        }
    }
}
void UART4_RxIdleCallback( void )
{
#ifdef INTERRUPT_DMA_FREE
//    if( __HAL_UART_GET_FLAG( &huart4, UART_FLAG_IDLE ) )
//    {
        HAL_Delay( 15 ); //�ж�����������ʱ
        if( __HAL_UART_GET_FLAG( &huart4, UART_FLAG_IDLE ) )
        {
            __HAL_UART_CLEAR_IDLEFLAG( &huart4 );
            HAL_UART_DMAStop( &huart4 );
            sim_rec_data_num  = hdma_uart4_rx.Instance->CNDTR;// ��ȡDMA��δ��������ݸ�����NDTR�Ĵ�������������
            sim_rec_data_num =  REC_LEN - sim_rec_data_num;    //�ܼ�����ȥδ��������ݸ������õ��Ѿ����յ����ݸ���
            sim_rec_flag = TRUE;
        }
//    }
#endif
}
void Wifi_Rec_Callback()
{
    //�����ʱ������
    htim6.Instance->CNT = 0;
    if( ( htim6.Instance->CR1 & 1 ) == 0 )
    {
        HAL_TIM_Base_Start_IT( &htim6 );
    }
}


void Wifi_Time_Callback()
{
    HAL_TIM_Base_Stop_IT( &htim6 );
    htim6.Instance->CNT = 0;
    HAL_UART_AbortReceive( &huart4 );
    sim_rec_flag = TRUE;
}

void HAL_TIM_PeriodElapsedCallback( TIM_HandleTypeDef* htim )
{
    uint8_t static i = 0;
    if( htim == &htim6 )
    {
        i++;
        if( i % 2 == 0 ) //��Ϊ�ڶ�ʱ��ʹ��ʱ������жϣ�����Ҫ�ڵڶ��ν�����жϲ�˵�������������
        {
            Wifi_Time_Callback();
            i = 0;
        }
    }
}



void process_cmd_tx_rx( void )
{
    if( sim_rec_flag == 1 )
    {
        RS485_Send_Data( sim_rec_data, sim_rec_data_num );
        sim_rec_flag = 0;
#ifdef INTERRUPT_DMA_FREE
        __HAL_UART_ENABLE_IT( &huart4, UART_IT_IDLE ); //���������ж�
        HAL_UART_Receive_DMA( &huart4, sim_rec_data , 300 );
#else
        HAL_UART_Receive_IT( &huart4, sim_rec_data , 300 );
#endif
    }
    if( rs485_rec_flag == 1 )
    {
        module_send_data( rs485_rec_data, rs485_rec_data_num );
        rs485_rec_flag = 0;
        __HAL_UART_ENABLE_IT( &huart1, UART_IT_IDLE ); //���������ж�
        HAL_UART_Receive_DMA( &huart1, rs485_rec_data , 300 );
    }
}



void init( void )
{
    __HAL_UART_ENABLE_IT( &huart1, UART_IT_IDLE ); //���������ж�
    HAL_UART_Receive_DMA( &huart1, rs485_rec_data , 300 );
    __HAL_UART_ENABLE_IT( &huart4, UART_IT_IDLE ); //���������ж�
    HAL_UART_Receive_DMA( &huart4, sim_rec_data , 300 );
}



void RS485_Send_Data( uint8_t* buf, uint8_t len )
{
    uint8_t t;
    RS_485_TX();       //����Ϊ����ģʽ
    for( t = 0; t < len; t++ ) //ѭ����������
    {
        while( ( USART1->SR & 0X40 ) == 0 ); //�ȴ����ͽ���
        USART1->DR = buf[t];
    }
    while( ( USART1->SR & 0X40 ) == 0 ); //�ȴ����ͽ���
    RS_485_RX();       //      //����Ϊ����ģʽ
}

void module_send_data( uint8_t* buf, uint8_t len )
{
    uint8_t t;
    for( t = 0; t < len; t++ ) //ѭ����������
    {
        while( ( UART4->SR & 0X40 ) == 0 ); //�ȴ����ͽ���
        UART4->DR = buf[t];
    }
    while( ( UART4->SR & 0X40 ) == 0 ); //�ȴ����ͽ���
}








