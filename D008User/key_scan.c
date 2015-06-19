#include "key_scan.h"
#include "main.h"

extern DEVICE_STATUS    DeviceStatus;

/*
函数：unsigned char code_key_driver(void) 
功能：功能选择键底层函数
参数：无
返回值：按键的状态, 返回无键、短按和长按
*/
unsigned char code_key_driver(void) 
{ 
    static unsigned char code_key_state = key_state_0;
    static unsigned int code_key_time = 0; 
    unsigned char key_press, key_return = N_key; 

    if(!code_key_input)                 
        key_press = 0;              //按下               
    else
        key_press = 1;              //松开

    switch (code_key_state) 
    { 
    case key_state_0:                             
        if (!key_press) code_key_state = key_state_1;     
        break; 

    case key_state_1:                      
        if (!key_press) 
        { 
            code_key_time = 0;                    
            code_key_state = key_state_2;  
        } 
        else 
            code_key_state = key_state_0;  
        break; 

    case key_state_2: 
        if(key_press) 
        { 
            key_return = S_key;       
            code_key_state = key_state_0;   
        } 
        else if (++code_key_time >= KEY_LONG_TIME)    
        { 
            key_return = L_key;       
            code_key_state = key_state_3;   
        } 
        break; 

    case key_state_3:                
        if (key_press) code_key_state = key_state_0; 
        break; 
    } 
    return key_return; 
} 


/*
函数：void power_key_read(void)
功能：功能选择键处理函数.
参数：无
返回值：无
*/
void code_key_read(void)	//每10ms执行一次
{
    unsigned char key_temp;

    key_temp = code_key_driver();

    if(key_temp == S_key)
    {
        
        if(DeviceStatus.enterMode == ENTER_DEFINE)                      // Code按键默认状态
        {
            showFunction(DeviceStatus.workState, ON);                   // 让功能指示灯常亮
                
            if(DeviceStatus.workState == 14)
            {
                DeviceStatus.up_Temperature = 0;
                if(0 == DeviceStatus.down_Temperature) DeviceStatus.down_Temperature = Temperature[DeviceStatus.workState];     // 获取下管默认的温度值
                DeviceStatus.hotUpDown = HOT_DOWN;      // 仅为下管工作
            }
            else  
            {
                if(0 == DeviceStatus.up_Temperature) DeviceStatus.up_Temperature = Temperature[DeviceStatus.workState];         // 获取上管默认的温度值
                if(0 == DeviceStatus.down_Temperature) DeviceStatus.down_Temperature = Temperature[DeviceStatus.workState];     // 获取下管默认的温度值
                DeviceStatus.hotUpDown = HOT_UP;        // 默认为上管工作
            }
            
            showSymbol(DeviceStatus.hotUpDown);                         // 显示温度符号
            DeviceStatus.enterMode = ENTER_SET_UP_DOWN_TEMP;
        }
        else if((DeviceStatus.enterMode == ENTER_SET_UP_DOWN_TEMP) || 
                (DeviceStatus.enterMode == ENTER_SET_TIME))               // Code按键设置上下管温度状态 Code按键设置工作时间状态    
        {
            if(0 == DeviceStatus.workTime) DeviceStatus.workTime = Timing[DeviceStatus.workState];      // 获取默认工作时间
            showTime(DeviceStatus.workTime, ON, ON);                    // 显示工作时间
            DeviceStatus.startWork = ON;                                 // 设置工作指示标志
          DeviceStatus.startWorkBeep = ON;
            DeviceStatus.enterMode = ENTER_START_WORK;
        }
        else if(DeviceStatus.enterMode == ENTER_START_WORK)             // Code按键开始工作状态
        {
            DeviceStatus.startWork = OFF;                                 // 清除工作指示标志
            if(0 == DeviceStatus.workTime) DeviceStatus.workTime = Timing[DeviceStatus.workState];      // 获取默认工作时间
            showTime(DeviceStatus.workTime, ON, ON);                    // 显示工作时间
            DeviceStatus.enterMode = ENTER_SET_TIME;
        }
        else
        {
            DeviceStatus.enterMode = ENTER_DEFINE;
        }
        
        KeyBeep();
    }
}