//////////////////////////////////////////////////////////
//创建日期：2018/11/22
//创建人：szc
//文件介绍：FLASH的驱动代码
//Copyright(C)：
//////////////////////////////////////////////////////////
#include "flash.h"
//#include "app.h"
//解锁STM32的FLASH
void STMFLASH_Unlock(void)
{
  FLASH->KEYR=FLASH_KEY1;//写入解锁序列.
  FLASH->KEYR=FLASH_KEY2;
}
//flash上锁
void STMFLASH_Lock(void)
{
  FLASH->CR|=1<<7;//上锁
}
//得到FLASH状态
u8 STMFLASH_GetStatus(void)
{	
	u32 res;		
	res=FLASH->SR; 
	if(res&(1<<0))return 1;		    //忙
	else if(res&(1<<2))return 2;	//编程错误
	else if(res&(1<<4))return 3;	//写保护错误
	return 0;						//操作完成
}
//等待操作完成
//time:要延时的长短
//返回值:状态.
u8 STMFLASH_WaitDone(u16 time)
{
	u8 res;
	do
	{
		res=STMFLASH_GetStatus();
		if(res!=1)break;//非忙,无需等待了,直接退出.
		delay_us(1);
		time--;
	 }while(time);
	 if(time==0)res=0xff;//TIMEOUT
	 return res;
}
//擦除页
//paddr:页地址
//返回值:执行情况
u8 STMFLASH_ErasePage(u32 paddr)
{
	u8 res=0;
	res=STMFLASH_WaitDone(0X5FFF);//等待上次操作结束,>20ms    
	if(res==0)
	{ 
		FLASH->CR|=1<<1;//页擦除
		FLASH->AR=paddr;//设置页地址 
		FLASH->CR|=1<<6;//开始擦除		  
		res=STMFLASH_WaitDone(0X5FFF);//等待操作结束,>20ms  
		if(res!=1)//非忙
		{
			FLASH->CR&=~(1<<1);//清除页擦除标志.
		}
	}
	return res;
}
//在FLASH指定地址写入半字
//faddr:指定地址(此地址必须为2的倍数!!)
//dat:要写入的数据
//返回值:写入的情况
u8 STMFLASH_WriteHalfWord(u32 faddr, u16 dat)
{
	u8 res;	   	    
	res=STMFLASH_WaitDone(0XFF);	 
	if(res==0)//OK
	{
		FLASH->CR|=1<<0;//编程使能
		*(vu16*)faddr=dat;//写入数据
		res=STMFLASH_WaitDone(0XFF);//等待操作完成
		if(res!=1)//操作成功
		{
			FLASH->CR&=~(1<<0);//清除PG位.
		}
	} 
	return res;
} 

//读取指定地址的半字(16位数据) 
//faddr:读地址 
//返回值:对应数据.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
   return *(uint16_t*)faddr; 
}
#if STM32_FLASH_WREN	//如果使能了写   
//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		STMFLASH_WriteHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
} 
//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
#if     STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //扇区地址
	u16 secoff;	   //扇区内偏移地址(16位字计算)
	u16 secremain; //扇区内剩余地址(16位字计算)	   
 	u16 i;    
	u32 offaddr;   //去掉0X08000000后的地址
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//非法地址
	STMFLASH_Unlock();						//解锁
	offaddr=WriteAddr-STM32_FLASH_BASE;		//实际偏移地址.
	secpos=offaddr/STM_SECTOR_SIZE;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			STMFLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
			for(i=0;i<secremain;i++)//复制
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;				//扇区地址增1
			secoff=0;				//偏移位置为0 	 
		   	pBuffer+=secremain;  	//指针偏移
			WriteAddr+=secremain*2;	//写地址偏移(16位数据地址,需要*2)	   
		   	NumToWrite-=secremain;	//字节(16位)数递减
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
			else secremain=NumToWrite;//下一个扇区可以写完了
		}	 
	};	
	STMFLASH_Lock();//上锁
}
#endif
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

//////////////////////////////////////////测试用///////////////////////////////////////////
//WriteAddr:起始地址
//WriteData:要写入的数据
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
  
  STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字 
  
}



//获取波特率 后将波特率记录
//*record_budarata ：电表的波特率 1200 2400  4800  9600 115200
//*record_adddr ：   电表的地址
bool Record_Buadrate_Function(uint16_t *record_budarata,uint16_t *record_adddr)
{
   uint32_t ture_addr;
   //定义结构体
    Meter_Memory meter_memory_type;
    //判断需要写入的波特率是不是已经存在
    switch(*record_budarata)
    {
      case VERY_LOW_BUADRATE  :    break;
      case LOW_BUADRATE       :    break;
      case MED_BUADRATE       :    break;
      case HIGH_BUADRATE      :    break;
      case VERY_HIGH_BUADRATE :    break;
    default: return false;
    }  
    
    //关全局中断
   DISABLE_INT(); 
    
   ture_addr=METER_CNT_BASE;
    //读出已经存储的数据个数
   STMFLASH_Read(ture_addr ,&meter_memory_type.record_num,1);  
   //开始未写入的时候为0XFFFF 设置为0
   if(meter_memory_type.record_num==0XFFFF)
   {
     meter_memory_type.record_num=0;
   }
   //计算存储偏移地址
   ture_addr=meter_memory_type.record_num*8+METER_BASE;
   
   //将地址写入 占用3个半字节
   STMFLASH_Write(ture_addr,record_adddr,3);
   
   ture_addr=ture_addr+6;
   
   //将波特率写入 占用四个字节
   
   STMFLASH_Write(ture_addr,record_budarata,1);
   
   
    //写入的数据量+1
    meter_memory_type.record_num=meter_memory_type.record_num+1;
   //将个数写入 擦除后写入
   STMFLASH_ErasePage(METER_CNT_BASE);	//擦除页
   
  //   HAL_Delay(2);
    //将页码写入
    STMFLASH_Write(METER_CNT_BASE,&meter_memory_type.record_num,1);
   //开全局中断  
     ENABLE_INT();   
     
     return true;
     
}

////offset 偏移量 位置
//bool Record_Change_Buadrate_Func(uint16_t *record_budarata,uint8_t *offset)
//{
//    uint32_t ture_addr;
//    uint8_t   offsent;//偏移量
//   //定义结构体
//    Meter_Memory meter_memory_type;
//    
//     //判断需要写入的波特率是不是已经存在
//    switch(*record_budarata)
//    {
//      case VERY_LOW_BUADRATE  :    break;
//      case LOW_BUADRATE       :    break;
//      case MED_BUADRATE       :    break;
//      case HIGH_BUADRATE      :    break;
//      case VERY_HIGH_BUADRATE :    break;
//    default: return false;
//    }  
//    //读出所有信息
//    Get_Buadrate_func();
//    //返回第几个数据存在了不同
//    
//    STMFLASH_Read(ture_addr ,&meter_memory_type.record_num,1);  
//    
//} 
//





////将波特率等数据写入flash
//uint16_t Write_Buadrate_To_Flash(uint16_t *record_data,uint16_t data_length,uint16_t *record_num )
//{
//  
//
//}






























