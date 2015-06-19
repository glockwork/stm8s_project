#include "i2c.h"

uint8_t Time_Date[7];

//��ʼ��I2C
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
* ����: Read_RT8563
* ����: ��RT8563ʱ������
* �β�: *pBuffer ���ض�ȡ����ָ��
         index ʱ��-��Ĵ�����ַ����, ��, ʱ, ��, ����, ��, �꡿
         NumByteToRead ��Ҫ��ȡ�ֽ���


* ����: ��
* ˵��: �ú���ֱ�Ӳ���stm8�Ĵ�����
         �Ĵ���I2c_CR2 Ӧ��ʹ��λ��λ2��ack=1����stm8���ܵ����ݺ��Զ�����ACK.
         ����Ҫ�ֶ�ack.

         Ϊ�����յ����һ���ֽں����һ��NACK ���壬�ڶ������ڶ��������ֽ�֮��,
         �������ackλ(ack=0).

         �����յ����һ���ֽڣ���Ҫ����ʹ��ack=1.�����������ack=1,ѭ����ʼ��
         �յ���һ���ֽ�stm8���ᷢ��ack,��ɳ�ÿ��ѭ����һ���ֽ������⣬�����յ�
         �����ݶ���0xFF....stm8һֱ����ack.
           
         ��Ҫע����� �Ĵ�����Щλ��ֻҪ���Ĵ����Ϳ������������Ҫר��д��Ĵ�����
         �ڷ����ʱ����ò�Ҫ�򿪼Ĵ���ҳ�棩.
******************************************************************************/
void Read_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToWrite) 
{  
  
  while(I2C->SR3 & 0x02);  //�ȴ����߿���   ���i2c-SR3 busyλ  
    
  //���¼�stm8s���������ֲ�P251��ͼ96���豸����ģʽ��������ͼ��
  
  //S ��ʼ����
  I2C->CR2 |= 0x01;  //������ʼλ            CR2 startλ       

  //EV5��SB=1����SR1 Ȼ�󽫵�ַд��DR�Ĵ���������ñ�־��
  while(!(I2C->SR1 & 0x01));  //�ȴ�START������ E5
 
  //ADDRESS (����ģʽ)
  I2C->DR = 0xA2;  //����MLX90615������ַ(���һλ��0,��ʾ����)
  
  while(!(I2C->SR1 & 0x02));  //����7λ������ַ�����겢���յ�ack,ADDR��1
  
  //EV6:ADDR �������ȡSR1�󣬶�SR3�Ĵ��������� �������λ
  I2C->SR1; //��P251 ��SR1 (ʵ��֤�����Բ�Ҫ)
  I2C->SR3; //Ȼ���SR3 ��  ADDR�����ڿ⺯��I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED)��

  //DATA ���ͼĴ�����ַ
  I2C->DR = (uint8_t)(index); 
  
  //EV8_2 TxE=1 ��BTF=1������ֹͣ����ʱ��Ӳ�������
  while(!(I2C->SR1 & 0x84));  //���SR1 TXE1 BTFλ�ã�ֻ�е�stm8�յ�ack,TxE�Ż���1����ʵ����൱���ж��յ�ackû�У���
    
  //�ڷ��͵�ַ�����ADDR ֮��I2C�ӿڽ������豸����ģʽ�����¼�stm8s���������ֲ�P252��ͼ97���豸����ģʽ��������ͼ��
  
  //S �ظ���ʼ����
  I2C->CR2 |= 0x01;  //�����ظ���ʼλ

  //EV5��SB=1����SR1 Ȼ�󽫵�ַд��DR�Ĵ���������ñ�־��
  while(!(I2C->SR1 & 0x01));  //�ȴ�START������
  
  //ADDRESS (����)
  I2C->DR = 0xA3;  //����MLX90615������ַ(���һλ��1,��ʾ����)����������Զ�����ack(��ǰ��CR2 ackλʹ��)

  //EV6:ADDR �������ȡSR1�󣬶�SR3�Ĵ��������� �������λ
  while(!(I2C->SR1 & 0x02));  //����7λ������ַ�����겢���յ�ack,ADDR��1

  I2C->SR1; //��P251 ��SR1 (ʵ��֤�����Բ�Ҫ)
  I2C->SR3; //Ȼ���SR3 ��  ADDR�����ڿ⺯��I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED)�� 

  //ѭ����ȡ����
  while(NumByteToWrite)  
  {
    //EV7_1 ��RxNE=1 ����DR�Ĵ�������ñ�־������ACK=0��STOP ����(�ڽ������һ���ֽ�ǰ) 
    if(NumByteToWrite == 1) //ʵ��֤�������һ���ֽ�ǰ��һ��
    { 
      I2C->CR2 &= ~0x04; //ackʹ��     
      I2C->CR2 |= 0x02;  //ֹͣλ����stop
    } 
    
    ///����EV7 RxNE=1���յ�һ���ֽں�RxNE��1�� ���ж�DR�Ĵ���������
    if(I2C->SR1 & 0x40)
    {    
      *pBuffer=I2C->DR;//�ڽ���ģʽ�£��յ������ֽں�,�Զ�����ack(��ǰ��CR2 ackλʹ�ܣ�����Ҫר��CR2 ackλ��1)
      //�ڷ���������ӣ���ÿ���յ��ֽں��I2C_AcknowledgeConfig(I2C_ACK_CURR)���κ����壬
      pBuffer++;  
      NumByteToWrite--;     
    }      
  }
   
  I2C->CR2 |= 0x04;//Ϊһ��ѭ����ʼ ���� ackʹ�ܣ����� EV7_1����ack=0����stop��;��Ҫ�ֶ�����ack=1ʹ�ܣ���Ҫ�ڽ�������֮ǰ

  //�мǣ��мǣ��ܶ����Ӷ�û�м�����䣬������� �����ӣ��� ���� ��û�м�ѭ��! �������ѭ��һ�ξͻ�������⡣CR2 ackλ��ʵ����ʹ�ܵ���˼�����ܶ��˶�������Ҫ�ֶ����ã�
  // �������ģ��I2C ��̫�࣡�� �뵱Ȼ����û����ϸ���ٷ����ĵ���
}

/*******************************************************************************
* ����: Set_RT8563
* ����: ����RT8563ʱ������
* �β�: *pBuffer ���ò�������ָ��
         index ʱ��-��Ĵ�����ַ����, ��, ʱ, ��, ����, ��, �꡿
         NumByteToWrite ��Ҫд���ֽ���


* ����: ��
* ˵��: �ú���ֱ�Ӳ���stm8�Ĵ�����
         �Ĵ���I2c_CR2 Ӧ��ʹ��λ��λ2��ack=1����stm8���ܵ����ݺ��Զ�����ACK.
         ����Ҫ�ֶ�ack.

         Ϊ�����յ����һ���ֽں����һ��NACK ���壬�ڶ������ڶ��������ֽ�֮��,
         �������ackλ(ack=0).

         �����յ����һ���ֽڣ���Ҫ����ʹ��ack=1.�����������ack=1,ѭ����ʼ��
         �յ���һ���ֽ�stm8���ᷢ��ack,��ɳ�ÿ��ѭ����һ���ֽ������⣬�����յ�
         �����ݶ���0xFF....stm8һֱ����ack.
           
         ��Ҫע����� �Ĵ�����Щλ��ֻҪ���Ĵ����Ϳ������������Ҫר��д��Ĵ�����
         �ڷ����ʱ����ò�Ҫ�򿪼Ĵ���ҳ�棩.
******************************************************************************/
void Set_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToRead) 
{  
  
  while(I2C->SR3 & 0x02);  //�ȴ����߿���   ���i2c-SR3 busyλ  
    
  //���¼�stm8s���������ֲ�P251��ͼ96���豸����ģʽ��������ͼ��
  
  //S ��ʼ����
  I2C->CR2 |= 0x01;  //������ʼλ            CR2 startλ       

  //EV5��SB=1����SR1 Ȼ�󽫵�ַд��DR�Ĵ���������ñ�־��
  while(!(I2C->SR1 & 0x01));  //�ȴ�START������ E5
 
  //ADDRESS (����ģʽ)
  I2C->DR = 0xA2;  //����RT8563������ַ(���һλ��0,��ʾ����)
  
  while(!(I2C->SR1 & 0x02));  //����7λ������ַ�����겢���յ�ack,ADDR��1
  
  //EV6:ADDR �������ȡSR1�󣬶�SR3�Ĵ��������� �������λ
  I2C->SR1; //��P251 ��SR1 (ʵ��֤�����Բ�Ҫ)
  I2C->SR3; //Ȼ���SR3 ��  ADDR�����ڿ⺯��I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED)��

  //DATA ���ͼĴ�����ַ
  I2C->DR = (uint8_t)(index); 
  
  //EV8_2 TxE=1 ��BTF=1������ֹͣ����ʱ��Ӳ�������
  //���SR1 TXE1 BTFλ�ã�ֻ�е�stm8�յ�ack,TxE�Ż���1����ʵ����൱���ж��յ�ackû�У���
  while(!(I2C->SR1 & 0x84));  
  
 
  //ѭ��д����
  while(NumByteToRead)  
  {    
    I2C->DR = *pBuffer;         //��������
    while(!(I2C->SR1 & 0x04));  //�ȴ��������
    pBuffer++;
    NumByteToRead--; 
  }
  
  I2C->CR2 |= 0x02;  //ֹͣλ����stop

  (void)sEE_I2C->SR1;
  (void)sEE_I2C->SR3;  //Ȼ���SR3 ��  ADDR
}
