#ifndef __I2C_H
#define __I2C_H

#include "stm8s.h"

/*
RT8563
PE1:SCL
PE2:SDA
PE3:INT

EEPROM 24C02
PG3:SDA3
PG4:SCL3
*/

/** @addtogroup STM8_128_EVAL_LOW_LEVEL_I2C_EE
  * @{
  */
/**
  * @brief  I2C EEPROM Interface pins
  */
#define sEE_I2C                          I2C  
#define sEE_I2C_CLK                      CLK_PERIPHERAL_I2C
#define sEE_I2C_SCL_PIN                  GPIO_PIN_1                  /* PC.01 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOE                       /* GPIOE */
#define sEE_I2C_SDA_PIN                  GPIO_PIN_2                  /* PC.00 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOE                       /* GPIOE */
#define sEE_RT8563_INT_PORT		 GPIOE
#define sEE_RT8563_INT_PIN		 GPIOE


#define sEE_DIRECTION_TX                 0
#define sEE_DIRECTION_RX                 1

/**
  * @}
  */
  
#define I2C_SPEED                        400000
#define I2C_SLAVE_ADDRESS7               0xA0
    
#define RTCAddress                       0xA2

/*RT8563*/
/*
Address Register Name   Bit
                        7       6       5       4       3       2       1       0
    00H Control_Sta_1   Test1   N       STOP    N       TestC   N       N       N
    01H Control_Sta_2   N       N       N       TI_TP   AF      TF      AIE     TIE
    
    02H VL_Seconds      VL      Seconds(0 to 59)
    03H Minutes         x       Minutes(0 to 59)
    04H Hours           x       x       Hours(0 to 23)
    05H Days            x       x       Days(1 to 31)
    06H Weekdays        x       x       x       x       x       Weekdays(0 to 6)
    07H Century_months  C       x       x       Months(1 to 12)
    08H Years           Years(0 to 99)
    
    09H Minute_alarm    AE_M    Minute_alarm(0 to 59)
    0AH Hour_alarm      AE_H    x       Hour_alarm(0 to 23)
    0BH Day_alarm       AE_D    x       Day_alarm(1 to 31)
    
    
    
    
    
    
    */
#define TIME_ADDRESS                     0x02
#define CONTROL_1                        0x00
#define CONTROL_2                        0x01
#define SECONDS                          0x02
#define MINUTES                          0x03
#define HOURS                            0x04
#define DAYS                             0x05
#define WEEKDAYS                         0x06
#define MONTHS                           0x07
#define YEARS                            0x08
#define MINUTE_ALARM                     0x09
#define HOUR_ALARM                       0x0A
#define DAY_ALARM                        0x0B
#define WEEKDAY_ALARM                    0x0C
#define CLKOUT_FREQUENCY                 0x0D
#define TIMER_CONTROL                    0x0E
#define TIMER                            0x0F

void I2C_RTC_Init(void);
void RTC_DeInit(void);
void Read_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToRead);
void Set_RT8563(uint8_t *pBuffer, uint8_t index, uint8_t NumByteToRead);


#endif /* __I2C_H */