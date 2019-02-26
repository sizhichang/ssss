#include "tool.h"

/*******************************************************************
function: compare two string then cout(output) result (false OR true)
param:  cin  sourceChar  want to compare with want char  cout��index find the char head
*********************************************************************/

bool FindString(char * wantChar,char* sourceChar,uint16_t * index,uint32_t length)
{
    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t wantCharCount = 0;
    
    bool flag  = false;
    
    //���ҵ����ж��ٸ��ַ�
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
    
    //��source���ҵ���û����ȫ��ͬ���ַ�
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
    
    //���ҵ����ж��ٸ��ַ�
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
    
    //��source���ҵ���û����ȫ��ͬ���ַ�
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
//  ���source_data �Ƿ���� want_data
// */
//bool Contain_data


/*
  ���sourceChar �Ƿ���� wantChar
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
  ���sourceChar�Ƿ����� wantChat ��ͷ��
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



//�������壺��ȡ firstKey �� lastKey ֮����ַ��������û��lastKey �����ȡ�����
// ����ֵ��ָ���ȡ���ַ������׵�ַ
//datain :�����ַ������׵�ַ ��   count :�����ַ����ĳ���    firstKey �������1 �� lastKey ,�����2 �� length ����������ݵĳ���
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
  ���ַ���ת��Ϊ�ֽ����飬����ֽ�����Ϊ�������������а�������16�������ݵ��ַ����򷵻ش�����ȷ�����ת��
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
  ���ֽ�����ת��Ϊ�ַ�����
*/
void ConvertHexToString(char * dest,uint8_t * sourceByte,uint16_t count)
{
    uint16_t i = 0;  
    uint8_t value = 0;   //ʮλ
    
    
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

//��dest ������ source֮�󣬱�֤source���㹻�Ŀռ�
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

//���ַ�ת��Ϊ����   "16" => 16
bool ConvertHexStringToInt(uint8_t * data, uint8_t count,uint16_t * outData)
{
    uint8_t i = 0;
    uint8_t j = 0;
    
    *outData = 0;
    
    //����в������ֵ��ַ�����false;
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

//��16λ�ֽ�Ϊ8λ
uint8_t * Data_Resolve_16Bit_To_8Bit(uint16_t *cin_data,uint8_t resolve_length)
{
  uint8_t i,j;
  uint8_t *data_pt;
  //�����ںϻ�����
  static uint8_t data_resolve_buf[8];
    //�����
   for(i=0;i<resolve_length*2;i++)
  {  
       data_resolve_buf[i]=0;
  }
  j=0;
  //�ֽ�����
    for(i=0;i<resolve_length;i++)
    {
      data_resolve_buf[j+1]=  (uint8_t)cin_data[i];
      data_resolve_buf[j]=(uint8_t)(cin_data[i]>>8);
       j=j+2;
    }   
      //��������ָ��
     data_pt=data_resolve_buf; 
    return data_pt;  
}
//����ַ�����ں� 
//����ֵ ��uint16_t ����ָ��
//fuse_length ������ֵ����Ϊż��  assert����
uint16_t * Data_Fuse_8Bit_To_16Bit(uint8_t *data,uint8_t fuse_length)
{
    uint8_t i,j;
    uint16_t *data_pt;
    //�����ںϻ�����
    static uint16_t data_fuse_buf[8];
      //�����
     for(i=0;i<fuse_length/2;i++)
    {  
         data_fuse_buf[i]=0;
    }

    i=0;

    //�ں�����
    for(j=0;j<fuse_length/2;j++)
    {
       data_fuse_buf[j]=data[i]<<8|data[i+1];
       i=i+2;   
    }   
      //��������ָ��
     data_pt=data_fuse_buf; 
    return data_pt;
}


//��BCD��ת����Hex,�����ֽ�
uint8_t ConvertBcd_ToHex(uint8_t data)
{
    uint8_t value=0;
    value += data&0x0f;
    value += ((data>>4)&0x0f)*10;
    return value;
    
}

//��Hexת����BCD��
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

//����������ƫ�Ƶ�ַ
//NVIC_VectTab:��ַ
//Offset:ƫ����		 
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab,u32 Offset)	 
{ 	   	  
	SCB->VTOR=NVIC_VectTab|(Offset&(u32)0xFFFFFE00);//����NVIC��������ƫ�ƼĴ���,VTOR��9λ����,��[8:0]������
}
//����NVIC����
//NVIC_Group:NVIC���� 0~4 �ܹ�5�� 		   
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//ȡ����λ
	temp1<<=8;
	temp=SCB->AIRCR;  //��ȡ��ǰ������
	temp&=0X0000F8FF; //�����ǰ����
	temp|=0X05FA0000; //д��Կ��
	temp|=temp1;	   
	SCB->AIRCR=temp;  //���÷���	    	  				   
}
//����NVIC 
//NVIC_PreemptionPriority:��ռ���ȼ�
//NVIC_SubPriority       :��Ӧ���ȼ�
//NVIC_Channel           :�жϱ��
//NVIC_Group             :�жϷ��� 0~4
//ע�����ȼ����ܳ����趨����ķ�Χ!����������벻���Ĵ���
//�黮��:
//��0:0λ��ռ���ȼ�,4λ��Ӧ���ȼ�
//��1:1λ��ռ���ȼ�,3λ��Ӧ���ȼ�
//��2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�
//��3:3λ��ռ���ȼ�,1λ��Ӧ���ȼ�
//��4:4λ��ռ���ȼ�,0λ��Ӧ���ȼ�
//NVIC_SubPriority��NVIC_PreemptionPriority��ԭ����,��ֵԽС,Խ����	   
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	  
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//���÷���
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;								//ȡ����λ
	NVIC->ISER[NVIC_Channel/32]|=1<<NVIC_Channel%32;//ʹ���ж�λ(Ҫ����Ļ�,����ICER��ӦλΪ1����)
	NVIC->IP[NVIC_Channel]|=temp<<4;				//������Ӧ���ȼ����������ȼ�   	    	  				   
} 




//��ʱnus
//nusΪҪ��ʱ��us��.	
//ע��:nus��ֵ,��Ҫ����798915us(���ֵ��2^24/fac_us@fac_us=21)
void delay_us(u32 nus)
{		
    u32 temp;	    	 
    SysTick->LOAD=nus*fac_us; 				//ʱ�����	  		 
    SysTick->VAL=0x00;        				//��ռ�����
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //��ʼ���� 	 
    do
    {
      temp=SysTick->CTRL;
    }while((temp&0x01)&&!(temp&(1<<16)));	//�ȴ�ʱ�䵽��   
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //�رռ�����
    SysTick->VAL =0X00;       				//��ռ����� 
}
void delay_ms(u16 nms)
{		
  delay_us((u32)(nms*1000));				//��ͨ��ʽ��ʱ
}


//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	asm("WFI");		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
void INTX_DISABLE(void)
{		
    asm("CPSID   I");	
    asm("BX      LR");	
}
//���������ж�
void INTX_ENABLE(void)
{
    asm("CPSIE   I");	
    asm("BX      LR");	
}
//����ջ����ַ
//addr:ջ����ַ
void MSR_MSP(u32 addr) 
{
    asm("MSR MSP, r0");	//set Main Stack value
    asm("BX r14");	
}
