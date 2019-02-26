#include "tool.h"

/*******************************************************************
function: compare two string then cout(output) result (false OR true)
param:  cin  sourceChar  want to compare with want char  cout：index find the char head
*********************************************************************/

bool FindString(char * wantChar,char* sourceChar,uint16_t * index,uint32_t length)
{
    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t wantCharCount = 0;
    
    bool flag  = false;
    
    //先找到共有多少个字符
    for(i=0;;i++)
    {
        if(wantChar[i]==0)
        {
          wantCharCount = i;
          break;
        }
    }
    
    if(wantCharCount == 0)
      return false;
    
    
    if(wantCharCount==0)
      return false;
    
    //从source中找到有没有完全相同的字符
    for(i=0;i<length;i++)
    {
        if(wantChar[j] == sourceChar[i])
        {
            flag = true;
            
            for(j=0;j<wantCharCount;j++)
            {
                if(wantChar[j]!=sourceChar[i+j])
                {
                    flag = false;
                    j = 0;
                    break;
                }
            }
            
            if(flag)
            {
                *index = i;
                return flag;
            }
              
        }
        
//        if(sourceChar[i] == 0)
//          break;
    }
    
    return false;
}


bool ContainsString(char * wantChar ,char * sourceChar,uint32_t length)
{
  
    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t wantCharCount = 0;
    
    bool flag  = false;
    
    //先找到共有多少个字符
    for(i=0;;i++)
    {
        if(wantChar[i]==0)
        {
          wantCharCount = i;
          break;
        }
    }
    
    if(wantCharCount == 0)
      return false;
    
    
    if(wantCharCount==0)
      return false;
    
    //从source中找到有没有完全相同的字符
    for(i=0;i<length;i++)
    {
        if(wantChar[j] == sourceChar[i])
        {
            flag = true;
            
            for(j=0;j<wantCharCount;j++)
            {
                if(wantChar[j]!=sourceChar[i+j])
                {
                    flag = false;
                    j = 0;
                    break;
                }
            }
            
            if(flag)
              return flag;
        }
        
//        if(sourceChar[i] == 0)
//          break;
    }
    
    return false;
    
}
///*
//  检查source_data 是否包含 want_data
// */
//bool Contain_data


/*
  检查sourceChar 是否包含 wantChar
 */
bool ContanisStrings(char * wantChar1,char * wantChar2,char* wantChar3 ,char * sourceChar ,uint8_t * index,uint32_t length)
{
    bool flag = false;
    
    * index = 0;    
    flag = ContainsString(wantChar1,sourceChar,length);
    
    if(flag)    
        return true;
    
    *index = 1;
    flag = ContainsString(wantChar2,sourceChar,length);
    
    if(flag)
      return true;
    
    *index  = 2;
    flag = ContainsString(wantChar3,sourceChar,length);
    
    return flag;
    
}

/*
  检查sourceChar是否是以 wantChat 开头的
 */
bool StartWith(char * wantChar,char * sourceChar)
{
    uint16_t i = 0;
    
    for(i=0;;i++)
    {
        if(wantChar[i]==0)
          return true;
        if(wantChar[i]!=sourceChar[i])
          return false;
    }
}



//函数意义：截取 firstKey 和 lastKey 之间的字符串，如果没有lastKey ，则截取到最后
// 返回值：指向截取的字符串的首地址
//datain :传入字符串的首地址 ，   count :传入字符串的长度    firstKey ，间隔符1 ， lastKey ,间隔符2 ， length ，输出的数据的长度
uint8_t * GetStringFromKeyWords(uint8_t * datain,uint16_t count,char firstKey,char lastKey,uint16_t * length)
{
    int32_t firstKeyIndex = -1;
    int32_t lastKeyIndex = -1;
    uint8_t * outData = (uint8_t *)0;
    
    uint16_t i = 0;
    
    for(i=0;i<count;i++)
    {
        if((datain[i] == firstKey)&&(firstKeyIndex==-1))
            firstKeyIndex = i;
        if((datain[i] == lastKey)&&(lastKeyIndex==-1))
            lastKeyIndex = i;
    }
    
    if((firstKeyIndex == -1)||((firstKeyIndex+1)>=count))
        return outData;
    
    outData = &datain[firstKeyIndex+1];
    
    if(lastKeyIndex==-1)
        lastKeyIndex = count;
    
    * length = lastKeyIndex - firstKeyIndex-1;               
    
    return outData;
    
}

uint8_t * GetFirstStringFromKeyWord(uint8_t * datain,uint16_t count,char split,uint16_t * length,bool containKey)
{
    uint8_t * outData = (uint8_t *)0;
    uint16_t i= 0;
    int32_t index = -1;
    
    for(i = 0;i<count;i++)
    {
        if((datain[i]==split)&&(index==-1))
            index = i;
    }
    
    if(index == -1)
        return outData;
    
    outData = datain;
    
    if(containKey)    
        * length = index+1;    
    else
        * length = index;
    
    return outData;
    
}

uint8_t * GetLastStringFromKeyWord(uint8_t * datain,uint16_t count,char split,uint16_t * length,bool containKey)
{
    uint8_t * outData = (uint8_t *)0;
    uint16_t i= 0;
    int32_t index = -1;
    
    for(i = 0;i<count;i++)
    {
        if((datain[i]==split)&&(index==-1))
            index = i;
    }
    
    if(index == -1)
        return outData;
    
    
    
    if(containKey)    
    {
        outData = &datain[index];
        * length = count - index;    
    }
        
    else
    {
        outData = &datain[index+1];
        * length = count - index - 1;
    }
        
    
    return outData;
    
}

/*
  将字符串转化为字节数组，如果字节数组为单数，或者其中包含不是16进制数据的字符，则返回错误，正确则进行转换
*/
bool ConvertStringToHex(uint8_t * dest, char * sourceChar)
{
    uint16_t  i = 0;
    
    for(i=0;;i++)
    {
          if(sourceChar[i]==0)
              break;  
          if(!(((sourceChar[i]>='0')&&(sourceChar[i]<='9'))||((sourceChar[i]>='A')&&(sourceChar[i]<='F'))))
                return false;
        
    }
    
    if(i%2!=0)
      return false;
    
    i= 0;
    
    while(sourceChar[2*i]!=0)
    {
        if((sourceChar[2*i]>='0')&&(sourceChar[2*i]<='9'))
          dest[i] += (sourceChar[2*i]-0x30)*16;
        else
          dest[i] += (sourceChar[2*i]-55)*16;
            
        if((sourceChar[2*i+1]>='0')&&(sourceChar[2*i+1]<='9'))
          dest[i] += sourceChar[2*i+1]-0x30;
        else
          dest[i] += sourceChar[2*i+1]-55;
        
        i++;
    }
    
    return true;
    
}

/*
  将字节数组转化为字符串，
*/
void ConvertHexToString(char * dest,uint8_t * sourceByte,uint16_t count)
{
    uint16_t i = 0;  
    uint8_t value = 0;   //十位
    
    
    for(i=0;i<count;i++)
    {
        value = sourceByte[i]/16;
        
        if(value>=10)
          dest[2*i] = value+55;
        else
          dest[2*i] = value+0x30;
        
        value = sourceByte[i]%16;
        
        if(value>=10)
          dest[2*i+1] = value+55;
        else
          dest[2*i+1] = value+0x30;        
    }
}


void copyTo(uint8_t * source, uint8_t * dest, uint16_t count)
{
    uint16_t i = 0;
    
    for(i=0;i<count;i++)
    {
        dest[i] = source[i];
    }    
}

void ConvertHexToChar(uint16_t  source,char * dest )
{
    uint8_t numCount = 0;
    uint16_t count = source;
    while(1)
    {
       
        
        if(source/10>0)
        {
            numCount++;
        }
        
        if(source/10==0)
        {            
            numCount++;  
            break;            
        }
        
         source/=10;
    }
    
    while(1)
    {
        dest[numCount-1] = count%10+0x30;
        count = count/10;
        
        if(numCount-1 == 0)
          break;
        numCount--;
    }
}

//将dest 链接在 source之后，保证source有足够的空间
void ConnactString(char * source,char * dest)
{
    uint16_t i = 0;
    uint16_t j = 0;
    
    for(i=0;;i++)
    {
        if(source[i]==0)
          break;
    }
    
    for(j=0;;j++)
    {
        if(dest[j]==0)
          break;
        source[i+j] = dest[j];
    }
        
}

void ConnactLengthString(char * source,char * dest,uint16_t length)
{
    uint16_t i = 0;
    uint16_t j = 0;
    
    for(i=0;;i++)
    {
        if(source[i]==0)
          break;
    }
    
    for(j=0;j<length;j++)
    {
        source[i+j] = dest[j];
    }
}

//将字符转化为正数   "16" => 16
bool ConvertHexStringToInt(uint8_t * data, uint8_t count,uint16_t * outData)
{
    uint8_t i = 0;
    uint8_t j = 0;
    
    *outData = 0;
    
    //如果有不是数字的字符返回false;
    for(i=0;i<count;i++)
    {
        if((data[i]>'9')&&(data[i]<'0'))
          return false;
    }
   
   
    
    for(j=0;j<count;j++)
    {
       
        *outData = (* outData)*10;
        *outData += data[j]-0x30;
    }
    
    return true;
}


void ReserveCache(uint8_t * data,uint16_t count)
{
    uint8_t  cache;
    uint16_t i = 0;
    
    uint16_t j = count/2;
    
    for(i=0;i<j;i++)
    {
        cache = data[i] ;
        data[i] = data[count-1-i];
        data[count-1-i] = cache;
    }
}

//将16位分解为8位
uint8_t * Data_Resolve_16Bit_To_8Bit(uint16_t *cin_data,uint8_t resolve_length)
{
  uint8_t i,j;
  uint8_t *data_pt;
  //数据融合缓冲区
  static uint8_t data_resolve_buf[8];
    //先清除
   for(i=0;i<resolve_length*2;i++)
  {  
       data_resolve_buf[i]=0;
  }
  j=0;
  //分解数据
    for(i=0;i<resolve_length;i++)
    {
      data_resolve_buf[j+1]=  (uint8_t)cin_data[i];
      data_resolve_buf[j]=(uint8_t)(cin_data[i]>>8);
       j=j+2;
    }   
      //返回数据指针
     data_pt=data_resolve_buf; 
    return data_pt;  
}
//将地址数据融合 
//返回值 ：uint16_t 类型指针
//fuse_length 输入数值必须为偶数  assert函数
uint16_t * Data_Fuse_8Bit_To_16Bit(uint8_t *data,uint8_t fuse_length)
{
    uint8_t i,j;
    uint16_t *data_pt;
    //数据融合缓冲区
    static uint16_t data_fuse_buf[8];
      //先清除
     for(i=0;i<fuse_length/2;i++)
    {  
         data_fuse_buf[i]=0;
    }

    i=0;

    //融合数据
    for(j=0;j<fuse_length/2;j++)
    {
       data_fuse_buf[j]=data[i]<<8|data[i+1];
       i=i+2;   
    }   
      //返回数据指针
     data_pt=data_fuse_buf; 
    return data_pt;
}


//将BCD码转换成Hex,单个字节
uint8_t ConvertBcd_ToHex(uint8_t data)
{
    uint8_t value=0;
    value += data&0x0f;
    value += ((data>>4)&0x0f)*10;
    return value;
    
}

//将Hex转换成BCD码
uint8_t ConvertHex_ToBcd(uint8_t data)
{
     uint8_t value = 0;
    uint8_t _1x = 0;
    uint8_t _0x = 0;
    
    
    _1x = data/10;
    _0x = data%10;
    
    value = _1x<<4;
    value|=_0x;
    return value;
}

void MemSet(uint8_t * source ,uint8_t data,uint16_t count)
{
    uint16_t i = 0;
    
    for(i=0;i<count;i++)
    {
        source[i] = data;
    }
    
}

uint32_t GetCharsLength(char * data)
{
	uint32_t length = 0;
	while(1)
	{
		if(data[length]!=0)
			length++;
		else
			break;
	}
	return length;
}
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
/************************************************************/
static u32 fac_us;

//设置向量表偏移地址
//NVIC_VectTab:基址
//Offset:偏移量		 
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab,u32 Offset)	 
{ 	   	  
	SCB->VTOR=NVIC_VectTab|(Offset&(u32)0xFFFFFE00);//设置NVIC的向量表偏移寄存器,VTOR低9位保留,即[8:0]保留。
}
//设置NVIC分组
//NVIC_Group:NVIC分组 0~4 总共5组 		   
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//取后三位
	temp1<<=8;
	temp=SCB->AIRCR;  //读取先前的设置
	temp&=0X0000F8FF; //清空先前分组
	temp|=0X05FA0000; //写入钥匙
	temp|=temp1;	   
	SCB->AIRCR=temp;  //设置分组	    	  				   
}
//设置NVIC 
//NVIC_PreemptionPriority:抢占优先级
//NVIC_SubPriority       :响应优先级
//NVIC_Channel           :中断编号
//NVIC_Group             :中断分组 0~4
//注意优先级不能超过设定的组的范围!否则会有意想不到的错误
//组划分:
//组0:0位抢占优先级,4位响应优先级
//组1:1位抢占优先级,3位响应优先级
//组2:2位抢占优先级,2位响应优先级
//组3:3位抢占优先级,1位响应优先级
//组4:4位抢占优先级,0位响应优先级
//NVIC_SubPriority和NVIC_PreemptionPriority的原则是,数值越小,越优先	   
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	  
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//设置分组
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;								//取低四位
	NVIC->ISER[NVIC_Channel/32]|=1<<NVIC_Channel%32;//使能中断位(要清除的话,设置ICER对应位为1即可)
	NVIC->IP[NVIC_Channel]|=temp<<4;				//设置响应优先级和抢断优先级   	    	  				   
} 




//延时nus
//nus为要延时的us数.	
//注意:nus的值,不要大于798915us(最大值即2^24/fac_us@fac_us=21)
void delay_us(u32 nus)
{		
    u32 temp;	    	 
    SysTick->LOAD=nus*fac_us; 				//时间加载	  		 
    SysTick->VAL=0x00;        				//清空计数器
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //开始倒数 	 
    do
    {
      temp=SysTick->CTRL;
    }while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达   
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //关闭计数器
    SysTick->VAL =0X00;       				//清空计数器 
}
void delay_ms(u16 nms)
{		
  delay_us((u32)(nms*1000));				//普通方式延时
}


//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	asm("WFI");		  
}
//关闭所有中断(但是不包括fault和NMI中断)
void INTX_DISABLE(void)
{		
    asm("CPSID   I");	
    asm("BX      LR");	
}
//开启所有中断
void INTX_ENABLE(void)
{
    asm("CPSIE   I");	
    asm("BX      LR");	
}
//设置栈顶地址
//addr:栈顶地址
void MSR_MSP(u32 addr) 
{
    asm("MSR MSP, r0");	//set Main Stack value
    asm("BX r14");	
}
