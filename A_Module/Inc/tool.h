#ifndef __TOOL_H__
#define __TOOL_H__
//#include "stm32f4xx.h"
//#include "stm32f4xx_hal.h"
#include "stm32f1xx_hal.h"
//#include "main.h"

#include "stm32f1xx.h"



#define TRUE   1
#define FALSE  0

#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
/*******************************************************************************************/

/*******************************************************************************************/
#define VERY_LOW_BUADRATE   1200 
#define LOW_BUADRATE        2400
#define MED_BUADRATE        4800
#define HIGH_BUADRATE       9600
#define VERY_HIGH_BUADRATE  115200
/*******************************************************************************************/
typedef enum
{
    false,
    true,
}bool;

/*******************************************************************************************/
//�������ݽ���ͷ�ļ� 
/*******************************************************************************************/

#define REC_LEN       300 //���ճ���
#define SEND_LEN      300 // ���ͳ���

#define REC_QUE      24  //���г���

#define CACHE_SIZE      24    // 

#define CACHE_DATASIZE  300


typedef struct rec_typedef
{
 bool     has_rec_acp;     //afһ֡���ݽ������ 
 uint8_t  msg[REC_LEN];
 struct   rec_typedef *pre_rec; //���յ�ǰһ֡
 struct   rec_typedef *next_rec;
}Rec_Typedef;


typedef struct 
{
 uint8_t    start_index;    
 uint8_t    end_index;
 uint16_t   rece_data_num[CACHE_SIZE];
 uint8_t    cache[CACHE_SIZE][CACHE_DATASIZE];
}Cache_Typedef;

#define  REC_NULL   (Rec_Typedef *)0
/*******************************************************************************************/
/*******************************************************************************************/
/*******************************************************************************************/

#define MeS_length  300
#define FromMeterSize 300
#define EquStackSize 20  


typedef struct Sever_ToEqu
{
  uint16_t tx_count;
  bool     equ_answer;
  bool     had_send_sever;
  uint8_t  msg_content[MeS_length];
  struct   Sever_ToEqu *prev; 
  struct   Sever_ToEqu *next;   
}Sever_ToEqu; 

typedef struct{
  bool    know_address;
  bool    rec_finish;
  uint8_t tx_conunt;
//    uint8_t tx_conunt;
  uint8_t address[6];
  uint8_t data_from_meter[FromMeterSize];
}Meter_Type; 

#define ToEque_Null (Sever_ToEqu*)0
/*******************************************************************************************/	 
/*******************************************************************************************/

/*******************************************************************************************/
/******************************************************************************************/
bool FindString(char * wantChar,char* sourceChar,uint16_t * index,uint32_t length);

//�Ƿ�����ַ���
bool ContainsString(char * wantChar ,char * sourceChar,uint32_t length);

//�Ƿ�����ַ���
bool ContanisStrings(char * wantChar1,char * wantChar2,char* wantChar3 ,char * sourceChar ,uint8_t * index,uint32_t length);

//�Ƿ���ָ����������ʼ
bool StartWith(char * wantChar,char * sourceChar);

//��String ת��ΪHex
bool ConvertStringToHex(uint8_t * dest, char * sourceChar);


void ConvertHexToString(char * dest,uint8_t * sourceByte,uint16_t count);

//��������
void copyTo(uint8_t * source, uint8_t * dest, uint16_t count);

void ConvertHexToChar(uint16_t  source,char * dest );

//�����ַ���
void ConnactString(char * source,char * dest);

//����ָ�����ȵ��ַ���
void ConnactLengthString(char * source,char * dest,uint16_t length);

bool ConvertHexStringToInt(uint8_t * data, uint8_t count,uint16_t * outData);


//��BCD��ת��ΪHex�ֽ�
uint8_t ConvertBcd_ToHex(uint8_t data);


//��Hexת��ΪBCD���ʽ
uint8_t ConvertHex_ToBcd(uint8_t data);


//��ת���ݵ�˳��
void ReserveCache(uint8_t * data,uint16_t count);


//���ڴ�����Ϊָ��������
void MemSet(uint8_t * source ,uint8_t data,uint16_t count);


//��ȡ�ַ�������
uint32_t GetCharsLength(char * data);


uint8_t * GetStringFromKeyWords(uint8_t * datain,uint16_t count,char firstKey,char lastKey,uint16_t * length);

uint8_t * GetFirstStringFromKeyWord(uint8_t * datain,uint16_t count,char split,uint16_t * length,bool containKey);

uint8_t * GetLastStringFromKeyWord(uint8_t * datain,uint16_t count,char split,uint16_t * length,bool containKey);
//�����ں�
uint16_t * Data_Fuse_8Bit_To_16Bit(uint8_t *cin_data,uint8_t fuse_length);
//���ݷֽ�
uint8_t * Data_Resolve_16Bit_To_8Bit(uint16_t *cin_data,uint8_t resolve_length);
//////////////////////////////////////////////////////////////////////  
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
///////////////////////////////////////////////////////////////////////////////////
//����һЩ���õ��������Ͷ̹ؼ��� 
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  
typedef const int16_t sc16;  
typedef const int8_t sc8;  

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  
typedef __I int16_t vsc16; 
typedef __I int8_t vsc8;   

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  
typedef const uint16_t uc16;  
typedef const uint8_t uc8; 

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  
typedef __I uint16_t vuc16; 
typedef __I uint8_t vuc8;  

//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����



//GPIO����ר�ú궨��
#define GPIO_MODE_IN    	0		//��ͨ����ģʽ
#define GPIO_MODE_OUT		1		//��ͨ���ģʽ
#define GPIO_MODE_AF		2		//AF����ģʽ
#define GPIO_MODE_AIN		3		//ģ������ģʽ

#define GPIO_SPEED_2M		0		//GPIO�ٶ�2Mhz
#define GPIO_SPEED_25M		1		//GPIO�ٶ�25Mhz
#define GPIO_SPEED_50M		2		//GPIO�ٶ�50Mhz
#define GPIO_SPEED_100M		3		//GPIO�ٶ�100Mhz

#define GPIO_PUPD_NONE		0		//����������
#define GPIO_PUPD_PU		1		//����
#define GPIO_PUPD_PD		2		//����
#define GPIO_PUPD_RES		3		//���� 

#define GPIO_OTYPE_PP		0		//�������
#define GPIO_OTYPE_OD		1		//��©��� 

//GPIO���ű�Ŷ���
#define PIN0				1<<0
#define PIN1				1<<1
#define PIN2				1<<2
#define PIN3				1<<3
#define PIN4				1<<4
#define PIN5				1<<5
#define PIN6				1<<6
#define PIN7				1<<7
#define PIN8				1<<8
#define PIN9				1<<9
#define PIN10				1<<10
#define PIN11				1<<11
#define PIN12				1<<12
#define PIN13				1<<13
#define PIN14				1<<14
#define PIN15				1<<15 
////////////////////////////////////////////////////////////////////////////////// 

void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq);//ʱ��ϵͳ����
//void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD);//GPIO���ú���  
//void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx);//GPIOģʽ���ú���
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);			//����NVIC����
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);//�����ж�
void delay_us(uint32_t nus); //���ú��뼶����ӳ�
void delay_ms(uint16_t mus); //���ú��뼶����ӳ�
//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ 

#endif

