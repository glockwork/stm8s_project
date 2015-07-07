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
        KeyBeep();
        if(DeviceStatus.enterMode == ENTER_CHOICE_FUNCTION)             // Code按键 默认状态
        {
            SelectedFunction();
            DeviceStatus.enterMode = ENTER_SELECTED_FUNCTION;           // 标志为确认功能模式
        }
        else if(DeviceStatus.enterMode == ENTER_SELECTED_FUNCTION)
        {
            showTime(DeviceStatus.workTime, ON, ON);                    // 显示工作时间
            showPreheat(ON);                                            // 让预热指示灯显示， 如果带预热则亮，否则不会亮
            DeviceStatus.startWorkBeep = ON;                            // 工作提示音
            
            DeviceStatus.knob = KNOB_DISABLE;                           // 禁用旋钮
            DeviceStatus.setMode = SET_TIME;                            // 设置时间
            
            if(DeviceStatus.preheat == ON)
            {
                DeviceStatus.startWork = OFF;                           // 清除工作指示标志
                DeviceStatus.enterMode = ENTER_PREHEAT;                 // 预热工作状态
            }
            else
            {   
                if(DeviceStatus.workState == 8) RELAY_3_H;              // 烤鸡功能 开转插
                DeviceStatus.startWork = ON;                            // 设置工作指示标志
                DeviceStatus.enterMode = ENTER_START_WORK;              // 倒计时加热
            }
        }
        else if(DeviceStatus.enterMode == ENTER_PREHEAT)
        {
//            showTime(DeviceStatus.workTime, ON, ON);                    // 显示工作时间
            if(DeviceStatus.workState == 8) RELAY_3_H;              // 烤鸡功能 开转插
            DeviceStatus.startWork = ON;                                // 设置工作指示标志
            DeviceStatus.preheat = OFF;                                 // 清预热指示灯标志
            showPreheat(OFF);                                           // 不显示预热指示灯
            DeviceStatus.startWorkBeep = ON;                            // 工作提示音
            DeviceStatus.enterMode = ENTER_START_WORK;                  // Code按键开始工作状态
            DeviceStatus.knob = KNOB_DISABLE;                           // 禁用旋钮
        }
        else if(DeviceStatus.enterMode == ENTER_START_WORK)           
        {
            Relay_Off_All();
            DeviceStatus.startWork = OFF;                               // 清除工作指示标志
//            showTime(DeviceStatus.workTime, ON, ON);                    // 显示工作时间
            DeviceStatus.knob = OFF;                                    // 启用旋钮              
            DeviceStatus.setMode = SET_TIME;                            // 标志位设置时间
            DeviceStatus.enterMode = ENTER_PAUSE_WORK;
        }
        else if(DeviceStatus.enterMode == ENTER_PAUSE_WORK)
        {
            showTime(DeviceStatus.workTime, ON, ON);                    // 显示工作时间
            DeviceStatus.startWorkBeep = ON;                            // 工作提示音
            DeviceStatus.startWork = ON;                                // 设置工作指示标志
            if(DeviceStatus.workState == 8) RELAY_3_H;                  // 烤鸡功能 开转插
            DeviceStatus.enterMode = ENTER_START_WORK;                  // 倒计时加热
            DeviceStatus.knob = KNOB_DISABLE;                           // 禁用旋钮
        }
        else
        {
            DeviceStatus.enterMode = ENTER_CHOICE_FUNCTION;
            DeviceStatus.enterMode = ENTER_CHOICE_FUNCTION;             // 标志为选择功能模式
        }
        DeviceStatus.flashLight = 0;
        
    }
}

// 确认功能模式-初始化功能参数
void SelectedFunction(void)
{
    if(0 == DeviceStatus.workTime) DeviceStatus.workTime = Timing[DeviceStatus.workState];      // 获取默认工作时间

    if(DeviceStatus.workState == 14)                            // 判断是否为发酵功能
    {
        DeviceStatus.up_Temperature = 0;
        if(0 == DeviceStatus.down_Temperature) DeviceStatus.down_Temperature = Temperature[DeviceStatus.workState];     // 获取下管默认的温度值
        DeviceStatus.hotUpDown = HOT_DOWN;                      // 仅为下管工作
    }
    else  
    {
        if(0 == DeviceStatus.up_Temperature) DeviceStatus.up_Temperature = Temperature[DeviceStatus.workState];         // 获取上管默认的温度值
        if(0 == DeviceStatus.down_Temperature) DeviceStatus.down_Temperature = Temperature[DeviceStatus.workState];     // 获取下管默认的温度值
        DeviceStatus.hotUpDown = HOT_UP;                        // 默认为上管工作
    }

    if(((DeviceStatus.workState >= 9) && (DeviceStatus.workState <= 13)) || (DeviceStatus.workState == 15))             // 判断是否有预热功能
    {
        DeviceStatus.preheat = ON;
    }
    else
    {
        DeviceStatus.preheat = OFF;                             
    }
    showPreheat(ON);                                            // 显示预热指示灯
    showFunction(DeviceStatus.workState, ON);                   // 让功能指示灯常亮
    showSymbol(DeviceStatus.hotUpDown);                         // 显示温度符号
    DeviceStatus.knob = OFF;                                    // 启用旋钮 
    DeviceStatus.setMode = SET_TEMP;                            // 标志位设置温度
}