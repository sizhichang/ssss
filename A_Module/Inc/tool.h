#ifndef __TOOL_H__
#define __TOOL_H__
//#include "stm32f4xx.h"
//#include "stm32f4xx_hal.h"
#include "stm32f1xx_hal.h"
//#include "main.h"

#include "stm32f1xx.h"



#define TRUE   1
#define FALSE  0

#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */
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
//定义数据接受头文件 
/*******************************************************************************************/

#define REC_LEN       300 //接收长度
#define SEND_LEN      300 // 发送长度

#define REC_QUE      24  //队列长度

#define CACHE_SIZE      24    // 

#define CACHE_DATASIZE  300


typedef struct rec_typedef
{
 bool     has_rec_acp;     //af一帧数据接收完成 
 uint8_t  msg[REC_LEN];
 struct   rec_typedef *pre_rec; //接收的前一帧
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

//是否包含字符串
bool ContainsString(char * wantChar ,char * sourceChar,uint32_t length);

//是否包含字符串
bool ContanisStrings(char * wantChar1,char * wantChar2,char* wantChar3 ,char * sourceChar ,uint8_t * index,uint32_t length);

//是否以指定的数据起始
bool StartWith(char * wantChar,char * sourceChar);

//将String 转化为Hex
bool ConvertStringToHex(uint8_t * dest, char * sourceChar);


void ConvertHexToString(char * dest,uint8_t * sourceByte,uint16_t count);

//拷贝数据
void copyTo(uint8_t * source, uint8_t * dest, uint16_t count);

void ConvertHexToChar(uint16_t  source,char * dest );

//连接字符串
void ConnactString(char * source,char * dest);

//连接指定长度的字符串
void ConnactLengthString(char * source,char * dest,uint16_t length);

bool ConvertHexStringToInt(uint8_t * data, uint8_t count,uint16_t * outData);


//将BCD码转化为Hex字节
uint8_t ConvertBcd_ToHex(uint8_t data);


//将Hex转化为BCD码格式
uint8_t ConvertHex_ToBcd(uint8_t data);


//反转数据的顺序
void ReserveCache(uint8_t * data,uint16_t count);


//将内存设置为指定的数据
void MemSet(uint8_t * source ,uint8_t data,uint16_t count);


//获取字符串长度
uint32_t GetCharsLength(char * data);


uint8_t * GetStringFromKeyWords(uint8_t * datain,uint16_t count,char firstKey,char lastKey,uint16_t * length);

uint8_t * GetFirstStringFromKeyWord(uint8_t * datain,uint16_t count,char split,uint16_t * length,bool containKey);

uint8_t * GetLastStringFromKeyWord(uint8_t * datain,uint16_t count,char split,uint16_t * length,bool containKey);
//数据融合
uint16_t * Data_Fuse_8Bit_To_16Bit(uint8_t *cin_data,uint8_t fuse_length);
//数据分解
uint8_t * Data_Resolve_16Bit_To_8Bit(uint16_t *cin_data,uint8_t resolve_length);
//////////////////////////////////////////////////////////////////////  
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
///////////////////////////////////////////////////////////////////////////////////
//定义一些常用的数据类型短关键字 
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

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
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
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入



//GPIO设置专用宏定义
#define GPIO_MODE_IN    	0		//普通输入模式
#define GPIO_MODE_OUT		1		//普通输出模式
#define GPIO_MODE_AF		2		//AF功能模式
#define GPIO_MODE_AIN		3		//模拟输入模式

#define GPIO_SPEED_2M		0		//GPIO速度2Mhz
#define GPIO_SPEED_25M		1		//GPIO速度25Mhz
#define GPIO_SPEED_50M		2		//GPIO速度50Mhz
#define GPIO_SPEED_100M		3		//GPIO速度100Mhz

#define GPIO_PUPD_NONE		0		//不带上下拉
#define GPIO_PUPD_PU		1		//上拉
#define GPIO_PUPD_PD		2		//下拉
#define GPIO_PUPD_RES		3		//保留 

#define GPIO_OTYPE_PP		0		//推挽输出
#define GPIO_OTYPE_OD		1		//开漏输出 

//GPIO引脚编号定义
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

void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq);//时钟系统配置
//void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD);//GPIO设置函数  
//void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx);//GPIO模式设置函数
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);			//设置NVIC分组
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);//设置中断
void delay_us(uint32_t nus); //设置毫秒级别的延迟
void delay_ms(uint16_t mus); //设置毫秒级别的延迟
//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址 

#endif

