#include "function.h"

extern DEVICE_STATUS    DeviceStatus;

unsigned char num[] = {// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, DP, C
    0x5F, 0x42,0x9B, 0xD3, 0xC6, 0xD5, 0xDD, 0x43, 
    0xDF, 0xD7, 0x20, 0x1D};

unsigned char led[16] = {// 16->1
    0x08, 0x01, 0x04, 0x02, 0x80, 0x40, 0x20, 0x10,
    0x80, 0x40, 0x20, 0x10, 0x02, 0x04, 0x01, 0x08};

unsigned char KEY[5] = {0};

void KeyBeep(void)
{
    BEEP_H;
    DeviceStatus.beep = 0;
    DeviceStatus.beepSW = 1;
}

void PowerOnBeep(void)
{
    BEEP_H;
    Delay(200);
    BEEP_L;
    Delay(200);
    BEEP_H;
    Delay(200);
    BEEP_L;
    Delay(200);
    BEEP_H;
    Delay(200);
    BEEP_L;
    Delay(600);
    BEEP_Cmd(DISABLE);
}

void StartWorkBeep(void)
{
    BEEP_H;
    Delay(200);
    BEEP_L;
    Delay(200);
    BEEP_H;
    Delay(200);
    BEEP_L;
    Delay(600);
    BEEP_Cmd(DISABLE);
}

#if 0
//控制旋钮顺时针方向转一次
void right_handwise(void)
{
    Delay(2);
    B_L;
    Delay(2);
    A_L;
    Delay(2);
    B_H;
    Delay(2);
    A_H;
    if(++DeviceStatus.workState >= 17) DeviceStatus.workState = 3;
}

//控制旋钮逆时针方向转一次
void left_handwise(void)
{
    A_L;
    B_H;
    Delay(2);
    B_L;
    Delay(2);
    A_H;
    Delay(2);
    A_H;
    B_H;
    Delay(2);
    if(--DeviceStatus.workState <= 2) DeviceStatus.workState = 16;
}
#endif
// 向TM1628发送一个字节 
void send_8bit(uint8_t data)
{
    uint8_t i;

    for(i = 0; i < 8; i++)
    {
        
        if(data & 0x01) DIO_H;
        else DIO_L;
        CLK_L;
        CLK_H;
        data >>= 1;
    }
}

// 向TM1628发送命令
void command(uint8_t command)
{
    STB_H;
    STB_L;
    send_8bit(command);
}

// TM1628显示所有内容
void showAll(void)
{
    uint8_t i;
    
    command(0x03);      // 设置显示模式， 7位10段模式
#if 1
    command(0x40);      // 地址自动加一模式
#else
    command(0x44);      // 固定地址模式
#endif
    command(0xC0);      // 设置显示地址， 00H开始
    
    for(i = 0; i < 7; i++)
    {
        send_8bit(0xFF);        // 从00H起， 偶地址写显示数据
        send_8bit(0x00);        // 因为SEG9-14均未用到，所以奇地址全写0
    }
    
    command(0x89);      //开显示、1/16占空比。
    STB_H;
}

// TM1628清除所有显示
void clear(void)
{
    uint8_t i;
    
    command(0x03);
#if 1
    command(0x40);
#else
    command(0x44);
#endif
    command(0xC0);
    
    for(i = 0; i < 14; i++) send_8bit(0x00);
    
    command(0x89);      //开显示、1/16占空比。
    STB_H;
}

// TM1628显示功能指示灯
// function：功能，state：状态-ON显示 OFF不显示
void showFunction(uint8_t function, uint8_t state)
{
    if(function >= 9) 
    {
        command(0xC0);
        if(state) send_8bit(led[function-1]);
        else send_8bit(0);
        command(0xC2);
        if(DeviceStatus.light == ON) send_8bit(0x08);
        else send_8bit(0);
    }
    else 
    {
        command(0xC0);
        send_8bit(0);
        command(0xC2);
        if(state)
        {
            if(DeviceStatus.light == ON) send_8bit(led[function-1] | 0x08);
            else send_8bit(led[function-1]);
        }
        else 
        {
            if(DeviceStatus.light == ON) send_8bit(0x08);
            else send_8bit(0);
        }
    }
    STB_H;
}

// TM1628显示炉灯指示灯
void showLight(void)
{
    uint8_t data = 0x08;

    if(DeviceStatus.light == OFF) data = 0;
    if(DeviceStatus.preheat == ON) data |= 0x01;
  
    command(0xC2);
    send_8bit(data);

    STB_H;
}

// TM1628显示预热指示灯
void showPreheat(uint8_t sw)
{
    uint8_t data  = 0x01;       // 预热指示灯
    
    if(DeviceStatus.preheat == ON) data = 0x01;
    else data = 0;
    
    if(DeviceStatus.light == ON) data |= 0x08;
    
    command(0xC2);
    if(sw == ON) send_8bit(data);
    else send_8bit(data & 0x08);
    
    STB_H;
}

//TM1628 显示数码管
// index：第几个数码管（1-4） number：数字
void showLED(uint8_t index, uint8_t number)
{
    switch(index)
    {
    case 1:
        command(0xCA);
        break;
    case 2:
        command(0xC6);
        break;
    case 3:
        command(0xC4);
        break;
    case 4:
        command(0xC8);
        break;
    }
    send_8bit(num[number]);
    STB_H;
}

// TM1628数码管显示时间
// time：时间， sw-ON：显示，OFF不显示， dp：是否显示点， ON：显示，OFF不显示
void showTime(uint16_t time, uint8_t sw, uint8_t dp)
{
    uint8_t a, b, c;
    
    command(0xCA);
    send_8bit(0);
    
    if(ON == sw)
    {
        a = time / 60 % 10;
        b = time % 60 / 10 % 10;
        c = time % 60 % 10;

        command(0xC6);
        if(ON == dp) send_8bit(num[a] | 0x20);
        else send_8bit(num[a]);
        
        command(0xC4);
        send_8bit(num[b]);
        
        command(0xC8);
        send_8bit(num[c]);
    }
    else
    {
        command(0xC6);
        send_8bit(0x20);
        
        command(0xC4);
        send_8bit(0);
        
        command(0xC8);
        send_8bit(0);
    }
    
    STB_H;
}

// TM1628 数码管显示温度值
// temp：温度值， sw：ON-显示，OFF-不显示
void showTemp(uint8_t temp, uint8_t sw)
{
    uint8_t a, b, c;
    
    if(ON == sw)
    {
        a = temp / 100 % 10;
        b = temp / 10 % 10;
        c = temp % 10;

        command(0xCA);
        if(a == 0) send_8bit(0);
        else send_8bit(num[a]);
        
        command(0xC6);
        send_8bit(num[b]);
        
        command(0xC4);
        send_8bit(num[c]);
        
        if(DeviceStatus.workState == 14) 
        {
            if((61 == temp) || (29 == temp)) showOFF();
        }
        else 
        {
            if((231 == temp) || (39 == temp)) showOFF();
        }
    }
    else
    {
        command(0xCA);
        send_8bit(0);
        
        command(0xC6);
        send_8bit(0);
        
        command(0xC4);
        send_8bit(0);
    }
    
    STB_H;
}

// TM1628 数码管显示温度符号C
// symbol ：默认位置、上位置、下位置
void showSymbol(uint8_t symbol)
{
    command(0xC8);
    if(SYMBOL_DEFAULT == symbol) send_8bit(num[11]);          // 默认温度
    else if(SYMBOL_UP == symbol) send_8bit(0x85);       // 上管温度
    else if(SYMBOL_DOWN == symbol) send_8bit(0x98);       // 下管温度
    STB_H;
}

// TM1628 数码管显示‘OFF’
void showOFF(void)
{
    command(0xCA);
    send_8bit(0x5F);
    
    command(0xC6);
    send_8bit(0x8D);
    
    command(0xC4);
    send_8bit(0x8D);
    STB_H;
}

// 获取TM1628按键状态
void read_key(void)
{
    uint8_t i, j;
    
    command(0x42);
    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);        // DIO
    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < 8; j++)
        {
            KEY[i] >>= 1;
            CLK_L;
            CLK_H;
            if(READ_DIO != RESET) KEY[i] |= 0x80;
        }
    }
    STB_H;
    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);
}

// TM1628按键处理程序
void key_process(void)
{
    static uint8_t key1 = 0, key2 = 0;
    
    if(key1 != KEY[0])
    {
        if(KEY[0] & 0x08) // 设置上下管温度
        {
            if(DeviceStatus.enterMode == ENTER_SET_UP_DOWN_TEMP)
            {
                if(14 != DeviceStatus.workState)
                {
                    if(HOT_UP == DeviceStatus.hotUpDown)
                    {
                        DeviceStatus.hotUpDown = HOT_DOWN;
                        showTemp(DeviceStatus.down_Temperature, ON);
                        showSymbol(SYMBOL_DOWN);
                    }
                    else if(HOT_DOWN == DeviceStatus.hotUpDown)
                    {
                        DeviceStatus.hotUpDown = HOT_UP;
                        showTemp(DeviceStatus.up_Temperature, ON);
                        showSymbol(SYMBOL_UP);
                    }
                }
                KeyBeep();
            }
            else if((DeviceStatus.enterMode == ENTER_SET_TIME) || 
                     (DeviceStatus.enterMode == ENTER_START_WORK))
            {
                if(14 != DeviceStatus.workState)
                {
                    if(HOT_UP == DeviceStatus.hotUpDown)
                    {
                        showTemp(DeviceStatus.up_Temperature, ON);
                        showSymbol(SYMBOL_UP);
                    }
                    else if(HOT_DOWN == DeviceStatus.hotUpDown) 
                    {
                        showTemp(DeviceStatus.down_Temperature, ON);
                        showSymbol(SYMBOL_DOWN);
                    }
                }
                else 
                {
                    showTemp(DeviceStatus.down_Temperature, ON);
                    showSymbol(SYMBOL_DOWN);
                }
                DeviceStatus.flashLight = 0;
                KeyBeep();
                DeviceStatus.enterMode = ENTER_SET_UP_DOWN_TEMP;
            }
            DeviceStatus.flashLight = 0;
        }
        if(KEY[0] & 0x01) // 炉灯开关
        {
            
            if(DeviceStatus.light == 0)
            {
                RELAY_1_H;
                DeviceStatus.light = 1;
            }
            else
            {
                RELAY_1_L;
                DeviceStatus.light = 0;
            }
            showLight();
            KeyBeep();
        }
        key1 = KEY[0];
        
    }
    if(key2 != KEY[1])
    {
        if(KEY[1] & 0x08) // 取消键
        {
            DeviceStatus.enterMode = ENTER_DEFINE;
            DeviceStatus.preheat = 0;
            DeviceStatus.startWork = 0;
            DeviceStatus.workState = 16;
            DeviceStatus.workTime = 0;
            DeviceStatus.hotUpDown = HOT_UP;
            DeviceStatus.up_Temperature = 0;
            DeviceStatus.down_Temperature = 0;
            showFunction(DeviceStatus.workState, ON);
            showTemp(Temperature[DeviceStatus.workState], ON);
            showSymbol(SYMBOL_DEFAULT);
            KeyBeep();
        }
        if(KEY[1] & 0x01) // 设置时间
        {
            if(DeviceStatus.enterMode != ENTER_DEFINE)
            {
                
                if(DeviceStatus.enterMode == ENTER_SET_UP_DOWN_TEMP)
                {
                    if(DeviceStatus.startWork == OFF)   // 是否处于工作状态
                    {
                        if(0 == DeviceStatus.workTime) DeviceStatus.workTime = Timing[DeviceStatus.workState];
                        showTime(DeviceStatus.workTime, ON, ON);
                        DeviceStatus.enterMode = ENTER_SET_TIME;
                    }
                    else
                    {
                        showTime(DeviceStatus.workTime, ON, ON);
                        DeviceStatus.enterMode = ENTER_START_WORK;
                    }
                }
                else if(DeviceStatus.enterMode == ENTER_START_WORK)
                {
                    if(0 == DeviceStatus.workTime) DeviceStatus.workTime = Timing[DeviceStatus.workState];
                    showTime(DeviceStatus.workTime, ON, ON);
                }
                KeyBeep();
                DeviceStatus.flashLight = 0;
            }
        }
        key2 = KEY[1];
        
    }
}

//获取旋钮变化状态
void CodingSwitchPolling(void)
{ 
    static unsigned char st = 0, Aold = 0, Bold = 0;
    if(CodingSwitch_A != RESET && CodingSwitch_B != RESET) st = SET;
    if(st == SET)
    {
        if(CodingSwitch_A == RESET && CodingSwitch_B == RESET)   //如果当前编码开关的两个引脚都为底电平执行下面的步骤 
        {
            if(Aold != RESET)                                  //B为高说明编码开关在向加大的方向转
            {
                st = RESET;
//                UART1_SendByte(DeviceStatus.state);
                CodeProcess(1);
            } 
            else if(Bold != RESET)                   //else不能省! 省了右旋会同时加减
            {
                st = RESET;
//                UART1_SendByte(DeviceStatus.state);
                CodeProcess(-1);
            }
        }         
    }
    Aold = CodingSwitch_A;
    Bold = CodingSwitch_B;
}

//根据旋钮的方向和旋钮按键的状态来执行相应的代码
// direction: 方向，1与-1
void CodeProcess(int8_t direction)
{
    if(DeviceStatus.enterMode == ENTER_DEFINE)                      // Code按键默认状态
    {
        KeyBeep();
        if(1 == direction)
        {
            if(++DeviceStatus.workState >= 17) DeviceStatus.workState = 3;
        }
        else
        {
            if(--DeviceStatus.workState <= 2) DeviceStatus.workState = 16;
        }
        showFunction(DeviceStatus.workState, 1);
        showTemp(Temperature[DeviceStatus.workState], ON);
        DeviceStatus.flashLight = 0;
    }
    else if(DeviceStatus.enterMode == ENTER_SET_UP_DOWN_TEMP)       // Code按键设置上下管温度状态
    {
        if(DeviceStatus.startWork == OFF)
        {
            KeyBeep();
            if(DeviceStatus.hotUpDown == HOT_UP) //上管
            {
                if(-1 == direction)
                {
                    if(++DeviceStatus.up_Temperature >= 232) DeviceStatus.up_Temperature = 40;
                }
                else
                {
                    if(--DeviceStatus.up_Temperature <= 38) DeviceStatus.up_Temperature = 230;
                }
                
                showTemp(DeviceStatus.up_Temperature, ON);
                showSymbol(SYMBOL_UP);
            }
            else if(DeviceStatus.hotUpDown == HOT_DOWN)    //下管
            {
                if(-1 == direction)
                {
                    if(DeviceStatus.workState == 14) 
                    {
                        if(++DeviceStatus.down_Temperature >= 62) DeviceStatus.down_Temperature = 30;
                    }
                    else 
                    {
                        if(++DeviceStatus.down_Temperature >= 232) DeviceStatus.down_Temperature = 40;
                    }
                }
                else
                {
                    if(DeviceStatus.workState == 14) 
                    {
                        if(--DeviceStatus.down_Temperature <= 28) DeviceStatus.down_Temperature = 60;
                    }
                    else
                    {
                        if(--DeviceStatus.down_Temperature <= 38) DeviceStatus.down_Temperature = 230;
                    }
                }
                
                showTemp(DeviceStatus.down_Temperature, ON);
                showSymbol(SYMBOL_DOWN);
            }
            DeviceStatus.flashLight = 0;
        }
    }
    else if(DeviceStatus.enterMode == ENTER_SET_TIME)               // Code按键设置工作时间状态
    {
        if(DeviceStatus.startWork == OFF)
        {
            KeyBeep();
            if(-1 == direction)
            {
                if(DeviceStatus.workTime <= 119) DeviceStatus.workTime++;
                else
                {
                    DeviceStatus.workTime += 10;
                    if(DeviceStatus.workTime >= 490) DeviceStatus.workTime = 1;
                }
            }
            else
            {
                if(DeviceStatus.workTime <= 200)
                {
                    if(DeviceStatus.workTime == 1) DeviceStatus.workTime = 480;
                    else DeviceStatus.workTime--;
                } 
                else DeviceStatus.workTime -= 10;
            }
            
            showTime(DeviceStatus.workTime, ON, ON);
            DeviceStatus.flashLight = 0;
        }
    }
    else if(DeviceStatus.enterMode == ENTER_START_WORK)             // Code按键开始工作状态
    {
        // 未使用。
    }
}