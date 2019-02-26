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
extern UART_HandleTypeDef  huart4;//接收上位机数据
extern UART_HandleTypeDef  huart1;//接收上位机数据

extern  DMA_HandleTypeDef  hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_usart1_rx;
/*****************************************************/
static void RS485_Send_Data( uint8_t* buf, uint8_t len );
static void module_send_data( uint8_t* buf, uint8_t len );
/*****************************************************/
//串口开启空闲中断 控制寄存器设置为空闲中断，等待数据发送检测空闲


void UART_RxIdleCallback( UART_HandleTypeDef* huart )
{
    if( huart == &huart1 )
    {
        if( __HAL_UART_GET_FLAG( huart, UART_FLAG_IDLE ) )
        {
            __HAL_UART_CLEAR_IDLEFLAG( huart );
            HAL_UART_DMAStop( huart );
            rs485_rec_data_num  = hdma_usart1_rx.Instance->CNDTR;// 获取DMA中未传输的数据个数，NDTR寄存器分析见下面
            rs485_rec_data_num =  REC_LEN - rs485_rec_data_num;       //总计数减去未传输的数据个数，得到已经接收的数据个数
            rs485_rec_flag = TRUE;
        }
    }
}
void UART4_RxIdleCallback( void )
{
#ifdef INTERRUPT_DMA_FREE
//    if( __HAL_UART_GET_FLAG( &huart4, UART_FLAG_IDLE ) )
//    {
        HAL_Delay( 15 ); //中断中增加了延时
        if( __HAL_UART_GET_FLAG( &huart4, UART_FLAG_IDLE ) )
        {
            __HAL_UART_CLEAR_IDLEFLAG( &huart4 );
            HAL_UART_DMAStop( &huart4 );
            sim_rec_data_num  = hdma_uart4_rx.Instance->CNDTR;// 获取DMA中未传输的数据个数，NDTR寄存器分析见下面
            sim_rec_data_num =  REC_LEN - sim_rec_data_num;    //总计数减去未传输的数据个数，得到已经接收的数据个数
            sim_rec_flag = TRUE;
        }
//    }
#endif
}
void Wifi_Rec_Callback()
{
    //清除计时器计数
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
        if( i % 2 == 0 ) //因为在定时器使能时会进入中断，所以要在第二次进入该中断才说明发送数据完成
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
        __HAL_UART_ENABLE_IT( &huart4, UART_IT_IDLE ); //开启空闲中断
        HAL_UART_Receive_DMA( &huart4, sim_rec_data , 300 );
#else
        HAL_UART_Receive_IT( &huart4, sim_rec_data , 300 );
#endif
    }
    if( rs485_rec_flag == 1 )
    {
        module_send_data( rs485_rec_data, rs485_rec_data_num );
        rs485_rec_flag = 0;
        __HAL_UART_ENABLE_IT( &huart1, UART_IT_IDLE ); //开启空闲中断
        HAL_UART_Receive_DMA( &huart1, rs485_rec_data , 300 );
    }
}



void init( void )
{
    __HAL_UART_ENABLE_IT( &huart1, UART_IT_IDLE ); //开启空闲中断
    HAL_UART_Receive_DMA( &huart1, rs485_rec_data , 300 );
    __HAL_UART_ENABLE_IT( &huart4, UART_IT_IDLE ); //开启空闲中断
    HAL_UART_Receive_DMA( &huart4, sim_rec_data , 300 );
}



void RS485_Send_Data( uint8_t* buf, uint8_t len )
{
    uint8_t t;
    RS_485_TX();       //设置为发送模式
    for( t = 0; t < len; t++ ) //循环发送数据
    {
        while( ( USART1->SR & 0X40 ) == 0 ); //等待发送结束
        USART1->DR = buf[t];
    }
    while( ( USART1->SR & 0X40 ) == 0 ); //等待发送结束
    RS_485_RX();       //      //设置为接收模式
}

void module_send_data( uint8_t* buf, uint8_t len )
{
    uint8_t t;
    for( t = 0; t < len; t++ ) //循环发送数据
    {
        while( ( UART4->SR & 0X40 ) == 0 ); //等待发送结束
        UART4->DR = buf[t];
    }
    while( ( UART4->SR & 0X40 ) == 0 ); //等待发送结束
}








