#ifndef __FLASH_H__
#define __FLASH_H__
#include "tool.h"

/***************************************************************************************
电表地址以及波特率存储说明：
             LAST_TWO_PAGE ： 存储电表的数量 最大存储数 ：16块电表
             LAST_ONE_PAGE ： 存储电表信息8字节   电表地址占用6字节，波特率占用两个字节， 
                                       AA AA AA AA  AA AA   BB BB
                              ADDR：   AA AA AA AA  AA AA 
                              BUADRATE:                     BB BB
***************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 	256	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 	1              	//使能FLASH写入(0，不是能;1，使能)
#define STM32_FLASH_BASE        0x08000000 		//STM32 FLASH的起始地址
//FLASH解锁键值
//#define FLASH_KEY1               0X45670123
//#define FLASH_KEY2               0XCDEF89AB
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIGE_SIZE       2048
#define MXA_RECORD      18                     // 最大记录的数目
#define METER_MSG_SIZE  6                      // 每条记录信息内容大小

//一页2k字节 一共0-127页 
#define LAST_TWO_PAGE    126   // ((STM32_FLASH_SIZE/2)-2)   //倒数第二页的位置 
#define LAST_ONE_PAGE    127   // ((STM32_FLASH_SIZE/2)-1)   //最后一页 本次用来存储电表的地址以及对应的波特率 

#define METER_BASE            (STM32_FLASH_BASE+(LAST_ONE_PAGE*PIGE_SIZE)) 		//STM32 FLASH的起始地址
#define METER_CNT_BASE        (STM32_FLASH_BASE+(LAST_TWO_PAGE*PIGE_SIZE)) 		//STM32 FLASH的起始地址


#define FUSE_AMOUNT      4 	//需要8-16位融合的个数

//在倒数第2页的位置存储次数
//在倒数第二

//记录flash的使用次数的结构体 
typedef struct memory
{
 uint8_t  meter_addr[6];                               //电表地址
 
 uint8_t meter_msg[MXA_RECORD][METER_MSG_SIZE];  //已经记录的信息
 
 uint16_t meter_buadrate[MXA_RECORD];                             //波特率
 
 uint16_t record_num;                             //电表已经记录次数
 
 uint16_t write_msg[METER_MSG_SIZE];             //需要写入的信息半字 16位
 
 uint16_t read_msg[METER_MSG_SIZE];              //需要读出的信息半字 16位
 
}Meter_Memory;


void STMFLASH_WriteLenByte(uint32_t WriteAddr,uint32_t DataToWrite,uint16_t Len);	//指定地址开始写入指定长度的数据
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr,uint16_t Len);  //指定地址开始读取指定长度数据
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);		//从指定地址开始写入指定长度的数据

void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);   //从指定地址开始读出指定长度的数据

void STMFLASH_Unlock(void);					  	//FLASH解锁
void STMFLASH_Lock(void);					  	//FLASH上锁
uint8_t STMFLASH_GetStatus(void);				  	//获得状态
uint8_t STMFLASH_WaitDone(uint16_t time);				  	//等待操作结束
uint8_t STMFLASH_ErasePage(uint32_t paddr);			  	//擦除页
uint8_t STMFLASH_WriteHalfWord(uint32_t faddr, uint16_t dat);	//写入半字
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  	//读出半字  

bool Record_Buadrate_Function(uint16_t *record_budarata,uint16_t *record_adddr);

//记录更改后的波特率
bool Record_Change_Buadrate_Func(uint16_t *record_budarata,uint16_t *record_adddr);


void FLASH_Erasure(void); //擦除已经写入的东西然后再进行重新上电的


#endif