#include "key_scan.h"
#include "main.h"

extern DEVICE_STATUS      DeviceStatus;

/*
函数：void openings_check(void) 
功能：开盖检测底层函数
参数：无
返回值：按键的状态, 返回无键、短按和长按
*/
void openings_check(void) 
{ 
    static unsigned char openings_state = key_state_0;
    static unsigned int dc_in_time = 0; 
    unsigned char key_press; 

    if(!openings_check_input)                 
        key_press = 1;              //合盖               
    else
        key_press = 0;              //开盖

    switch (openings_state) 
    { 
    case key_state_0:                             
        if (!key_press) openings_state = key_state_1;     
        break; 

    case key_state_1:                      
        if (!key_press) 
        { 
            dc_in_time = 0;                    
            openings_state = key_state_2;  
        } 
        else 
            openings_state = key_state_0;  
        break; 

    case key_state_2: 
        if (++dc_in_time >= 10)     // 300ms 合盖
        { 
            DeviceStatus.Openings = 1;
            openings_state = key_state_3;   
        } 
        break; 

    case key_state_3:                
        if (key_press) 
        {
            ZWD_POWER_OFF;              // 关闭紫外灯电源
            LED_2_OFF;                  // 指示灯关闭
            DeviceStatus.T_6Min_Count = 0;
            DeviceStatus.Openings = 0;
            DeviceStatus.Disinfect = 0;         // 开盖
            openings_state = key_state_0; 
        }
        break; 
    } 
} 

/*
函数：unsigned char feng_key_driver(void)
功能：风扇开关键底层函数
参数：无
返回值：按键的状态, 返回无键、短按和长按
*/
unsigned char feng_key_driver(void) 
{ 
    static unsigned char backlit_key_state = key_state_0;
    static unsigned int backlit_key_time = 0; 
    unsigned char key_press, key_return = N_key; 

    if(!feng_key_input)                 
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
函数：unsigned char disinfect_key_driver(void) 
功能：消毒控制键底层函数
参数：无
返回值：按键的状态, 返回无键、短按和长按
*/
unsigned char disinfect_key_driver(void) 
{ 
    static unsigned char beep_key_state = key_state_0;
    static unsigned int beep_key_time = 0; 
    unsigned char key_press, key_return = N_key; 

    if(!disinfect_key_input)                 
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
函数：void feng_key_read(void)
功能：风扇控制开关。
参数：无
返回值：无
*/
void feng_key_read(void)
{
    unsigned char key_temp;

    key_temp = feng_key_driver();

    if(key_temp == S_key)
    {
        if(DeviceStatus.Feng == 0)
        {
            LED_1_ON;
            FLZ_POWER_ON;
            FAN_1_SPEED_LOW;
            FAN_2_SPEED_LOW;
            DeviceStatus.Feng = 1;
        }
        else if(DeviceStatus.Feng == 1)
        {
            FAN_1_SPEED_HIGH;
            FAN_2_SPEED_HIGH;
            DeviceStatus.Feng = 2;
        }
        else if(DeviceStatus.Feng == 2)
        {
            LED_1_OFF;
            FLZ_POWER_OFF;
            FAN_1_SPEED_OFF;
            FAN_2_SPEED_OFF;
            DeviceStatus.Feng = 0; 
        } 
#if 0
        else
        {
            LED_1_OFF;
            FLZ_POWER_OFF;
            FAN_1_SPEED_OFF;
            FAN_2_SPEED_OFF;
            DeviceStatus.Feng = 0; 
        }
#endif
    }
#if 0
    else if(key_temp == L_key) /*添加长按按键功能代码*/
#endif
}

/*
函数：void disinfect_key_read(void)
功能：消毒功能开关
参数：无
返回值：无
*/
void disinfect_key_read(void)	
{
    unsigned char key_temp;

    key_temp = disinfect_key_driver();

    if(key_temp == S_key)
    {
        if(DeviceStatus.Openings == 1)          // 未开盖
        {
            if(DeviceStatus.Disinfect == ON) 
            {
                ZWD_POWER_OFF;
                LED_2_OFF;
                DeviceStatus.T_6Min_Count = 0;
                DeviceStatus.Disinfect = OFF; 
            }
            else if(DeviceStatus.Disinfect == OFF) 
            {
                LED_2_ON;
                ZWD_POWER_ON;
                DeviceStatus.Disinfect = ON;   // 开启消毒
            }
#if 0
            else
            {
                ZWD_POWER_OFF;
                LED_2_OFF;
                DeviceStatus.Disinfect = OFF; 
            }
#endif
        }
    }
#if 0
    else if(key_temp == L_key) /*添加长按按键功能代码*/
#endif
}
  
    