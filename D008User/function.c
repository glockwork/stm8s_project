#include "function.h"
#include <math.h>

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
    DeviceStatus.startWorkBeep = OFF;
    DeviceStatus.beepSW = ON;
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
    
    if(DeviceStatus.enterMode != ENTER_PREHEAT)
    {
        if((DeviceStatus.preheat == ON) && (DeviceStatus.flashLight == 1)) data |= 0x01;
    }
    else data |= 0x01;
    
    if(DeviceStatus.workState < 9) data |= led[DeviceStatus.workState-1];
  
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
    
    if(DeviceStatus.workState < 9) data |= led[DeviceStatus.workState-1];
    
    command(0xC2);
    if(sw == ON) send_8bit(data);
    else send_8bit(data & 0xFE);
    
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
            SetHotUpOrDown();
        }
        if(KEY[0] & 0x01) // 炉灯开关
        {
            LightSwitch();
        }
        key1 = KEY[0];
    }
    if(key2 != KEY[1])
    {
        if(KEY[1] & 0x08) // 取消键
        {
            Relay_Off_All();
            CancelKey();
        }
        if(KEY[1] & 0x01) // 设置时间
        {
            SetTimeKey();
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
    if(DeviceStatus.setMode == SET_FUNCTION)                      // Code按键默认状态, 选择功能
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
        showFunction(DeviceStatus.workState, ON);
        showTemp(Temperature[DeviceStatus.workState], ON);
        FunctionReport(DeviceStatus.workState);
        DeviceStatus.flashLight = 0;
    }
    else if(DeviceStatus.setMode == SET_TEMP)
    {
        if(DeviceStatus.knob != KNOB_DISABLE)
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
    else if(DeviceStatus.setMode == SET_TIME)
    {
        if(DeviceStatus.knob != KNOB_DISABLE)
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
}

void DeviceRemind(void) // 设备指示灯提示
{
    if(DeviceStatus.enterMode == ENTER_CHOICE_FUNCTION)
    {
        if(DeviceStatus.flashLight == 1) showFunction(DeviceStatus.workState, ON);
        else if(DeviceStatus.flashLight >= 2) 
        {
            showFunction(DeviceStatus.workState, OFF);
            DeviceStatus.flashLight = 0;
        }
    }
    else if(DeviceStatus.enterMode == ENTER_SELECTED_FUNCTION)
    {
        if(DeviceStatus.flashLight == 1)
        {
            if(DeviceStatus.setMode == SET_TEMP)
            {
                if(DeviceStatus.hotUpDown == HOT_UP) showTemp(DeviceStatus.up_Temperature, ON);
                else if(DeviceStatus.hotUpDown == HOT_DOWN) showTemp(DeviceStatus.down_Temperature, ON);
                showSymbol(DeviceStatus.hotUpDown);
            }
            else if(DeviceStatus.setMode == SET_TIME)
            {
                showTime(DeviceStatus.workTime, ON, ON);
            }
            if(DeviceStatus.preheat == ON) showPreheat(ON);
        }
        else if(DeviceStatus.flashLight >= 2) 
        {
            if(DeviceStatus.setMode == SET_TEMP)
            {
                if(DeviceStatus.hotUpDown == HOT_UP) showTemp(DeviceStatus.up_Temperature, OFF);
                else if(DeviceStatus.hotUpDown == HOT_DOWN) showTemp(DeviceStatus.down_Temperature, OFF);
                DeviceStatus.flashLight = 0;
            }
            else if(DeviceStatus.setMode == SET_TIME)
            {
                showTime(DeviceStatus.workTime, OFF, ON);
                DeviceStatus.flashLight = 0;
            }
            if(DeviceStatus.preheat == ON) showPreheat(OFF);
        }
    }
    else if(DeviceStatus.enterMode == ENTER_PREHEAT)
    {
        if(DeviceStatus.flashLight >= 10)       // 5秒后自动返回到显示时间状态
        {
            if(DeviceStatus.setMode == SET_TEMP)
            {
                showTime(DeviceStatus.workTime, ON, ON);
                DeviceStatus.setMode = SET_TIME;
            }
            DeviceStatus.flashLight = 0;
        }
    }
    else if(DeviceStatus.enterMode == ENTER_START_WORK)
    {
        if(DeviceStatus.flashLight == 1)
        {
            if(DeviceStatus.setMode == SET_TEMP)
            {
                if(DeviceStatus.hotUpDown == HOT_UP) showTemp(DeviceStatus.up_Temperature, ON);
                else if(DeviceStatus.hotUpDown == HOT_DOWN) showTemp(DeviceStatus.down_Temperature, ON);
                showSymbol(DeviceStatus.hotUpDown);
            }
            else if(DeviceStatus.setMode == SET_TIME)
            {
                showTime(DeviceStatus.workTime, ON, ON);
            }
        }
        else if(DeviceStatus.flashLight == 2) 
        {
            if(DeviceStatus.setMode == SET_TIME)
            {
                showTime(DeviceStatus.workTime, ON, OFF);
                DeviceStatus.flashLight = 0;
            }
        }
        else if(DeviceStatus.flashLight >= 10)
        {
            if(DeviceStatus.setMode == SET_TEMP)
            {
                showTime(DeviceStatus.workTime, ON, ON);
                DeviceStatus.setMode = SET_TIME;
            }
            DeviceStatus.flashLight = 0;
        }
    }
    else if(DeviceStatus.enterMode == ENTER_PAUSE_WORK)
    {
        if(DeviceStatus.flashLight == 1)
        {
            if(DeviceStatus.setMode == SET_TEMP)
            {
                if(DeviceStatus.hotUpDown == HOT_UP) showTemp(DeviceStatus.up_Temperature, ON);
                else if(DeviceStatus.hotUpDown == HOT_DOWN) showTemp(DeviceStatus.down_Temperature, ON);
                showSymbol(DeviceStatus.hotUpDown);
            }
            else if(DeviceStatus.setMode == SET_TIME)
            {
                showTime(DeviceStatus.workTime, ON, ON);
            }
        }
        else if(DeviceStatus.flashLight >= 2) 
        {
            if(DeviceStatus.setMode == SET_TEMP)
            {
                if(DeviceStatus.hotUpDown == HOT_UP) showTemp(DeviceStatus.up_Temperature, OFF);
                else if(DeviceStatus.hotUpDown == HOT_DOWN) showTemp(DeviceStatus.down_Temperature, OFF);
                DeviceStatus.flashLight = 0;
            }
            else if(DeviceStatus.setMode == SET_TIME)
            {
                showTime(DeviceStatus.workTime, OFF, ON);
                DeviceStatus.flashLight = 0;
            }
        }
    }
}

void SetFunction(uint8_t function)
{
    DeviceStatus.workState = function;
    DeviceStatus.enterMode = ENTER_CHOICE_FUNCTION;
    DeviceStatus.setMode = SET_FUNCTION;
    showFunction(DeviceStatus.workState, ON);
    showTemp(Temperature[DeviceStatus.workState], ON);
    showSymbol(SYMBOL_DEFAULT);
    KeyBeep();
}

void LightSwitch(void)
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

void CancelKey(void)
{
    DeviceStatus.enterMode = ENTER_CHOICE_FUNCTION;
    DeviceStatus.preheat = OFF;
    DeviceStatus.startWork = OFF;
    DeviceStatus.workState = 16;
    DeviceStatus.workTime = 0;
    DeviceStatus.hotUpDown = HOT_UP;
    DeviceStatus.up_Temperature = 0;
    DeviceStatus.down_Temperature = 0;
    DeviceStatus.temperatureOK = NO;
    DeviceStatus.setMode = SET_FUNCTION;
    showFunction(DeviceStatus.workState, ON);
    showTemp(Temperature[DeviceStatus.workState], ON);
    showSymbol(SYMBOL_DEFAULT);
    KeyBeep();
}

void SetHotUpOrDown(void)
{
    if(DeviceStatus.enterMode != ENTER_CHOICE_FUNCTION)
    {
        if(14 != DeviceStatus.workState)        // 14号功能只有下管
        {
            if(DeviceStatus.setMode == SET_TEMP)    // 只有在设置温度模式下才能切换上/下管
            {
                if(HOT_DOWN == DeviceStatus.hotUpDown)
                {
                    DeviceStatus.hotUpDown = HOT_UP;
                }
                else if(HOT_UP == DeviceStatus.hotUpDown) 
                {
                    DeviceStatus.hotUpDown = HOT_DOWN;
                }
            }
          
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
        KeyBeep();
        DeviceStatus.setMode = SET_TEMP;
    }
    DeviceStatus.flashLight = 0;
}

void SetTimeKey(void)
{
    if(DeviceStatus.enterMode != ENTER_CHOICE_FUNCTION)
    {
        showTime(DeviceStatus.workTime, ON, ON);
        KeyBeep();
        DeviceStatus.flashLight = 0;
        DeviceStatus.setMode = SET_TIME;
    }
}


/*
名称: uint16_t MathData(uint8_t *Data)
功能: 求平均值
形参: *Data 数据缓冲区地址
返回值：数据的平均值。
*/ 
uint16_t MathData(uint16_t *Data)
{
    uint8_t i, j;
    uint16_t temp;
    uint32_t value;

    for(i = 0; i < 50; i++)
    {
        for(j = i; j < 50; j++)
        {
              if(Data[i] < Data[j])
              {
                    temp = Data[i];
                    Data[i] = Data[j];
                    Data[j] = temp;
              }
        }
    }

    value = 0;
    for(i = 10; i < 40; i++)
        value += Data[i];

    temp = value / 30;

    return temp;
}

uint16_t Get_UP_NTC_Value(void)
{
    uint16_t i, j, temp, buf[50];

    GPIO_Init(GPIOE, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);
  
    ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL9, DISABLE);
    ADC2_ConversionConfig(ADC2_CONVERSIONMODE_SINGLE, ADC2_CHANNEL_9, ADC2_ALIGN_RIGHT);

    ADC2_StartConversion();

    for (i = 0; i < 50; i++)
    {
        ADC2_StartConversion();
        buf[i] = ADC2_GetConversionValue();
        for (j = 0; j < 0xFF; j++);
    }

    temp = MathData(buf);

    return temp;
}

uint16_t Get_DOWN_NTC_Value(void)
{
    uint16_t i, j, temp, buf[50];

    GPIO_Init(GPIOE, GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);
  
    ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL8, DISABLE);
    ADC2_ConversionConfig(ADC2_CONVERSIONMODE_SINGLE, ADC2_CHANNEL_8, ADC2_ALIGN_RIGHT);

    ADC2_StartConversion();

    for (i = 0; i < 50; i++)
    {
        ADC2_StartConversion();
        buf[i] = ADC2_GetConversionValue();
        for (j = 0; j < 0xFF; j++);
    }

    temp = MathData(buf);

    return temp;
}

#define B25     3435
#define R_ST    100000
// 温度越低电压越高、温度越高电压越低。
void AutoControl(uint8_t up_temp, uint8_t down_temp)
{
    static uint8_t up_temp_old = 0, down_temp_old = 0;
    static float Up_TempMax = 0, Up_TempMin = 0; 
    static float Down_TempMax = 0, Down_TempMin = 0; 
    float Vup = 0, Vdown = 0;
    uint16_t Rt;
    
    Vup = Get_UP_NTC_Value() * ( 5.0 / 1023);
    Vdown = Get_DOWN_NTC_Value() * ( 5.0 / 1023);
    
    if(DeviceStatus.workState != 14)    // 不为发酵功能
    {
        if((231 != up_temp) && (39 != up_temp))    // 不为关闭
        {
            if(up_temp != up_temp_old)
            {
                Rt = (int)(R_ST * exp(B25 * (1 / (273.15 + up_temp+10) - 1 / (273.15 + 25))));
                Up_TempMax = 5.0 / (4700 + Rt) * Rt;
                Rt = (int)(R_ST * exp(B25 * (1 / (273.15 + up_temp-10) - 1 / (273.15 + 25))));
                Up_TempMin = 5.0 / (4700 + Rt) * Rt;
                up_temp_old = up_temp;
            }
            
            if(Vup <= Up_TempMax)
            {
                RELAY_5_L;
            }
            else if(Vup >= Up_TempMin)
            {
                RELAY_5_H;
            }
        }
        
        if((231 != down_temp) && (39 != down_temp))
        {
            if(down_temp != down_temp_old)
            {
                Rt = (int)(R_ST * exp(B25 * (1 / (273.15 + down_temp+10) - 1 / (273.15 + 25))));
                Down_TempMax = 5.0 / (4700 + Rt) * Rt;
                Rt = (int)(R_ST * exp(B25 * (1 / (273.15 + down_temp-10) - 1 / (273.15 + 25))));
                Down_TempMin = 5.0 / (4700 + Rt) * Rt;
                down_temp_old = down_temp;
            }
        }
    }
    else
    {
        if((61 != down_temp) && (29 != down_temp))
        {
            if(down_temp != down_temp_old)
            {
                Rt = (int)(R_ST * exp(B25 * (1 / (273.15 + down_temp+2) - 1 / (273.15 + 25))));
                Down_TempMax = 5.0 / (4700 + Rt) * Rt;
                Rt = (int)(R_ST * exp(B25 * (1 / (273.15 + down_temp-2) - 1 / (273.15 + 25))));
                Down_TempMin = 5.0 / (4700 + Rt) * Rt;
                down_temp_old = down_temp;
            }
        }
        else return;
    }
    
    if(Vdown <= Down_TempMax)
    {
        if(DeviceStatus.workState == 14)
        {
            RELAY_2_L;
        }
        else
        {
            RELAY_4_L;
        }
    }
    else if(Vdown >= Down_TempMin)
    {
        if(DeviceStatus.workState == 14)
        {
            RELAY_2_H;
        }
        else
        {
            RELAY_4_H;
        }
    }
    
    if(DeviceStatus.preheat == ON)
    {
        if((Vdown < Down_TempMin) && (Vup < Up_TempMin))
        {
            DeviceStatus.temperatureOK = OK;
        }
    }
}

void Relay_Off_All(void)
{
    RELAY_2_L;
    RELAY_3_L;
    RELAY_4_L;
    RELAY_5_L;
}