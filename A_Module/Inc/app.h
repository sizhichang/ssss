
#ifndef __APP_H__
#define __APP_H__

#include "tool.h"
#include "module.h"

#include "flash.h"

//#define RS485_EN(n) ((n!=FALSE)?(HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_SET)):(HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_RESET)) )

#define RS485_EN_GPIO_Port GPIOB 
#define RS485_EN_Pin GPIO_PIN_5  

#define LED1_PORT   GPIOB 
#define LED1_PIN    GPIO_PIN_1 

//#define LED2_PORT   GPIOE
//#define LED2_PIN    GPIO_PIN_5 

//#define GPIOB  RS485_EN_GPIO_Port
//#define GPIO_PIN_5  RS485_EN_Pin

#define  MODE_CHICE_PORT GPIOB          //模式选择
#define  MODE_CHICE_Pin  GPIO_PIN_15   //模式选择端口





#define  MODE_CHICE_PORT GPIOB          //模式选择
#define  MODE_CHICE_Pin_12  GPIO_PIN_12   //模式选择端口12


#define  MODE_CHICE_PORT GPIOB          //模式选择
#define  MODE_CHICE_Pin_13  GPIO_PIN_13   //模式选择端口13

#define  MODE_CHICE_PORT GPIOB          //模式选择
#define  MODE_CHICE_Pin_14  GPIO_PIN_14   //模式选择端口14


#define RS_485_RX() HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_SET)
#define RS_485_TX() HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_RESET)

//#define RS_485_TX() HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_SET)
//#define RS_485_RX() HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_RESET)

//置低打开灯
#define LED_1(n)  ((n!=FALSE)? HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,GPIO_PIN_RESET): HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,GPIO_PIN_SET))
//#define LED_2(n)  ((n!=FALSE)? HAL_GPIO_WritePin(LED2_PORT,LED2_PIN,GPIO_PIN_RESET) :HAL_GPIO_WritePin(LED2_PORT,LED2_PIN,GPIO_PIN_SET))


#define METER_ADDR_SIZE 3   //定义电表数据大小 存放顺序
#define METER_BUADRATE_SIZE      (METER_FLASH_ADDR_SIZE+1)  //电表的波特率的存放位置

typedef struct {
 bool low_buadrate;
 bool med_buadrate;
 bool high_buadrate;
 bool very_high_buadrate;
}Buadrate_Type;



void  process_cmd(void);
bool Config_Mode_Func(void);

bool set_init_buadrate(void);
void set_buadrate(void);
bool Judge_645_Entire(uint8_t * data,uint8_t count);
void set_flag_buadrate(void);
bool get_meter_two_addr(uint32_t buadrate ,uint32_t time);  //tiaoshi 
void RS485_Send_Data(uint8_t *buf,uint8_t len);
void Meter_Time_Callback();
void Meter_Rec_Callback(void);
void core_init(void);
bool Process_Command_To_Meter(uint8_t *data,uint16_t rece_data_num,uint16_t time);
bool get_meter_addr(uint32_t buadrate ,uint32_t time);
void module_send_data(uint8_t *buf,uint8_t len);
//得到波特率的函数
void Get_Buadrate_func(  Meter_Memory *meter_meor);

//检查电表的信息 是不是已经存在
bool Check_Meter_Msg_Save(Meter_Memory * meter_memory, uint8_t *addr_data);
//校验波特率是不是已经修改
bool Check_Meter_Buadrate_Save(Meter_Memory * meter_memory, uint8_t *addr_data,uint16_t buadrate,uint8_t *out_data);
//查找电表的波特率并设置好
bool Read_Meter_Buadrate_Func(Meter_Memory * meter_memory, uint8_t *addr_data,uint16_t *out_budarate);
#endif






















































































