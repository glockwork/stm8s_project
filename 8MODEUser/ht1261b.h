#ifndef __HT1261B_H
#define __HT1261B_H

#include "stm8s.h"

//HT1621B Start 均带上拉
#define DATA_OUT	GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST)
#define DATA_IN 	GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT)
#define DATA_1	        GPIO_WriteHigh(GPIOB, GPIO_PIN_5)
#define DATA_0	        GPIO_WriteLow(GPIOB, GPIO_PIN_5)

#define WR_OUT	        GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_OD_HIZ_FAST)
#define WR_1	        GPIO_WriteHigh(GPIOB, GPIO_PIN_4)
#define WR_0	        GPIO_WriteLow(GPIOB, GPIO_PIN_4)

#define RD_OUT	        GPIO_Init(GPIOF, GPIO_PIN_6, GPIO_MODE_OUT_OD_HIZ_FAST)
#define RD_1	        GPIO_WriteHigh(GPIOF, GPIO_PIN_6)
#define RD_0	        GPIO_WriteLow(GPIOF, GPIO_PIN_6)

#define CS_OUT	        GPIO_Init(GPIOF, GPIO_PIN_3, GPIO_MODE_OUT_OD_HIZ_FAST)
#define CS_1	        GPIO_WriteHigh(GPIOF, GPIO_PIN_3)
#define CS_0	        GPIO_WriteLow(GPIOF, GPIO_PIN_3)

//Command ID Type define
#define READ_CMD        0x06
#define WRITE_CMD       0x05
#define READ_WRITE_CMD  0x05
#define COMMAND_CMD     0x04

//Command Code Type
#define SYS_EN          0x01
#define SYS_DIS         0x00
#define LCD_ON          0x03
#define LCD_OFF         0x02

//Number Type define
#define NUM_0           0x5F
#define NUM_1           0x06
#define NUM_2           0x3D
#define NUM_3           0x2F
#define NUM_4           0x66
#define NUM_5           0x6B
#define NUM_6           0x7B
#define NUM_7           0x0E
#define NUM_8           0x7F
#define NUM_9           0x6F

//AQI Type define
#define AQI_1           0x0E00
#define AQI_2           0x0F00
#define AQI_3           0x0F10
#define AQI_4           0x0F30
#define AQI_5           0x0F70
#define AQI_6           0x0F74
#define AQI_7           0x0F76
#define AQI_8           0x0F77

//Battery Type define
#define BAT_0           0x01
#define BAT_1           0x09
#define BAT_2           0x0D
#define BAT_3           0x0F

#define BAT_H           (uint16_t)(4/2/(3.3/1023))      //4.0V
#define BAT_M           (uint16_t)(3.6/2/(3.3/1023))    //3.6V
#define BAT_L           (uint16_t)(3.2/2/(3.3/1023))    //3.2V


void Write_ID(uint8_t ID);
void Write_MemoryAddr(uint8_t Address);
void Write_Data(uint8_t Value);
uint8_t Read_Data(void);
void Write_Command(uint8_t Command);

void HT1261B_WriteCommand(uint8_t Command);
void HT1261B_WriteData(uint8_t MAddr, uint8_t Value);
uint8_t HT1261B_ReadData(uint8_t MAddr);
void LCD_ShowDigital(uint8_t MAddr, uint8_t Number);
void LCD_Init(void);
void LCD_Enable(void);
void LCD_Disable(void);
void LCD_ShowWenDu(uint16_t Value);
void LCD_ShowShiDu(uint16_t Value);
void LCD_ShowTimer(uint8_t *Value);
void LCD_ShowTime_Hour(uint8_t hour);
void LCD_ShowTime_Minute(uint8_t minute);
void LCD_ShowPM(uint16_t Value);
void LCD_ShowTVOC(uint16_t Value);
void LCD_ShowAQI(uint16_t Value);
void LCD_ShowBattery(uint16_t Value);
void LCD_ShowBatteryCharge(uint8_t state);
void LCD_ShowBeep(uint8_t state);
void LCD_ShowON(uint8_t state);

#endif