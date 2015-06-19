#include "24lc02.h"  



#define EEP_SDA_PORT   GPIOG
#define EEP_SCL_PORT   GPIOG
#define EEP_SDA_PIN    GPIO_PIN_3
#define EEP_SCL_PIN    GPIO_PIN_4


static void delayus(uint16_t time) 
{ 
    uint16_t i,j;
    for(i=0;i<time;i++)
      for(j=0;j<4;j++);
}

void At24cxx_Init(void)
{
    GPIO_Init(EEP_SDA_PORT, EEP_SDA_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
    GPIO_Init(EEP_SCL_PORT, EEP_SCL_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
    GPIO_WriteHigh(EEP_SDA_PORT, EEP_SDA_PIN);	  	  
    GPIO_WriteHigh(EEP_SCL_PORT, EEP_SCL_PIN);
    delayus(500);
}
 

static void Eep_IIC_Start(void)
{  
    
    GPIO_Init(EEP_SDA_PORT, EEP_SDA_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
    GPIO_WriteHigh(EEP_SDA_PORT, EEP_SDA_PIN);	  
    GPIO_WriteHigh(EEP_SCL_PORT, EEP_SCL_PIN);
    delayus(30);
    GPIO_WriteLow(EEP_SDA_PORT, EEP_SDA_PIN);
    delayus(30);
    GPIO_WriteLow(EEP_SCL_PORT, EEP_SCL_PIN);
  
}	  

static void Eep_IIC_Stop(void)
{
  
    GPIO_Init(EEP_SDA_PORT, EEP_SDA_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
    GPIO_WriteLow(EEP_SDA_PORT, EEP_SDA_PIN);	  	  
    GPIO_WriteLow(EEP_SCL_PORT, EEP_SCL_PIN);
    delayus(30); 
    GPIO_WriteHigh(EEP_SCL_PORT, EEP_SCL_PIN);
    GPIO_WriteHigh(EEP_SDA_PORT, EEP_SDA_PIN);	  	  
    delayus(30);						   	
}

static uint8_t Eep_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    GPIO_Init(EEP_SDA_PORT, EEP_SDA_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
    GPIO_WriteHigh(EEP_SDA_PORT, EEP_SDA_PIN);	  	
    delayus(30);	 
    GPIO_Init(EEP_SDA_PORT, EEP_SDA_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_WriteHigh(EEP_SCL_PORT, EEP_SCL_PIN);
    delayus(30);	
    while(GPIO_ReadInputPin(EEP_SDA_PORT,EEP_SDA_PIN))
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            Eep_IIC_Stop();
            return 1;
        }
    }
    GPIO_WriteLow(EEP_SCL_PORT, EEP_SCL_PIN); 	   
    return 0;  
} 


static void Eep_IIC_Ack(void)
{
    GPIO_WriteLow(EEP_SCL_PORT, EEP_SCL_PIN);
    GPIO_Init(EEP_SDA_PORT, EEP_SDA_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
    GPIO_WriteLow(EEP_SDA_PORT, EEP_SDA_PIN);	
    delayus(30);
    GPIO_WriteHigh(EEP_SCL_PORT, EEP_SCL_PIN);
    delayus(30);
    GPIO_WriteLow(EEP_SCL_PORT, EEP_SCL_PIN);
}
    
static void Eep_IIC_NAck(void)
{
    GPIO_WriteLow(EEP_SCL_PORT, EEP_SCL_PIN);
    GPIO_Init(EEP_SDA_PORT, EEP_SDA_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
    GPIO_WriteHigh(EEP_SDA_PORT, EEP_SDA_PIN);	
    delayus(30);
    GPIO_WriteHigh(EEP_SCL_PORT, EEP_SCL_PIN);
    delayus(30);
    GPIO_WriteLow(EEP_SCL_PORT, EEP_SCL_PIN);
}					 				     
		  
static void Eep_IIC_Send_Byte(uint8_t txd)
{                        
   uint8_t t;   

    GPIO_Init(EEP_SDA_PORT, EEP_SDA_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
    GPIO_WriteLow(EEP_SCL_PORT, EEP_SCL_PIN);
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>>7)
        {
            GPIO_WriteHigh(EEP_SDA_PORT, EEP_SDA_PIN);
        }
        else
        {
            GPIO_WriteLow(EEP_SDA_PORT, EEP_SDA_PIN);
        }
        txd<<=1; 	  
        delayus(30);  
        GPIO_WriteHigh(EEP_SCL_PORT, EEP_SCL_PIN);
        delayus(30); 
        GPIO_WriteLow(EEP_SCL_PORT, EEP_SCL_PIN);
        delayus(30);
    }
} 	    

static uint8_t Eep_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;

    GPIO_Init(EEP_SDA_PORT, EEP_SDA_PIN, GPIO_MODE_IN_PU_NO_IT);
    for(i=0;i<8;i++ )
    {
        GPIO_WriteLow(EEP_SCL_PORT, EEP_SCL_PIN);
        delayus(30);
        GPIO_WriteHigh(EEP_SCL_PORT, EEP_SCL_PIN);
        receive<<=1;
        if(GPIO_ReadInputPin(EEP_SDA_PORT,EEP_SDA_PIN))receive++;   
        delayus(30); 
    }					 
    if (!ack)
        Eep_IIC_NAck();
    else
        Eep_IIC_Ack(); 
    return receive;
} 



uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{				  
    uint8_t temp=0;		  	    																 
    Eep_IIC_Start();  
    if(EE_TYPE>AT24C16)
    {
        Eep_IIC_Send_Byte(0XA0);	
        Eep_IIC_Wait_Ack();
        Eep_IIC_Send_Byte(ReadAddr>>8);
        Eep_IIC_Wait_Ack();		 
    }
    else Eep_IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));    
    Eep_IIC_Wait_Ack(); 
    Eep_IIC_Send_Byte(ReadAddr%256);  
    Eep_IIC_Wait_Ack();	    
    Eep_IIC_Start();  	 	   
    Eep_IIC_Send_Byte(0XA1);           		   
    Eep_IIC_Wait_Ack();	 
    temp=Eep_IIC_Read_Byte(0);		   
    Eep_IIC_Stop();    
    return temp;
}

void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{				   	  	    																 
    Eep_IIC_Start();  
    if(EE_TYPE>AT24C16)
    {
        Eep_IIC_Send_Byte(0XA0);	    
        Eep_IIC_Wait_Ack();
        Eep_IIC_Send_Byte(WriteAddr>>8);
        Eep_IIC_Wait_Ack();		 
    }
    else
    {
       Eep_IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));    
    }	 
    Eep_IIC_Wait_Ack();	   
    Eep_IIC_Send_Byte(WriteAddr%256);   
    Eep_IIC_Wait_Ack(); 	 										  		   
    Eep_IIC_Send_Byte(DataToWrite);     						   
    Eep_IIC_Wait_Ack();  		    	   
    Eep_IIC_Stop(); 
    
    delayus(500);	

}

uint8_t AT24CXX_Check(void)
{
  uint8_t temp;
  AT24CXX_WriteOneByte(0,0x54);
  AT24CXX_WriteOneByte(0,0x54);
  temp=AT24CXX_ReadOneByte(0);	
  if(temp == 0x54)
    return 0;
  return 1;											  
}


/*********************************************************************************************************
** End of File
*********************************************************************************************************/
