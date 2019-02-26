#ifndef __FLASH_H__
#define __FLASH_H__
#include "tool.h"

/***************************************************************************************
����ַ�Լ������ʴ洢˵����
             LAST_TWO_PAGE �� �洢�������� ���洢�� ��16����
             LAST_ONE_PAGE �� �洢�����Ϣ8�ֽ�   ����ַռ��6�ֽڣ�������ռ�������ֽڣ� 
                                       AA AA AA AA  AA AA   BB BB
                              ADDR��   AA AA AA AA  AA AA 
                              BUADRATE:                     BB BB
***************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 	256	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 	1              	//ʹ��FLASHд��(0��������;1��ʹ��)
#define STM32_FLASH_BASE        0x08000000 		//STM32 FLASH����ʼ��ַ
//FLASH������ֵ
//#define FLASH_KEY1               0X45670123
//#define FLASH_KEY2               0XCDEF89AB
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define PIGE_SIZE       2048
#define MXA_RECORD      18                     // ����¼����Ŀ
#define METER_MSG_SIZE  6                      // ÿ����¼��Ϣ���ݴ�С

//һҳ2k�ֽ� һ��0-127ҳ 
#define LAST_TWO_PAGE    126   // ((STM32_FLASH_SIZE/2)-2)   //�����ڶ�ҳ��λ�� 
#define LAST_ONE_PAGE    127   // ((STM32_FLASH_SIZE/2)-1)   //���һҳ ���������洢���ĵ�ַ�Լ���Ӧ�Ĳ����� 

#define METER_BASE            (STM32_FLASH_BASE+(LAST_ONE_PAGE*PIGE_SIZE)) 		//STM32 FLASH����ʼ��ַ
#define METER_CNT_BASE        (STM32_FLASH_BASE+(LAST_TWO_PAGE*PIGE_SIZE)) 		//STM32 FLASH����ʼ��ַ


#define FUSE_AMOUNT      4 	//��Ҫ8-16λ�ںϵĸ���

//�ڵ�����2ҳ��λ�ô洢����
//�ڵ����ڶ�

//��¼flash��ʹ�ô����Ľṹ�� 
typedef struct memory
{
 uint8_t  meter_addr[6];                               //����ַ
 
 uint8_t meter_msg[MXA_RECORD][METER_MSG_SIZE];  //�Ѿ���¼����Ϣ
 
 uint16_t meter_buadrate[MXA_RECORD];                             //������
 
 uint16_t record_num;                             //����Ѿ���¼����
 
 uint16_t write_msg[METER_MSG_SIZE];             //��Ҫд�����Ϣ���� 16λ
 
 uint16_t read_msg[METER_MSG_SIZE];              //��Ҫ��������Ϣ���� 16λ
 
}Meter_Memory;


void STMFLASH_WriteLenByte(uint32_t WriteAddr,uint32_t DataToWrite,uint16_t Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr,uint16_t Len);  //ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����

void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);   //��ָ����ַ��ʼ����ָ�����ȵ�����

void STMFLASH_Unlock(void);					  	//FLASH����
void STMFLASH_Lock(void);					  	//FLASH����
uint8_t STMFLASH_GetStatus(void);				  	//���״̬
uint8_t STMFLASH_WaitDone(uint16_t time);				  	//�ȴ���������
uint8_t STMFLASH_ErasePage(uint32_t paddr);			  	//����ҳ
uint8_t STMFLASH_WriteHalfWord(uint32_t faddr, uint16_t dat);	//д�����
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  	//��������  

bool Record_Buadrate_Function(uint16_t *record_budarata,uint16_t *record_adddr);

//��¼���ĺ�Ĳ�����
bool Record_Change_Buadrate_Func(uint16_t *record_budarata,uint16_t *record_adddr);


void FLASH_Erasure(void); //�����Ѿ�д��Ķ���Ȼ���ٽ��������ϵ��


#endif