#include "key_scan.h"

extern DEVICE_STATUS    DeviceStatus;

unsigned char reset_key_driver(void) 
{ 
    static unsigned char reset_key_state = key_state_0;
    static unsigned int reset_key_time = 0; 
    unsigned char key_press, key_return = N_key; 

    if(!reset_key_input)                 
        key_press = 0;              //按下               
    else
        key_press = 1;              //松开

    switch (reset_key_state) 
    { 
    case key_state_0:                             
        if (!key_press) reset_key_state = key_state_1;     
        break; 
    case key_state_1:                      
        if (!key_press) 
        { 
            reset_key_time = 0;                    
            reset_key_state = key_state_2;  
        } 
        else reset_key_state = key_state_0;  
        break; 
    case key_state_2: 
        if(key_press) 
        { 
            key_return = S_key;       
            reset_key_state = key_state_0;   
        } 
        else if (++reset_key_time >= RESET_L_TIME)    
        { 
            key_return = L_key;       
            reset_key_state = key_state_3;   
        } 
        break; 
    case key_state_3:                
        if (key_press) reset_key_state = key_state_0; 
        break; 
    } 
    return key_return; 
} 

unsigned char touch_key_driver(void) 
{ 
    static unsigned char touch_key_state = key_state_0;
    static unsigned int touch_key_time = 0; 
    unsigned char key_press, key_return = N_key; 

    if(!touch_key_input)                 
    key_press = 0;              //按下                
    else
    key_press = 1;              //松开

    switch (touch_key_state) 
    { 
    case key_state_0:                             
        if (!key_press) 
        { 
            touch_key_time = 0;                  
            touch_key_state = key_state_2;   
        } 
        else touch_key_state = key_state_0; 
        break; 
    case key_state_2: 
        if(key_press) 
        { 
            key_return = S_key;        
            touch_key_state = key_state_0;  
        } 
        else if (++touch_key_time >= TOUCH_L_TIME)    
        { 
            key_return = L_key;  
            touch_key_state = key_state_3;   
        } 
        break; 
    case key_state_3:               
        if (key_press) touch_key_state = key_state_0; 
        break; 
    } 
    return key_return; 
} 

/*
名称: void TouchKey_Read(void)
功能: 触摸按键功能实现
形参: 无
返回值：无
*/ 
void TouchKey_Read(void)
{
    unsigned char key_temp;

    key_temp = touch_key_driver();

    if(key_temp == S_key)
    {
        if(DeviceStatus.WorkStatus & DEVICE_SWITCH_MASK)        // 开机
        {
            if(DeviceStatus.FanMode == 1)       // 高速转为低速
            {
                FAN_SPEED_LOW;
                DeviceStatus.FanMode = 2;
            }
            else if(DeviceStatus.FanMode == 2)  // 低速转为高速
            {
                FAN_SPEED_HIGH;
                DeviceStatus.FanMode = 1;
            }   
            else                                // 其他状态则转为低速 
            {
                FAN_SPEED_LOW;
                DeviceStatus.FanMode = 2;
            }
        }
        
    }
    else if(key_temp == L_key)
    {
        if(DeviceStatus.WorkStatus & DEVICE_SWITCH_MASK)        // 开机
        {
            DeviceWait();
            if(DeviceStatus.FanMode != 3) DeviceStatus.FanMode = 2;
            else 
            DeviceStatus.WorkStatus &= ~DEVICE_SWITCH_MASK;
        }
        else
        {
            DeviceStatus.WorkStatus |= DEVICE_SWITCH_MASK;
            DeviceWork();
            if(DeviceStatus.FanMode == 1) FAN_SPEED_HIGH;
            else if(DeviceStatus.FanMode == 2) FAN_SPEED_LOW;
            else FAN_SPEED_LOW;
        }
    }
}

/*
名称: void ResetKey_Read(void)
功能: 复位按键功能实现
形参: 无
返回值：无
*/ 
void ResetKey_Read(void)
{
    unsigned char key_temp;

    key_temp = reset_key_driver();

    if(key_temp == S_key)
    {
        //短按Reset键， 滤网使用时间清零
    }
    else if(key_temp == L_key)
    {
        //长按Reset键， 恢复出厂设置
    }
}