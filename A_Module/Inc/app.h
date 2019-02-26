
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

#define  MODE_CHICE_PORT GPIOB          //ģʽѡ��
#define  MODE_CHICE_Pin  GPIO_PIN_15   //ģʽѡ��˿�





#define  MODE_CHICE_PORT GPIOB          //ģʽѡ��
#define  MODE_CHICE_Pin_12  GPIO_PIN_12   //ģʽѡ��˿�12


#define  MODE_CHICE_PORT GPIOB          //ģʽѡ��
#define  MODE_CHICE_Pin_13  GPIO_PIN_13   //ģʽѡ��˿�13

#define  MODE_CHICE_PORT GPIOB          //ģʽѡ��
#define  MODE_CHICE_Pin_14  GPIO_PIN_14   //ģʽѡ��˿�14


#define RS_485_RX() HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_SET)
#define RS_485_TX() HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_RESET)

//#define RS_485_TX() HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_SET)
//#define RS_485_RX() HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_RESET)

//�õʹ򿪵�
#define LED_1(n)  ((n!=FALSE)? HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,GPIO_PIN_RESET): HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,GPIO_PIN_SET))
//#define LED_2(n)  ((n!=FALSE)? HAL_GPIO_WritePin(LED2_PORT,LED2_PIN,GPIO_PIN_RESET) :HAL_GPIO_WritePin(LED2_PORT,LED2_PIN,GPIO_PIN_SET))


#define METER_ADDR_SIZE 3   //���������ݴ�С ���˳��
#define METER_BUADRATE_SIZE      (METER_FLASH_ADDR_SIZE+1)  //���Ĳ����ʵĴ��λ��

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
//�õ������ʵĺ���
void Get_Buadrate_func(  Meter_Memory *meter_meor);

//��������Ϣ �ǲ����Ѿ�����
bool Check_Meter_Msg_Save(Meter_Memory * meter_memory, uint8_t *addr_data);
//У�鲨�����ǲ����Ѿ��޸�
bool Check_Meter_Buadrate_Save(Meter_Memory * meter_memory, uint8_t *addr_data,uint16_t buadrate,uint8_t *out_data);
//���ҵ��Ĳ����ʲ����ú�
bool Read_Meter_Buadrate_Func(Meter_Memory * meter_memory, uint8_t *addr_data,uint16_t *out_budarate);
#endif






















































































