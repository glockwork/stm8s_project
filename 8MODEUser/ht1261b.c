#include "ht1261b.h"
#include "main.h"

extern DATA_STATUS     DataStatus;
extern unsigned char PM_V0;
extern float   Rmax, old_Rmax;
uint8_t number[10] = {NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, 
                    NUM_5, NUM_6, NUM_7, NUM_8, NUM_9};

/*
����: void Write_ID(uint8_t ID)
����: ����ID
�β�: ID ������
����: ��
*/
void Write_ID(uint8_t ID)
{
  uint8_t i, id;
  id = ID;
  for(i = 0; i < 3; i++)
  {  
    WR_0;
    if((id << i)& 0x04) 
      DATA_1;
    else
      DATA_0;
    WR_1;
  }
}

/*
����: void Write_MemoryAddr(uint8_t Address)
����: ����Memory Address
�β�: Address �ڴ��ַ
����: ��
*/
void Write_MemoryAddr(uint8_t Address)
{
  uint8_t i, address;
  address = Address;
  for(i = 0; i < 6; i++)
  {  
    WR_0;
    if((address << i)& 0x20) 
      DATA_1;
    else
      DATA_0;
    WR_1;
  }
}

/*
����: void Write_Data(uint8_t Value)
����: ��������
�β�: Data ����
����: ��
˵����D0��D3 = COM1��COM4 = 0b0000 D0 D1 D2 D3
*/
void Write_Data(uint8_t Value)
{
  uint8_t i, data;
  data = Value;
  for(i = 0; i < 4; i++)
  {  
    WR_0;
    if((data << i)& 0x08) 
      DATA_1;
    else
      DATA_0;
    WR_1;
  }
}

/*
����: void Read_Data(void)
����: ��������
�β�: ��
����: ��ȡ��������
˵����D0��D3 = COM1��COM4 = 0b0000 D0 D1 D2 D3
*/
uint8_t Read_Data(void)
{
  uint8_t i, data = 0;
  
  DATA_IN;
  WR_1;
  for(i = 0; i < 4; i++)
  {  
    RD_0;
    data <<= 1;
    if(GPIO_ReadInputPin(GPIOB, GPIO_PIN_5)) 
      data++;
    
    RD_1;
  }
 
  return data;
}

/*
����: void Write_Command(uint8_t Command)
����: ��������
�β�: Command ����
����: ��
*/
void Write_Command(uint8_t Command)
{
  uint8_t i, command;
  command = Command;
  for(i = 0; i < 9; i++)
  {  
    WR_0;
    if((command << i)& 0x80) 
      DATA_1;
    else
      DATA_0;
    WR_1;
  }
}

/*
����: void HT1261B_WriteData(uint8_t MAddr, uint8_t Value)
����: ��HT1261B��������
�β�: MAddr �ڴ��ַ
      Data ����
����: ��
*/
void HT1261B_WriteData(uint8_t MAddr, uint8_t Value)
{
  CS_OUT;
  WR_OUT;
  DATA_OUT;
  
  CS_0;
  WR_1;
  Write_ID(WRITE_CMD);
  Write_MemoryAddr(MAddr);
  Write_Data(Value);
  WR_0;
  CS_1;
}

/*
����: uint8_t HT1261B_ReadData(uint8_t MAddr)
����: ��HT1261B��������
�β�: MAddr �ڴ��ַ, ������2����ַ�ϵ�����, MAddr, MAddr+1
����: ��ȡ��������
*/
uint8_t HT1261B_ReadData(uint8_t MAddr)
{
  uint8_t data;
  CS_OUT;
  WR_OUT;
  RD_OUT;
  DATA_OUT;
  
  CS_0;
  WR_1;
  Write_ID(READ_CMD);
  Write_MemoryAddr(MAddr);
  data = Read_Data() << 4;
  data |= Read_Data();
  CS_1;
  
  return data;
}

/*
����: void HT1261B_WriteCommand(uint8_t Command)
����: ��HT1261B��������
�β�: Command ����
      Count ���ݸ���
����: ��
*/
void HT1261B_WriteCommand(uint8_t Command)
{
  CS_OUT;
  WR_OUT;
  DATA_OUT;
  
  CS_0;
  WR_1;
  Write_ID(COMMAND_CMD);
  Write_Command(Command);
  WR_0;
  CS_1;
}

/*
����: void LCD_ShowDigital(uint8_t MAddr, uint8_t Number)
����: ����ʾ����ʾһ������
�β�: MAddr �ڴ��ַ, ����д2�ε�ַ��MAddr, MAddr+1
      Number ����
����: ��
*/
void LCD_ShowDigital(uint8_t MAddr, uint8_t Number)
{
  CS_OUT;
  WR_OUT;
  DATA_OUT;
  
  CS_0;
  WR_1;
  Write_ID(WRITE_CMD);
  Write_MemoryAddr(MAddr);

  Write_Data(Number >> 4);
  Write_Data(Number);
  WR_0;
  CS_1;
}

/*
����: void LCD_Init(void)
����: ��ʾ����ʼ��, �����ʾ
�β�: ��
����: ��
*/
void LCD_Init(void)
{
  uint8_t i;
  uint8_t command[] = {SYS_EN, 0x29, LCD_ON};// 0x29 = 1/3 Bias 4 Com
  
  for(i = 0; i < 3; i++)
    HT1261B_WriteCommand(command[i]);
  
  for(i = 0; i < 32; i++)
    HT1261B_WriteData(i, 0x00);
  HT1261B_WriteData(0, 0x00);
}

/*
����: void LCD_ON(void)
����: ����ʾ��
�β�: ��
����: ��
*/
void LCD_Enable(void)
{
  uint8_t i;
  uint8_t command[] = {SYS_EN, 0x29, LCD_ON};// 0x29 = 1/3 Bias 4 Com
  
  for(i = 0; i < 3; i++)
    HT1261B_WriteCommand(command[i]);
}

/*
����: void LCD_OFFvoid)
����: �ر���ʾ��
�β�: ��
����: ��
*/
void LCD_Disable(void)
{
  uint8_t i;
  uint8_t command[] = {SYS_DIS, 0x29, LCD_OFF};// 0x29 = 1/3 Bias 4 Com
  
  for(i = 0; i < 3; i++)
    HT1261B_WriteCommand(command[i]);
}

/*
����: void LCD_ShowWenDu(uint16_t Value)
����: ����ʾ����ʾ�¶�ֵ
�β�: Data 16λ���¶�ֵ
����: ��
*/

void LCD_ShowWenDu(uint16_t Value)
{
  uint8_t i, data[3];
  
  data[0] = Value / 100 % 10;
  data[1] = Value / 10 % 10;
  data[2] = Value % 10;
  
  for(i = 0; i < 3; i++)
    LCD_ShowDigital(i*2, number[data[i]] | 0x80);
}


/*
����: void LCD_ShowShiDu(uint16_t Value)
����: ����ʾ����ʾʪ��ֵ
�β�: Data 16λ��ʪ��ֵ
����: ��
*/
void LCD_ShowShiDu(uint16_t Value)
{
  uint8_t i, data[3];

  data[0] = Value / 100 % 10;
  data[1] = Value / 10 % 10;
  data[2] = Value % 10;
  
  for(i = 0; i < 3; i++)
    LCD_ShowDigital(i*2+12, number[data[i]] | 0x80);
}
#if(MODE == TIMER)
/*
����: void LCD_ShowTime_Hour(uint8_t hour)
����: ����ʾ����ʾʱ��Сʱ
�β�: minute Сʱ
����: ��
*/
void LCD_ShowTime_Hour(uint8_t hour)
{
  // S4  10  P3   11  12  am   pm 
  // 1   8   :    8   8  
  // 18  18  22  20   22  24  25
  
  if(hour <= 12)
    LCD_ShowDigital(24, 0x30 | HT1261B_ReadData(24) & 0x0C);  //AM 
  else
    LCD_ShowDigital(24, 0x21 | HT1261B_ReadData(24) & 0x0C);  //PM
  
  if(hour > 12) hour -= 12;
  //Hour
  if(hour > 9)
    LCD_ShowDigital(18, number[hour%10] | 0x80);
  else
    LCD_ShowDigital(18, number[hour%10]);  
}

/*
����: void LCD_ShowTime_Minute(uint8_t minute)
����: ����ʾ����ʾʱ�����
�β�: minute ����
����: ��
*/
void LCD_ShowTime_Minute(uint8_t minute)
{
  // S4  10  P3   11  12  am   pm 
  // 1   8   :    8   8  
  // 18  18  22  20   22  24  25
  
  //Min
  LCD_ShowDigital(20, number[minute/10] & 0x7F);
  LCD_ShowDigital(22, number[minute%10] | 0x80);  
}

/*
����: void LCD_ShowTimer(uint8_t *Value)
����: ����ʾ����ʾʱ��
�β�: *Data ʱ����ֵ��ָ��, Value[0]: ʱ, Value[1]: �֡�
����: ��
*/
void LCD_ShowTimer(uint8_t *Value)
{
  // S4  10  P3   11  12  am   pm 
  // 1   8   :    8   8  
  // 18  18  22  20   22  24  25
  
  LCD_ShowTime_Minute(Value[0]);
  LCD_ShowTime_Hour(Value[1]);
}
#endif
/*
����: void LCD_ShowPM(uint16_t Data)
����: ����ʾ����ʾPM
�β�: Data 16λ��PMֵ
����: ��
*/
//#define PM_V0	60      //0.5V  ADC:77.5
void LCD_ShowPM(uint16_t Value)
{
  // S3   PM25   4       5       6
  //6     10    6       8       10
  
  uint8_t i, data[3];
#if 1   //����ɿ���������
  static unsigned char count = 0;
  float c = 0;
  
  c = 3.3/1023*2*600*(Value - PM_V0);   //ug/m3
  
  if(c > 999) c = 999;
  else if(c <= 20) c = 20;
  if(c == 20) //���� 20��24����
  {
    count++;
    if(count == 1)       c = 20;
    else if(count == 2) c = 22;
    else if(count == 3) c = 21;
    else if(count == 4) c = 24;
    else if(count == 5) c = 23;
    else if(count == 6)
    {
      count = 0;
      c = 20;
    }
  }

  data[0] = (uint16_t)c / 100 % 10;
  data[1] = (uint16_t)c / 10 % 10;
  data[2] = (uint16_t)c % 10;
#else  //ADCֵ
  data[0] = Value / 100 % 10;
  data[1] = Value / 10 % 10;
  data[2] = Value % 10;
#endif   
    
  for(i = 0; i < 3; i++)
    LCD_ShowDigital(6+i*2, number[data[i]]);
  
  LCD_ShowDigital(6, 0x80 | HT1261B_ReadData(6) & 0x7F);  //PM
  
  LCD_ShowDigital(10, 0x80 | HT1261B_ReadData(10) & 0x7F);  //PM2.5               
}

#if( MODE == TVOC)
/*
����: void LCD_ShowTVOC(uint16_t Value)
����: ����ʾ����ʾTVOC
�β�: Data 16λ��TVOCֵ
����: ��
*/
//�ƾ�Ũ��Խ��ADC��ԴԽ��
#define R0      225       //�޾ƾ�ʱ�ĵ���
void LCD_ShowTVOC(uint16_t Value)
{
  // S4  10  P4   11  12  mg/m3 
  // 1   8        8   8  
  // 18  18  20  20   22  25
  uint8_t i, data[4];
  
#if 1   
  static unsigned char count = 0;
  float Rs, L;
  
  if(Value <= 1022)
  {
   
    if(Value == 0) Value = 225;
    Rs = 4.8*10000/(3.3/1023*Value) - 10000;
    L = Rs / Rmax;
    
    if(L > 1)
    {
        old_Rmax = Rs;
        
        count++;
        if(count == 1)       L = 0.04;
        else if(count == 2) L = 0.05;
        else if(count == 3) L = 0.11;
        else if(count == 4) L = 0.07;
        else if(count == 5) L = 0.13;
        else if(count == 6) L = 0.08;
        else if(count == 7) L = 0.15;
        else if(count == 8) L = 0.06;
        else if(count == 9) L = 0.10;
        else if(count == 10) L = 0.14;
        else if(count == 11)
        {
          count = 0;
          L = 0.09;
        }
    }
    else
    {
        if(L > 0.78 && L <= 1)        //��
        {
            L = (1 - L) + 0.01;
            DataStatus.TVOC_Level = 0;
        }
        else if(L > 0.58 && L <= 0.78)
        {
            L = (0.78 - L) + 0.23;
            DataStatus.TVOC_Level = 1;
        }
        else if(L > 0.36 && L <= 0.58)
        {
            L = (0.58 - L) + 0.43;
            DataStatus.TVOC_Level = 2;
        }
        else if( L > 0.18 && L <= 0.36)
        {
            L = (0.36 - L) + 0.65;
            DataStatus.TVOC_Level = 3;
        }
        else if(L <= 0.18)
        {
            if(L <= 0.01) L = 9.99;
            else
            {
              L = (0.18 - L)*1000*(9.16/170)+0.83;
            }
            DataStatus.TVOC_Level = 4;
        }
    }
  }
  else if(Value == 1023) L = 9.99;
  
  L *= 100;
  
  data[0] = (uint16_t)L / 1000 % 10;
  data[1] = (uint16_t)L / 100 % 10;
  data[2] = (uint16_t)L / 10 % 10;
  data[3] = (uint16_t)L % 10;
  
  for(i = 0; i < 3; i++)
    LCD_ShowDigital(18+i*2, number[data[i+1]]);
  
  if(data[0] == 1)
    LCD_ShowDigital(18, 0x80 | HT1261B_ReadData(18) & 0x7F);
#else
  data[0] = Value / 100 % 10;
  data[1] = Value / 10 % 10;
  data[2] = Value % 10;
 
  for(i = 0; i < 3; i++)
    LCD_ShowDigital(18+i*2, number[data[i]]);
#endif   
  
  LCD_ShowDigital(20, 0x80 | HT1261B_ReadData(20) & 0x7F);  //.
  LCD_ShowDigital(24, 0x42 | HT1261B_ReadData(24) & 0x0C);  // TVOC��mg/m3              
}
#endif
/*
����: void LCD_ShowAQI(uint16_t Value)
����: ����ʾ����ʾAQI
�β�: Data 16λ��AQIֵ
����: ��
*/
void LCD_ShowAQI(uint16_t Value)
{
  // P      AQI
  // 26     26
  uint16_t aqi[8] = {AQI_1, AQI_2, AQI_3, AQI_4,
                     AQI_5, AQI_6, AQI_7, AQI_8};
  uint8_t level;
  float c = 0;
  
  c = 3.3/1023*2*600*(Value - PM_V0);   //ug/m3
  
  if(c < AQI_LEVEL_1)
  {
    level = 0;
  }
  else if(c < AQI_LEVEL_2)
  {
    level = 1;
  }
  else if(c < AQI_LEVEL_3)
  {
    level = 2;
  }
  else if(c < AQI_LEVEL_4)
  {
    level = 3;
  }
  else if(c < AQI_LEVEL_5)
  {
    level = 4;
  }
  else if(c < AQI_LEVEL_6)
  {
    level = 5;
  }
  else if(c < AQI_LEVEL_7)
  {
    level = 6;
  }
  else
  {
    level = 7;
  }
  
  CS_OUT;
  WR_OUT;
  DATA_OUT;
  
  CS_0;
  WR_1;
  Write_ID(WRITE_CMD);
  Write_MemoryAddr(26);
  Write_Data(aqi[level]>>8);
  Write_Data(aqi[level]>>4);
  Write_Data(aqi[level]);
  WR_0;
  CS_1; 
}

/*
����: void LCD_ShowBattery(uint16_t Value)
����: ����ʾ����ʾ����
�β�: Data ����ֵ
����: ��
*/
void LCD_ShowBattery(uint16_t Value)
{
  // Battery
  // 29
  uint8_t battery; //[4] = {BAT_0, BAT_1, BAT_2, BAT_3};
  
  if(Value > BAT_H )
    battery = BAT_3;
  else if(Value > BAT_M)
    battery = BAT_2;
  else if(Value > BAT_L)
    battery = BAT_1;
  else
    battery = BAT_0;
  
  HT1261B_WriteData(29, battery);
}

/*
����: void LCD_ShowBatteryCharge(uint8_t state)
����: ����ʾ����ʾ��س�綯��
�β�: state ����ֵ
����: ��
*/
void LCD_ShowBatteryCharge(uint8_t state)
{
  // Battery
  // 29
  uint8_t battery[4] = {BAT_0, BAT_1, BAT_2, BAT_3};
  
  HT1261B_WriteData(29, battery[state]);
}

/*
����: void LCD_ShowBeep(uint8_t state)
����: ����ʾ����ʾ������������־
�β�: state �Ƿ���ʾ��־
����: ��
*/
void LCD_ShowBeep(uint8_t state)
{
  if(state)
    HT1261B_WriteData(25, 0x04 | HT1261B_ReadData(24) & 0xFB);
  else
    HT1261B_WriteData(25, 0x00 | HT1261B_ReadData(24) & 0xFB);
}

/*
����: void LCD_ShowON(uint8_t state)
����: ����ʾ����ʾON��־
�β�: state �Ƿ���ʾ��־
����: ��
*/
void LCD_ShowON(uint8_t state)
{
  if(state)
    HT1261B_WriteData(25, 0x08 | HT1261B_ReadData(24) & 0xF7);
  else
    HT1261B_WriteData(25, 0x00 | HT1261B_ReadData(24) & 0xF7);
}