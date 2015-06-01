#ifndef __MAIN_H
#define __MAIN_H

#include "stm8s.h"
#include "adc.h"
#include "iwdg.h"
#include "gpio.h"
#include "ht1261b.h"
#include "am2320.h"
#include "basic_timer.h"
#include "data_backup.h"

#if(MODE == TIMER)
  #include "rt8563.h"
#endif

#include "key_scan.h"


#define DEBUG   0
#define TIMER   1
#define TVOC    2
#define MODE    TVOC

#define BEEP_TIME       100     //蜂鸣器叫声频率 1 s

#define AQI_LEVEL_1     70      //PM传感器获取的电压对应的AQI等级
#define AQI_LEVEL_2     80
#define AQI_LEVEL_3     130
#define AQI_LEVEL_4     250
#define AQI_LEVEL_5     300
#define AQI_LEVEL_6     400
#define AQI_LEVEL_7     500
#define AQI_LEVEL_8     600

#define DC_INPUT        0x10    //DC输入
#define BEEP_ON         0x80    //蜂鸣器开
#define TEMP_GET        0x40    //温湿度获取成功
#define LED_ON          0x20    //背光开
#define POWER_ON        0x08    //电源按键开
#define STATUS_ON       0x04    //状态图标

typedef struct
{
  uint8_t       am2320[4];      //AM2320 Data
  uint16_t      temp_data;      //温度
  uint16_t      rh_data;        //湿度
  uint8_t       aqi_data;       //AQI Data
  uint16_t      tvoc_data;      //TVOC Data
  uint16_t      pm_data;        //PM2.5 Data
  uint16_t      battery_data;   //Battery Data
  /*
  status        0b0000 0000
  beep          0b1000 0000
  temp          0b0100 0000
  LED           0b0010 0000
  DC            0b0001 0000
  power         0b0000 1000
  status_on     0b0000 0100
  */
  uint8_t       status;         //beep, LED status
  uint8_t       PM_Level;
  uint8_t       battery_count;
  uint8_t       KeyMode;
  uint8_t       SetTime;
  uint8_t       count;
  uint8_t       pm_error_judge;
  uint8_t       tvoc_error_judge;
  uint32_t      pm_8data;
  uint32_t      tvoc_8data;
  uint8_t       tvoc_ON;
  uint8_t       TVOC_Level;
}DATA_STATUS;

typedef struct
{
  uint8_t       w1sFlags;
  uint8_t       w10minFlags;
  uint8_t       w30minFlags;
  uint8_t       w2sFlags;
  uint16_t      ReloadDogTime;
}TIME_COUNT;


void LCD_ShowAll(void);
void Get_Sensor(void);
void Get_Temp(void);

#endif