#include "key_scan.h"
#include "main.h"

extern DATA_STATUS      DataStatus;

/*
函数：unsigned char power_key_driver(void) 
功能：电源键底层函数
参数：无
返回值：按键的状态, 返回无键、短按和长按
*/
unsigned char power_key_driver(void) 
{ 
  static unsigned char power_key_state = key_state_0;
  static unsigned int power_key_time = 0; 
  unsigned char key_press, key_return = N_key; 

  if(!power_key_input)                 
    key_press = 0;              //按下               
  else
    key_press = 1;              //松开

  switch (power_key_state) 
  { 
  case key_state_0:                             
    if (!key_press) power_key_state = key_state_1;     
    break; 

  case key_state_1:                      
    if (!key_press) 
    { 
      power_key_time = 0;                    
      power_key_state = key_state_2;  
    } 
    else 
      power_key_state = key_state_0;  
    break; 

  case key_state_2: 
    if(key_press) 
    { 
      key_return = S_key;       
      power_key_state = key_state_0;   
    } 
    else if (++power_key_time >= KEY_LONG_TIME)    
    { 
      key_return = L_key;       
      power_key_state = key_state_3;   
    } 
    break; 

  case key_state_3:                
    if (key_press) power_key_state = key_state_0; 
    break; 
  } 
  return key_return; 
} 

/*
函数：unsigned char backlit_key_driver(void)
功能：背光键底层函数
参数：无
返回值：按键的状态, 返回无键、短按和长按
*/
unsigned char backlit_key_driver(void) 
{ 
  static unsigned char backlit_key_state = key_state_0;
  static unsigned int backlit_key_time = 0; 
  unsigned char key_press, key_return = N_key; 

  if(!backlit_key_input)                 
    key_press = 0;              //按下               
  else
    key_press = 1;              //松开

  switch (backlit_key_state) 
  { 
  case key_state_0:                             
    if (!key_press) backlit_key_state = key_state_1;     
    break; 

  case key_state_1:                      
    if (!key_press) 
    { 
      backlit_key_time = 0;                    
      backlit_key_state = key_state_2;  
    } 
    else 
      backlit_key_state = key_state_0;  
    break; 

  case key_state_2: 
    if(key_press) 
    { 
      key_return = S_key;       
      backlit_key_state = key_state_0;   
    } 
    else if (++backlit_key_time >= KEY_LONG_TIME)    
    { 
      key_return = L_key;       
      backlit_key_state = key_state_3;   
    } 
    break; 

  case key_state_3:                
    if (key_press) backlit_key_state = key_state_0; 
    break; 
  } 
  return key_return; 
} 

/*
函数：unsigned char beep_key_driver(void) 
功能：报警蜂鸣器键底层函数
参数：无
返回值：按键的状态, 返回无键、短按和长按
*/
unsigned char beep_key_driver(void) 
{ 
  static unsigned char beep_key_state = key_state_0;
  static unsigned int beep_key_time = 0; 
  unsigned char key_press, key_return = N_key; 

  if(!beep_key_input)                 
    key_press = 0;              //按下               
  else
    key_press = 1;              //松开

  switch (beep_key_state) 
  { 
  case key_state_0:                             
    if (!key_press) beep_key_state = key_state_1;     
    break; 

  case key_state_1:                      
    if (!key_press) 
    { 
      beep_key_time = 0;                    
      beep_key_state = key_state_2;  
    } 
    else 
      beep_key_state = key_state_0;  
    break; 

  case key_state_2: 
    if(key_press) 
    { 
      key_return = S_key;       
      beep_key_state = key_state_0;   
    } 
    else if (++beep_key_time >= KEY_LONG_TIME)    
    { 
      key_return = L_key;       
      beep_key_state = key_state_3;   
    } 
    break; 

  case key_state_3:                
    if (key_press) beep_key_state = key_state_0;  
    break; 
  } 
  return key_return; 
} 

/*
函数：unsigned char DC_IN_CHECK_driver(void) 
功能：DC输入检测底层函数
参数：无
返回值：按键的状态, 返回无键、短按和长按
*/
void DC_IN_CHECK_driver(void) 
{ 
  static unsigned char dc_in_state = key_state_0;
  static unsigned int dc_in_time = 0; 
  unsigned char key_press; 

  if(!dc_in_input)                 
    key_press = 1;              //按下               
  else
    key_press = 0;              //松开

  switch (dc_in_state) 
  { 
  case key_state_0:                             
    if (!key_press) dc_in_state = key_state_1;     
    break; 

  case key_state_1:                      
    if (!key_press) 
    { 
      dc_in_time = 0;                    
      dc_in_state = key_state_2;  
    } 
    else 
      dc_in_state = key_state_0;  
    break; 

  case key_state_2: 
    if (++dc_in_time >= 10)     //100ms 消抖  
    { 
      DataStatus.status |= DC_INPUT;        //DC 供电      
      dc_in_state = key_state_3;   
    } 
    break; 

  case key_state_3:                
    if (key_press) 
    {
      DataStatus.status &= ~DC_INPUT;        //电池 供电
      dc_in_state = key_state_0; 
    }
    break; 
  } 
} 

/*
函数：void power_key_read(void)
功能：电源键处理函数, 短按电源开关、长按进入和退出设置时钟模式，
      进入时钟设置模式后，短按进行小时和分钟设置切换。
参数：无
返回值：无
*/
void power_key_read(void)	//每10ms执行一次
{
  static unsigned char power_status = OFF;
  unsigned char key_temp;

  key_temp = power_key_driver();

  if(DataStatus.KeyMode == POWERMODE)      //Power Mode
  {
    if(key_temp == S_key)
    {
      //开关机
      if(power_status == ON)
      {
        power_status = OFF;
        //添加关机执行代码
        POWER_SWITCH_OFF;
        LED_Backlit(BACKLIT_OFF);
        DataStatus.status &= ~POWER_ON;
        //DataStatus.status &= ~STATUS_ON; // 显示屏的ON标志
        LCD_Disable();
      }
      else if(power_status == OFF)
      {
        power_status = ON;
        //添加开机执行代码
        POWER_SWITCH_ON;
        DataStatus.status |= POWER_ON;
        //DataStatus.status |= STATUS_ON; // 显示屏的ON标志
        LCD_Enable();
        if(DataStatus.status & LED_ON) LED_Backlit(GREEN);  //好
        LCD_ShowAll();
      }
    }
#if(MODE == TIMER)
    else if(key_temp == L_key) DataStatus.KeyMode = SETMODE;
#endif
  }
#if(MODE == TIMER)
  else if(DataStatus.KeyMode == SETMODE)
  {
    if(key_temp == S_key)
    {
      //分钟、小时设置切换
      if(DataStatus.SetTime == SET_HOUR) DataStatus.SetTime = SET_MINUTE;
      else if(DataStatus.SetTime == SET_MINUTE) DataStatus.SetTime = SET_HOUR;
    }
    else if(key_temp == L_key) DataStatus.KeyMode = POWERMODE;
  }
#endif
}

/*
函数：void power_key_read(void)
功能：背光键处理函数, 短按背光开关，进入时钟设置模式后，
      短按进行小时和分钟设置加一操作。
参数：无
返回值：无
*/
void backlit_key_read(void)	//每10ms执行一次, +键
{
  static unsigned char backlit_status = ON;
  unsigned char key_temp;

  key_temp = backlit_key_driver();

  if(DataStatus.KeyMode == POWERMODE)      //Power Mode
  {
    if(key_temp == S_key)
    {
      //背光开关
      if(backlit_status == ON)
      {
        //添加背光关闭执行代码
        if(DataStatus.status & POWER_ON) //防止关机时操作按键而改变按键状态
        {
          LED_Backlit(BACKLIT_OFF);
          DataStatus.status &= ~LED_ON;
          backlit_status = OFF; 
        }
      }
      else if(backlit_status == OFF)
      {
        //添加背光开启执行代码
        if(DataStatus.status & POWER_ON) //防止关机时操作按键而改变按键状态
        {
          if(DataStatus.PM_Level == 1)
            LED_Backlit(GREEN);  //好
          else if(DataStatus.PM_Level == 2)
            LED_Backlit(BLUE);  //一般
          else if(DataStatus.PM_Level == 3)
            LED_Backlit(RED);  //差  
          DataStatus.status |= LED_ON;
          backlit_status = ON;
        }
      }
    }
  }
#if(MODE == TIMER)
  else if(DataStatus.KeyMode == SETMODE)
  {
    if(key_temp == S_key)
    {
      Get_RT8563(RTC_ROPT, RTC_SDA_PIN, RTC_SCL_PIN, TimeData, 2, 7);
      //分钟、小时设置切换
      if(DataStatus.SetTime == SET_HOUR)
      {
        //添加小时加一的代码
        if(++TimeData[2] == 24) TimeData[2] = 0;
      }
      else if(DataStatus.SetTime == SET_MINUTE)
      {
        //添加分钟加一的代码
        if(++TimeData[1] == 60) TimeData[1] = 0;
      }
      
      Set_RT8563(RTC_ROPT, RTC_SDA_PIN, RTC_SCL_PIN, TimeData, 2, 7);
      LCD_ShowTime_Hour(TimeData[2]);
      LCD_ShowTime_Minute(TimeData[1]);
    }
    else if(key_temp == L_key)
    {
    
    }
  }
#endif
}

/*
函数：void beep_key_read(void)
功能：报警蜂鸣器键处理函数, 短按报警蜂鸣器开关，进入时钟设置模式后，
      短按进行小时和分钟设置减一操作。
参数：无
返回值：无
*/
void beep_key_read(void)	//每10ms执行一次, -键
{
  static unsigned char beep_status = OFF;
  unsigned char key_temp;

  key_temp = beep_key_driver();

  if(DataStatus.KeyMode == POWERMODE)      //Power Mode
  {
    if(key_temp == S_key)
    {
      //报警蜂鸣器开关
      if(beep_status == ON)
      {
        //添加报警蜂鸣器关闭执行代码
        if(DataStatus.status & POWER_ON) //防止关机时操作按键而改变按键状态
        {
          DataStatus.status &= ~BEEP_ON;
          beep_status = OFF; 
        }
      }
      else if(beep_status == OFF)
      {
        //添加报警蜂鸣器开启执行代码
        if(DataStatus.status & POWER_ON) //防止关机时操作按键而改变按键状态
        {
          DataStatus.status |= BEEP_ON;
          beep_status = ON;   
        }
      }
      LCD_ShowBeep(DataStatus.status & BEEP_ON);
    }
  }
#if(MODE == TIMER)
  else if(DataStatus.KeyMode == SETMODE)
  {
    if(key_temp == S_key)
    {
      Get_RT8563(RTC_ROPT, RTC_SDA_PIN, RTC_SCL_PIN, TimeData, 2, 7);
      //分钟、小时设置切换
      if(DataStatus.SetTime == SET_HOUR)
      {
        //添加小时减一的代码
        if(TimeData[2] == 0) TimeData[2] = 23; 
        else TimeData[2]--;      
      }
      else if(DataStatus.SetTime == SET_MINUTE)
      {
        //添加分钟减一的代码
        if(TimeData[1] == 0) TimeData[1] = 59; 
        else TimeData[1]--;  
      }
      
      Set_RT8563(RTC_ROPT, RTC_SDA_PIN, RTC_SCL_PIN, TimeData, 2, 7);
      LCD_ShowTime_Hour(TimeData[2]);
      LCD_ShowTime_Minute(TimeData[1]);
    }
    else if(key_temp == L_key)
    {
    
    }
  }
#endif
}

#if 0 //调试DC输入检测
/*
函数：void DC_IN_read(void)
功能：报警蜂鸣器键处理函数, 短按报警蜂鸣器开关，进入时钟设置模式后，
      短按进行小时和分钟设置减一操作。
参数：无
返回值：无
*/
void DC_IN_read(void)	
{
  unsigned char key_temp;

  key_temp = beep_key_driver();

  if(key_temp == L_key) DataStatus.status |= DC_INPUT;        //DC 供电
  else if(key_temp == S_key) DataStatus.status &= ~DC_INPUT;        //电池 供电
}
#endif
    