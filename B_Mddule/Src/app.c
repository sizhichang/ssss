#include "app.h"
#include "flash.h"

/********************************************/
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim6;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef  huart4;
static void set_485buadrate( u32 buadrate );
/**********************************************/

uint16_t meter_rec_num; //记录中断中接收的数据的个数

//全局结构体变量 此文件中使用
static Meter_Memory meter_meory_gooble;
/**********************************************/
Sever_ToEqu sev_toeque[EquStackSize];//队列大小20
Sever_ToEqu* free_toequ;
Sever_ToEqu* busy_toequ;
Meter_Type meter; //定义从电表设备接收的数据大小
/**********************************************/
Buadrate_Type buadrate_type =
{
    false,
    false,
    false,
    false
};
/**********************************************/
uint8_t* Get_Meter_Addr( uint8_t* addr_data,uint8_t *out_addr_head);
uint8_t* Save_Addr( uint8_t* addr_data);
/*****************************************************/
void HAL_TIM_PeriodElapsedCallback( TIM_HandleTypeDef* htim )
{
    uint8_t static i = 0;
    uint8_t static j = 0;
    if( htim == &htim6 )
    {
        i++;
        if( i % 2 == 0 ) //因为在定时器使能时会进入中断，所以要在第二次进入该中断才说明发送数据完成
        {
#ifdef INTERRUPT_DMA_FREE
#else
            modu433_Time_Callback();
#endif
            i = 0;
        }
    }
    if( htim == &htim7 )
    {
        j++;
        if( j % 2 == 0 ) //因为在定时器使能时会进入中断，所以要在第二次进入该中断才说明发送数据完成
        {
#ifdef INTERRUPT_DMA_FREE
#else
            Meter_Time_Callback();
#endif
            j = 0;
        }
    }
}
/*****************************************************/

#ifndef INTERRUPT_DMA_FREE

void Meter_Rec_Callback( void )
{
    htim7.Instance->CNT = 0;
    if( ( htim7.Instance->CR1 & 1 ) == 0 )
    {
        HAL_TIM_Base_Start_IT( &htim7 );
    }
}


void Meter_Time_Callback()
{
    meter_rec_num = huart1.RxXferSize - huart1.RxXferCount ; //每次接收到数据后进行自减
    HAL_TIM_Base_Stop_IT( &htim7 );
    htim7.Instance->CNT = 0;
    //接收完成标志
    meter.rec_finish = true;
    //确定收到了多少个来自电表的数据
    HAL_UART_AbortReceive( &huart1 );
}
#endif
/*****************************************************/
void process_cmd( void ) //处理一条命令
{
    Get_Buadrate_func( &meter_meory_gooble );      //读取片内flash的 记录进行匹配使用
    while( 1 )
    {
        //    sim_wait_answer();
        Process_Command_Cache();
    }
}

/*****************************************************/
/*****************************************************/
void set_buadrate( void )
{
    if( buadrate_type.low_buadrate != false )
    {
        set_485buadrate( LOW_BUADRATE );
        buadrate_type.low_buadrate = false;
    }
    if( buadrate_type.med_buadrate != false )
    {
        set_485buadrate( MED_BUADRATE );
        buadrate_type.med_buadrate = false;
    }
    if( buadrate_type.high_buadrate != false )
    {
        set_485buadrate( HIGH_BUADRATE );
        buadrate_type.high_buadrate = false;
    }
    if( buadrate_type.very_high_buadrate != false )
    {
        set_485buadrate( VERY_HIGH_BUADRATE );
        buadrate_type.very_high_buadrate = false;
    }
}


bool set_init_buadrate( void )
{
    if( get_meter_addr( VERY_LOW_BUADRATE,  1000 ) ) //LOW_BUADRATE
    {
        return true;
    }
    else  if( get_meter_addr( LOW_BUADRATE, 1000 ) )
    {
        return true;
    }
    else  if( get_meter_addr( MED_BUADRATE, 1000 ) )
    {
        return true;
    }
    else  if( get_meter_addr( HIGH_BUADRATE, 1000 ) )
    {
        return true;
    }
    return false;
}

bool Judge_645_Entire( uint8_t* data, uint8_t count )
{
    uint8_t i = 0;
    for( i = 0; i < count; i++ )
    {
        if( data[i] == 0x68 )
        {
            if( ( i + 7 <= count ) && ( data[i + 7] == 0x68 ) )
            {
                return true;
            }
        }
    }
    return false;
}


//bool Get_645_Head( uint8_t* data)
//{
//    uint8_t i = 0;
//    for( i = 0; i < count; i++ )
//    {
//        if( data[i] == 0x68 )
//        {
//            data=data+i;
//            return true;
////            if( ( i + 7 <= count ) && ( data[i + 7] == 0x68 ) )
////            {
////              
////                return true;
////            }
//        }
//    }
//    return false;
//}


bool get_meter_addr( uint32_t buadrate , uint32_t time )
{
    uint32_t tickStart = 0;
    bool proceeResult = false;
    u8 rec_data[20];
    uint8_t askData[] = {0xfe, 0xfe, 0xfe, 0xfe, 0x68, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x68, 0x13, 0x00, 0xdf, 0x16};
    set_485buadrate( buadrate );
    HAL_UART_Receive_IT( &huart1, rec_data, FromMeterSize );
    meter_rec_num = 0; //清除接收数据计数量
    RS485_Send_Data( askData, 16 );
    tickStart = HAL_GetTick();
    while( 1 )
    {
        if( HAL_GetTick() - tickStart >= time )
        {
            proceeResult = false;
            break;
        }
        if( meter.rec_finish )
        {
            if( Judge_645_Entire( rec_data, meter_rec_num ) )
            {
                //            HAL_UART_Transmit(&huart1,rec_data,meter_rec_num,time); //ccdebug
                proceeResult = true;
                meter.rec_finish = false;
                break;
            }
        }
    }
    return proceeResult;
}

uint16_t  Mode_Check_And_Choice( void ) //uint16_t *buadtare
{
    u8 status = 0;
    uint16_t buadrate = 0;
    if( HAL_GPIO_ReadPin( MODE_CHICE_PORT, MODE_CHICE_Pin_12 ) == GPIO_PIN_RESET )
    {
        HAL_Delay( 1 );  //上电延时1s 要比电表启动慢一些
        if( HAL_GPIO_ReadPin( MODE_CHICE_PORT, MODE_CHICE_Pin_12 ) == GPIO_PIN_RESET )
        {
            status |= 0x01;
        }
    }
    if( HAL_GPIO_ReadPin( MODE_CHICE_PORT, MODE_CHICE_Pin_13 ) == GPIO_PIN_RESET )
    {
        HAL_Delay( 1 );    //上电延时1s 要比电表启动慢一些
        if( HAL_GPIO_ReadPin( MODE_CHICE_PORT, MODE_CHICE_Pin_13 ) == GPIO_PIN_RESET )
        {
            status |= 0x02;
        }
    }
    if( HAL_GPIO_ReadPin( MODE_CHICE_PORT, MODE_CHICE_Pin_14 ) == GPIO_PIN_RESET )
    {
        HAL_Delay( 1 );    //上电延时1s 要比电表启动慢一些
        if( HAL_GPIO_ReadPin( MODE_CHICE_PORT, MODE_CHICE_Pin_14 ) == GPIO_PIN_RESET )
        {
            status |= 0x04;
        }
    }
    if( status != 0 )
    {
        if( status == 0x01 )
        {
            buadrate = 1200;
        }
        if( status == 0x02 )
        {
            buadrate = 2400;
        }
        if( status == 0x03 )
        {
            buadrate = 4800;
        }
        //4为KEY3不能单独使用
        if( status == 0x05 )
        {
            buadrate = 9600;
        }
        if( status == 0x06 )
        {
            buadrate = 19200;
        }
    }
    return buadrate;
}


bool Process_Command_To_Meter( uint8_t* data, uint16_t rece_data_num, uint16_t time )
{
    uint32_t tickStart = 0;
    bool  status = false;
    uint16_t buadrate = 0; //自适应波特率
    uint8_t meter_rec_data[FromMeterSize]; // ccdebug   FromMeterSize
    meter_rec_num = 0;  //将数据清空 然后发送出去
    //得到数据的地址 如果是广播地址则使用2400进行广播，如果收到的是9999999地址则使用1200进行广播
    //查找波特率是不是已经存在了 没有存在说明是97电表使用1200波特率进行数据发送与接收
    //如果存在了                则使用记录的波特率进行数据的接收发送
    //   buadrate= Mode_Check_And_Choice(); //先判断是不是存在模式上的选择 ccdebug
    
    if( buadrate == FALSE ) //模式选择没有开启
    {
        if( Read_Meter_Buadrate_Func( &meter_meory_gooble, data, &buadrate ) )
        {
            if( buadrate == 1 ) //99广播地址 发送两边
            {
                set_485buadrate( 2400 );
                RS485_Send_Data( data, rece_data_num );
                set_485buadrate( 1200 );
                RS485_Send_Data( data, rece_data_num );
                return true ;
            }
             if( buadrate == data_error ) //如果数据错误 就直接返回
            {
                return true ;
            }
        }
        else
        {
            buadrate = 1200;
        }
        set_485buadrate( buadrate );
    }
    else
    {
        //使用设置好的波特率
        set_485buadrate( buadrate );
    }
    __HAL_UART_ENABLE_IT( &huart1, UART_IT_IDLE ); //开启空闲中断
    HAL_UART_Receive_DMA( &huart1, meter_rec_data , FromMeterSize );
    //发送数据到电表
    RS485_Send_Data( data, rece_data_num );
    tickStart = HAL_GetTick();
    while( 1 )
    {
        if( ( HAL_GetTick() - tickStart ) > time )
        {
            status = false;
            break;
        }
        if( meter.rec_finish )
        {
            if( meter_rec_data[meter_rec_num - 1] != 0x16 )
            {
                meter.rec_finish = false;
                return status;
            }
            else
            {
                meter.rec_finish = false;
                module_send_data( meter_rec_data, meter_rec_num );
                status = true;
            }
            break;
        }
    }
    return status;
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

void module_send_data( uint8_t* buf, uint16_t len )
{
    //  DISABLE_INT();
    uint16_t t;
    for( t = 0; t < len; t++ ) //循环发送数据
    {
        while( ( UART4->SR & 0X40 ) == 0 ); //等待发送结束
        UART4->DR = buf[t];
    }
    while( ( UART4->SR & 0X40 ) == 0 ); //等待发送结束
    //
    //   ENABLE_INT();
}


static void set_485buadrate( u32 buadrate )
{
    float temp;
    u16 mantissa;
    u16 fraction;
    temp = ( float )( 72 * 1000000 ) / ( buadrate * 16 ); //得到 USARTDIV
    mantissa = temp; //得到整数部分
    fraction = ( temp - mantissa ) * 16; //得到小数部分
    mantissa <<= 4;
    mantissa += fraction;
    USART1->BRR = mantissa; // 波特率设置
    //    if (buadrate==9600)
    //    USART1->BRR=0x7530; // 波特率设置
    //    if (buadrate==1200)
    //    USART1->BRR=0xEA60; // 波特率设置
    //  huart1.Instance = USART1;
    //  huart1.Init.BaudRate = buadrate;
    //  huart1.Init.WordLength = UART_WORDLENGTH_9B;
    //  huart1.Init.StopBits = UART_STOPBITS_1;
    //  huart1.Init.Parity = UART_PARITY_EVEN;
    //  huart1.Init.Mode = UART_MODE_TX_RX;
    //  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    //  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    //  if (HAL_UART_Init(&huart1) != HAL_OK)
    //  {
    //    _Error_Handler(__FILE__, __LINE__);
    //  }
    //RCC->APB2ENR|=1<<2; //使能 PORTA 口时钟
    //RCC->APB2ENR|=1<<14; //使能串口时钟
    ////GPIOA->CRH&=0XFFFFF00F;//IO 状态设置
    ////GPIOA->CRH|=0X000008B0;//IO 状态设置
    ////RCC->APB2RSTR|=1<<14; //复位串口 1
    ////RCC->APB2RSTR&=~(1<<14);//停止复位
    //////波特率设置
    ////USART1->BRR=mantissa; // 波特率设置
    ////USART1->CR1|=0X200C; //1 位停止,无校验位.
    //
    //}
}


//初始化sim接收队列 初始化发送队列 两个队列进行数据的收发
void core_init( void )
{
    sim_initial();
}


bool Save_Meter_Addr( uint32_t buadrate , uint8_t* out_data, uint32_t time )
{
    uint32_t tickStart = 0;
    bool proceeResult = false;
    //    u8 rec_data[20];
    //点对点通讯，读取电表的地址
    uint8_t askData[] = {0xfe, 0xfe, 0xfe, 0xfe, 0x68, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x68, 0x13, 0x00, 0xdf, 0x16};
    set_485buadrate( buadrate );
    meter_rec_num = 0; //清除接收数据计数量
    __HAL_UART_ENABLE_IT( &huart1, UART_IT_IDLE ); //开启空闲中断
    HAL_UART_Receive_DMA( &huart1, out_data , FromMeterSize );
    RS485_Send_Data( askData, 16 );
    tickStart = HAL_GetTick();
    while( 1 )
    {
        if( HAL_GetTick() - tickStart >= time )
        {
            proceeResult = false;
            break;
        }
        if( meter.rec_finish )
        {
            if( Judge_645_Entire( out_data, meter_rec_num ) )
            {
                //            HAL_UART_Transmit(&huart1,out_data,meter_rec_num,time); //ccdebug
                proceeResult = true;
                meter.rec_finish = false;
                break;
            }
        }
    }
    return proceeResult;
}

//数据保存到flash中

bool  Save_Data_To_Flash( Meter_Memory* meter_meory_gooble, uint8_t* addr_data, uint8_t offset, uint16_t buadrate )
{
    uint8_t i ;
    uint16_t* save_addr_data;
    uint8_t *addr_data_pt;
    uint8_t buf_data[8];
    Meter_Memory meter_meory_type;
    // 取得数据融合后的数据头指针
    Get_Buadrate_func( meter_meory_gooble );
    //检查是否已经存在
    if( Check_Meter_Msg_Save( meter_meory_gooble, addr_data ) != false ) //ture 说明存在 进一步验证是不是需要更改
    {
        //    if( Check_Meter_Buadrate_Save(meter_meory_gooble,addr_data,buadrate,&offset)) //检查是否需要更改 不需要就返回正常
        return  true;
        //     else                                                            //需要更改就进行更改
        //  Record_Change_Buadrate_Func(&buadrate,&offset);
    }
    //取得数据的头部数据
    //校验过返回的数据后进行数据的+5 fefefefe68 第6个开始
    //存在不是fefefefe开头的情况 需要判别第一个68解决问题

    
    
    
    addr_data_pt=Save_Addr(addr_data);
    
    for( i = 0; i < 8; i++ )
    {
        buf_data[i] = addr_data_pt[i];
    }

    save_addr_data = Data_Fuse_8Bit_To_16Bit( buf_data, 8 );
    for( i = 0; i < 4; i++ )
    {
        meter_meory_type.write_msg[i] = save_addr_data[i];
    }
    if( Record_Buadrate_Function( &buadrate, meter_meory_type.write_msg ) )
    {
        return true ;
    }
    else
    {
        return false;    //已经保存
    }
}


//配置模式 进行存储数据
bool Config_Mode_Func( void )
{
    //波特率记录
    //   Meter_Memory meter_meory_type;
    uint8_t addr_data[25];
    //   uint16_t *save_addr_data;
    uint16_t buadrate;
    //   uint8_t i ;
    uint8_t offset = 0 ;
    if( Save_Meter_Addr( VERY_LOW_BUADRATE, addr_data, 1000 ) ) //LOW_BUADRATE
    {
        buadrate = VERY_LOW_BUADRATE;
        if( Save_Data_To_Flash( &meter_meory_gooble, addr_data, offset, buadrate ) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else  if( Save_Meter_Addr( LOW_BUADRATE, addr_data, 1000 ) ) //LOW_BUADRATE
    {
        buadrate = LOW_BUADRATE;
        if( Save_Data_To_Flash( &meter_meory_gooble, addr_data, offset, buadrate ) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else  if( Save_Meter_Addr( HIGH_BUADRATE, addr_data, 1000 ) ) //LOW_BUADRATE
    {
        buadrate = HIGH_BUADRATE;
        if( Save_Data_To_Flash( &meter_meory_gooble, addr_data, offset, buadrate ) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}



//func:得到电表地址对应的波特率 存在结构体数组中去
//函数：void Get_Buadrate_func(void)meter_meory_gooble;
void Get_Buadrate_func( Meter_Memory* meter_meor )
{
    uint16_t i, j, k;
    uint16_t buf_data[10];
    uint8_t* bit8_data_pt;
    STMFLASH_Read( METER_CNT_BASE, &meter_meor->record_num, 1 );
    //如果地址没有数据存在
    if( meter_meor->record_num == 0xffff || 0 )
    {
        meter_meor->record_num = 0;
        return;
    }
    //将已经写入的全部读出 记录在数组中
    for( i = 0; i < meter_meor->record_num; i++ )
    {
        //将电表的存储到的地址 和波特率全部读出 4个半字
        STMFLASH_Read( METER_BASE + i * 8, buf_data, METER_ADDR_SIZE + 1 );
        //数据分解为6位
        bit8_data_pt = Data_Resolve_16Bit_To_8Bit( buf_data, METER_ADDR_SIZE );
        for( j = 0; j < METER_ADDR_SIZE * 2; j++ )
        {
            meter_meor->meter_msg[i][j] = bit8_data_pt[j];
        }
        meter_meor->meter_buadrate[i] = buf_data[METER_ADDR_SIZE];
        //清空使用的数组 测试可以不清楚 //ccdebug
        for( k = 0; k < 10; k++ )
        {
            buf_data[k] = 0;    //ccdebug
        }
    }
}

//得到电表的地址 处理返回数据
uint8_t* Get_Meter_Addr( uint8_t* addr_data,uint8_t *out_addr_head)
{
    uint8_t* addr_data_pt;
    // bool status=false;
    uint8_t i;
    addr_data_pt = addr_data;
    for( i = 0;i<10; i++ )
    {
        if( addr_data_pt[i] == 0x68 )
        {
            if( addr_data_pt[i + 7] == 0x68 )
            {
                *out_addr_head=addr_data_pt[i];
                i = i + 1;
                addr_data_pt = addr_data + i;
                break;           
            }
            //如果数据存在错误就返回
            else
            {
                  
                   break;  
            }
            
        }
      }      
    return addr_data_pt;
}


//check 电表中的数据是否已经保存
//保证验证速度 先进行一个数据的校对 校对完成一个数据后在进行其他的数据的检查
//输入的参数： Meter_Memory * meter_memory 数据存储结构体   uint8_t *addr_data 输入的电表的数据
bool Check_Meter_Msg_Save( Meter_Memory* meter_memory, uint8_t* addr_data )
{
    uint16_t i, j;
    uint8_t addr_head;
    //得到指向68后开始的头
    addr_data = Get_Meter_Addr( addr_data,&addr_head);
    for( i = 0; i < meter_memory->record_num; i++ )
    {
        //校验头是否匹配
        if( meter_memory->meter_msg[i][0] == addr_data[0] )
        {
            for( j = 0; j < 6; j++ )
            {
                if( meter_memory->meter_msg[i][j] == addr_data[j] )
                {
                    if( j == 5 )
                    {
                        return true; //已存在
                    }
                }
            }
        }
    }
    //没有找到
    return false;
}



bool Check_Meter_Buadrate_Save( Meter_Memory* meter_memory, uint8_t* addr_data,
                                uint16_t buadrate, uint8_t* out_data
                              )
{
    uint16_t i, j;
    uint8_t *addr_head=addr_null;
    //得到指向68后开始的头
    addr_data = Get_Meter_Addr( addr_data ,addr_head);
    for( i = 0; i < meter_memory->record_num; i++ )
    {
        //校验头是否匹配
        if( meter_memory->meter_msg[i][0] == addr_data[0] )
        {
            for( j = 0; j < 6; j++ )
            {
                if( meter_memory->meter_msg[i][j] == addr_data[j] )
                {
                    if( j == 5 )
                    {
                        if( meter_memory->meter_buadrate[i] != buadrate )
                        {
                            *out_data = i;
                            return false;   //存在不同波特率
                        }
                    }
                }
            }
        }
    }
    //没有找到
    return true;
}



//查询已经存储的波特率
//判断是不是广播 广播使用固定的波特率
bool Read_Meter_Buadrate_Func( Meter_Memory* meter_memory, uint8_t* addr_data, uint16_t* out_budarate )
{
    uint16_t i, j;
     uint8_t addr_head;
    uint8_t* addr_data_pt;
    //得到指向68后开始的头

    addr_data_pt = Get_Meter_Addr( addr_data ,&addr_head);
    if(addr_head!=0x68)
    {
      //数据错误 不需要进行下去
        *out_budarate=data_error;
         return true; //返回
    }
   
    for( i = 0; i < meter_memory->record_num; i++ )
    {
        //校验头是否匹配
        if( meter_memory->meter_msg[i][0] == addr_data_pt[0] )
        {
            for( j = 0; j < 6; j++ )
            {
                if( meter_memory->meter_msg[i][j] == addr_data_pt[j] )
                {
                    if( j == 5 )
                    {
                        *out_budarate = meter_memory->meter_buadrate[i];
                        return true; //已存在
                    }
                }
            }
        }
    }
    //应对所有的广播
    if( addr_data_pt[0] == 0x99 )
    {
        for( i = 0; i < 6; i++ )
        {
            if( addr_data_pt[i] != 0x99 )
            {
                break;
            }
            if( i == 5 )
            {
                //        *out_budarate =1200;
                //        *out_budarate =64597;
                *out_budarate = 1;
                return true;
            }
        }
    }
    //    if(addr_data_pt[0]==0xaa)
    //    {
    //     for(i=0;i<6;i++)
    //     {
    //        if(addr_data_pt[i]!=0xaa)
    //        {
    //           break;
    //        }
    //        if(i==5)
    //        {
    //          *out_budarate =2400;
    ////          *out_budarate =64507;
    //         return true;
    //        }
    //     }
    //    }
    //没有找到
    return false;
}



//得到电表的地址 处理返回数据
uint8_t* Save_Addr( uint8_t* addr_data)
{
    uint8_t* addr_data_pt;
    // bool status=false;
    uint8_t i;
    addr_data_pt = addr_data;
    for( i = 0;i<10; i++ )
    {
        if( addr_data_pt[i] == 0x68 )
        {
            if( addr_data_pt[i + 7] == 0x68 )
            {
               
                i = i + 1;
                addr_data_pt = addr_data + i;
                break;           
            }
            //如果数据存在错误就返回
            else
            {
                  
                   break;  
            }
            
        }
      }      
    return addr_data_pt;
}


