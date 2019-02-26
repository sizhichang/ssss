//////////////////////////////////////////////////////////
//�������ڣ�2018/11/22
//�����ˣ�szc
//�ļ����ܣ�FLASH����������
//Copyright(C)��
//////////////////////////////////////////////////////////
#include "flash.h"
//#include "app.h"
//����STM32��FLASH
void STMFLASH_Unlock(void)
{
  FLASH->KEYR=FLASH_KEY1;//д���������.
  FLASH->KEYR=FLASH_KEY2;
}
//flash����
void STMFLASH_Lock(void)
{
  FLASH->CR|=1<<7;//����
}
//�õ�FLASH״̬
u8 STMFLASH_GetStatus(void)
{	
	u32 res;		
	res=FLASH->SR; 
	if(res&(1<<0))return 1;		    //æ
	else if(res&(1<<2))return 2;	//��̴���
	else if(res&(1<<4))return 3;	//д��������
	return 0;						//�������
}
//�ȴ��������
//time:Ҫ��ʱ�ĳ���
//����ֵ:״̬.
u8 STMFLASH_WaitDone(u16 time)
{
	u8 res;
	do
	{
		res=STMFLASH_GetStatus();
		if(res!=1)break;//��æ,����ȴ���,ֱ���˳�.
		delay_us(1);
		time--;
	 }while(time);
	 if(time==0)res=0xff;//TIMEOUT
	 return res;
}
//����ҳ
//paddr:ҳ��ַ
//����ֵ:ִ�����
u8 STMFLASH_ErasePage(u32 paddr)
{
	u8 res=0;
	res=STMFLASH_WaitDone(0X5FFF);//�ȴ��ϴβ�������,>20ms    
	if(res==0)
	{ 
		FLASH->CR|=1<<1;//ҳ����
		FLASH->AR=paddr;//����ҳ��ַ 
		FLASH->CR|=1<<6;//��ʼ����		  
		res=STMFLASH_WaitDone(0X5FFF);//�ȴ���������,>20ms  
		if(res!=1)//��æ
		{
			FLASH->CR&=~(1<<1);//���ҳ������־.
		}
	}
	return res;
}
//��FLASHָ����ַд�����
//faddr:ָ����ַ(�˵�ַ����Ϊ2�ı���!!)
//dat:Ҫд�������
//����ֵ:д������
u8 STMFLASH_WriteHalfWord(u32 faddr, u16 dat)
{
	u8 res;	   	    
	res=STMFLASH_WaitDone(0XFF);	 
	if(res==0)//OK
	{
		FLASH->CR|=1<<0;//���ʹ��
		*(vu16*)faddr=dat;//д������
		res=STMFLASH_WaitDone(0XFF);//�ȴ��������
		if(res!=1)//�����ɹ�
		{
			FLASH->CR&=~(1<<0);//���PGλ.
		}
	} 
	return res;
} 

//��ȡָ����ַ�İ���(16λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
   return *(uint16_t*)faddr; 
}
#if STM32_FLASH_WREN	//���ʹ����д   
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		STMFLASH_WriteHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if     STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	STMFLASH_Unlock();						//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			STMFLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain*2;	//д��ַƫ��(16λ���ݵ�ַ,��Ҫ*2)	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	STMFLASH_Lock();//����
}
#endif
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

//////////////////////////////////////////������///////////////////////////////////////////
//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
  
  STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
  
}



//��ȡ������ �󽫲����ʼ�¼
//*record_budarata �����Ĳ����� 1200 2400  4800  9600 115200
//*record_adddr ��   ���ĵ�ַ
bool Record_Buadrate_Function(uint16_t *record_budarata,uint16_t *record_adddr)
{
   uint32_t ture_addr;
   //����ṹ��
    Meter_Memory meter_memory_type;
    //�ж���Ҫд��Ĳ������ǲ����Ѿ�����
    switch(*record_budarata)
    {
      case VERY_LOW_BUADRATE  :    break;
      case LOW_BUADRATE       :    break;
      case MED_BUADRATE       :    break;
      case HIGH_BUADRATE      :    break;
      case VERY_HIGH_BUADRATE :    break;
    default: return false;
    }  
    
    //��ȫ���ж�
   DISABLE_INT(); 
    
   ture_addr=METER_CNT_BASE;
    //�����Ѿ��洢�����ݸ���
   STMFLASH_Read(ture_addr ,&meter_memory_type.record_num,1);  
   //��ʼδд���ʱ��Ϊ0XFFFF ����Ϊ0
   if(meter_memory_type.record_num==0XFFFF)
   {
     meter_memory_type.record_num=0;
   }
   //����洢ƫ�Ƶ�ַ
   ture_addr=meter_memory_type.record_num*8+METER_BASE;
   
   //����ַд�� ռ��3�����ֽ�
   STMFLASH_Write(ture_addr,record_adddr,3);
   
   ture_addr=ture_addr+6;
   
   //��������д�� ռ���ĸ��ֽ�
   
   STMFLASH_Write(ture_addr,record_budarata,1);
   
   
    //д���������+1
    meter_memory_type.record_num=meter_memory_type.record_num+1;
   //������д�� ������д��
   STMFLASH_ErasePage(METER_CNT_BASE);	//����ҳ
   
  //   HAL_Delay(2);
    //��ҳ��д��
    STMFLASH_Write(METER_CNT_BASE,&meter_memory_type.record_num,1);
   //��ȫ���ж�  
     ENABLE_INT();   
     
     return true;
     
}

////offset ƫ���� λ��
//bool Record_Change_Buadrate_Func(uint16_t *record_budarata,uint8_t *offset)
//{
//    uint32_t ture_addr;
//    uint8_t   offsent;//ƫ����
//   //����ṹ��
//    Meter_Memory meter_memory_type;
//    
//     //�ж���Ҫд��Ĳ������ǲ����Ѿ�����
//    switch(*record_budarata)
//    {
//      case VERY_LOW_BUADRATE  :    break;
//      case LOW_BUADRATE       :    break;
//      case MED_BUADRATE       :    break;
//      case HIGH_BUADRATE      :    break;
//      case VERY_HIGH_BUADRATE :    break;
//    default: return false;
//    }  
//    //����������Ϣ
//    Get_Buadrate_func();
//    //���صڼ������ݴ����˲�ͬ
//    
//    STMFLASH_Read(ture_addr ,&meter_memory_type.record_num,1);  
//    
//} 
//





////�������ʵ�����д��flash
//uint16_t Write_Buadrate_To_Flash(uint16_t *record_data,uint16_t data_length,uint16_t *record_num )
//{
//  
//
//}






























