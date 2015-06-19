#include "i2c.h"

uint8_t Time_Date[7];

//初始化I2C
void I2C_RTC_Init(void)
{
  /* sEE_I2C Peripheral Disable */
  I2C_Cmd(DISABLE);

  /* sEE_I2C DeInit */
  I2C_DeInit();

  /*!< sEE_I2C Peripheral clock disable */
  CLK_PeripheralClockConfig(sEE_I2C_CLK, DISABLE);

  /*!< GPIO configuration */
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, sEE_I2C_SCL_PIN, GPIO_MODE_IN_PU_NO_IT);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, sEE_I2C_SDA_PIN, GPIO_MODE_IN_PU_NO_IT);
 
  /*!< sEE_I2C Peripheral clock enable */
  CLK_PeripheralClockConfig(sEE_I2C_CLK, ENABLE);
  
  /* I2C configuration */
  /* sEE_I2C Peripheral Enable */
  I2C_Cmd( ENABLE);
  /* sEE_I2C configuration after enabling it */
  I2C_Init(I2C_SPEED, I2C_SLAVE_ADDRESS7, I2C_DUTYCYCLE_2, I2C_ACK_CURR, 
           I2C_ADDMODE_7BIT, 16);
}

/*******************************************************************************
* 名称: Read_RT8563
* 功能: 读RT8563时间数据
* 形参: *pBuffer 返回读取数据指针
         index 时间-秒寄存器地址【秒, 分, 时, 日, 星期, 月, 年】
         NumByteToRead 需要读取字节数


* 返回: 无
* 说明: 该函数直接操作stm8寄存器！
         寄存器I2c_CR2 应答使能位（位2）ack=1，当stm8接受到数据后，自动发送ACK.
         不需要手动ack.

         为个在收到最后一个字节后产生一个NACK 脉冲，在读倒数第二个数据字节之后,
         必须清除ack位(ack=0).

         当接收到最后一个字节，需要重新使能ack=1.（如果不设置ack=1,循环开始后，
         收到第一个字节stm8不会发送ack,造成除每次循环第一个字节正常外，后面收到
         的数据都是0xFF....stm8一直不给ack.
           
         需要注意的是 寄存器有些位，只要读寄存器就可以清除（不需要专门写入寄存器，
         在仿真的时候，最好不要打开寄存器页面）.
******************************************************************************/
void Read_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToWrite) 
{  
  
  while(I2C->SR3 & 0x02);  //等待总线空闲   检测i2c-SR3 busy位  
    
  //以下见stm8s中文数据手册P251（图96主设备发送模式发送序列图）
  
  //S 起始条件
  I2C->CR2 |= 0x01;  //产生起始位            CR2 start位       

  //EV5：SB=1，读SR1 然后将地址写入DR寄存器将清除该标志。
  while(!(I2C->SR1 & 0x01));  //等待START发送完 E5
 
  //ADDRESS (发送模式)
  I2C->DR = 0xA2;  //发送MLX90615器件地址(最后一位是0,表示发送)
  
  while(!(I2C->SR1 & 0x02));  //等特7位器件地址发送完并且收到ack,ADDR置1
  
  //EV6:ADDR 在软件读取SR1后，对SR3寄存器读操作 将清除改位
  I2C->SR1; //见P251 读SR1 (实验证明可以不要)
  I2C->SR3; //然后读SR3 清  ADDR（等于库函数I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED)）

  //DATA 发送寄存器地址
  I2C->DR = (uint8_t)(index); 
  
  //EV8_2 TxE=1 ，BTF=1，产生停止条件时由硬件清除。
  while(!(I2C->SR1 & 0x84));  //检测SR1 TXE1 BTF位置（只有当stm8收到ack,TxE才会置1，其实这句相当于判断收到ack没有？）
    
  //在发送地址和清除ADDR 之后，I2C接口进入主设备接收模式。以下见stm8s中文数据手册P252（图97主设备接收模式接收序列图）
  
  //S 重复起始条件
  I2C->CR2 |= 0x01;  //产生重复起始位

  //EV5：SB=1，读SR1 然后将地址写入DR寄存器将清除该标志。
  while(!(I2C->SR1 & 0x01));  //等待START发送完
  
  //ADDRESS (接收)
  I2C->DR = 0xA3;  //发送MLX90615器件地址(最后一位是1,表示接收)，发送完后自动发送ack(提前是CR2 ack位使能)

  //EV6:ADDR 在软件读取SR1后，对SR3寄存器读操作 将清除改位
  while(!(I2C->SR1 & 0x02));  //等特7位器件地址发送完并且收到ack,ADDR置1

  I2C->SR1; //见P251 读SR1 (实验证明可以不要)
  I2C->SR3; //然后读SR3 清  ADDR（等于库函数I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED)） 

  //循环读取数据
  while(NumByteToWrite)  
  {
    //EV7_1 ：RxNE=1 ，读DR寄存器清除该标志。设置ACK=0和STOP 请求。(在接收最后一个字节前) 
    if(NumByteToWrite == 1) //实验证明在最后一个字节前后都一样
    { 
      I2C->CR2 &= ~0x04; //ack使能     
      I2C->CR2 |= 0x02;  //停止位产生stop
    } 
    
    ///测试EV7 RxNE=1（收到一个字节后RxNE置1） ，判断DR寄存器有数据
    if(I2C->SR1 & 0x40)
    {    
      *pBuffer=I2C->DR;//在接收模式下，收到完整字节后,自动发送ack(提前是CR2 ack位使能，不需要专门CR2 ack位置1)
      //在风驰里面例子，在每次收到字节后加I2C_AcknowledgeConfig(I2C_ACK_CURR)无任何意义，
      pBuffer++;  
      NumByteToWrite--;     
    }      
  }
   
  I2C->CR2 |= 0x04;//为一下循环开始 设置 ack使能，上面 EV7_1设置ack=0发送stop后;需要手动设置ack=1使能，必要在接收数据之前

  //切记！切记！很多例子都没有加上这句，包括风驰 的例子！！ 不过 他没有加循环! 如果他在循环一次就会出现问题。CR2 ack位其实就是使能的意思！！很多人都理解成需要手动设置！
  // 都是软件模拟I2C 搞太多！！ 想当然啦！没有仔细看官方的文档！
}

/*******************************************************************************
* 名称: Set_RT8563
* 功能: 设置RT8563时间数据
* 形参: *pBuffer 设置参数数据指针
         index 时间-秒寄存器地址【秒, 分, 时, 日, 星期, 月, 年】
         NumByteToWrite 需要写入字节数


* 返回: 无
* 说明: 该函数直接操作stm8寄存器！
         寄存器I2c_CR2 应答使能位（位2）ack=1，当stm8接受到数据后，自动发送ACK.
         不需要手动ack.

         为个在收到最后一个字节后产生一个NACK 脉冲，在读倒数第二个数据字节之后,
         必须清除ack位(ack=0).

         当接收到最后一个字节，需要重新使能ack=1.（如果不设置ack=1,循环开始后，
         收到第一个字节stm8不会发送ack,造成除每次循环第一个字节正常外，后面收到
         的数据都是0xFF....stm8一直不给ack.
           
         需要注意的是 寄存器有些位，只要读寄存器就可以清除（不需要专门写入寄存器，
         在仿真的时候，最好不要打开寄存器页面）.
******************************************************************************/
void Set_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToRead) 
{  
  
  while(I2C->SR3 & 0x02);  //等待总线空闲   检测i2c-SR3 busy位  
    
  //以下见stm8s中文数据手册P251（图96主设备发送模式发送序列图）
  
  //S 起始条件
  I2C->CR2 |= 0x01;  //产生起始位            CR2 start位       

  //EV5：SB=1，读SR1 然后将地址写入DR寄存器将清除该标志。
  while(!(I2C->SR1 & 0x01));  //等待START发送完 E5
 
  //ADDRESS (发送模式)
  I2C->DR = 0xA2;  //发送RT8563器件地址(最后一位是0,表示发送)
  
  while(!(I2C->SR1 & 0x02));  //等特7位器件地址发送完并且收到ack,ADDR置1
  
  //EV6:ADDR 在软件读取SR1后，对SR3寄存器读操作 将清除改位
  I2C->SR1; //见P251 读SR1 (实验证明可以不要)
  I2C->SR3; //然后读SR3 清  ADDR（等于库函数I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED)）

  //DATA 发送寄存器地址
  I2C->DR = (uint8_t)(index); 
  
  //EV8_2 TxE=1 ，BTF=1，产生停止条件时由硬件清除。
  //检测SR1 TXE1 BTF位置（只有当stm8收到ack,TxE才会置1，其实这句相当于判断收到ack没有？）
  while(!(I2C->SR1 & 0x84));  
  
 
  //循环写数据
  while(NumByteToRead)  
  {    
    I2C->DR = *pBuffer;         //发送数据
    while(!(I2C->SR1 & 0x04));  //等待发送完成
    pBuffer++;
    NumByteToRead--; 
  }
  
  I2C->CR2 |= 0x02;  //停止位产生stop

  (void)sEE_I2C->SR1;
  (void)sEE_I2C->SR3;  //然后读SR3 清  ADDR
}
