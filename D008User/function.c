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
//������ť˳ʱ�뷽��תһ��
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

//������ť��ʱ�뷽��תһ��
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
// ��TM1628����һ���ֽ� 
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

// ��TM1628��������
void command(uint8_t command)
{
    STB_H;
    STB_L;
    send_8bit(command);
}

// TM1628��ʾ��������
void showAll(void)
{
    uint8_t i;
    
    command(0x03);      // ������ʾģʽ�� 7λ10��ģʽ
#if 1
    command(0x40);      // ��ַ�Զ���һģʽ
#else
    command(0x44);      // �̶���ַģʽ
#endif
    command(0xC0);      // ������ʾ��ַ�� 00H��ʼ
    
    for(i = 0; i < 7; i++)
    {
        send_8bit(0xFF);        // ��00H�� ż��ַд��ʾ����
        send_8bit(0x00);        // ��ΪSEG9-14��δ�õ����������ַȫд0
    }
    
    command(0x89);      //����ʾ��1/16ռ�ձȡ�
    STB_H;
}

// TM1628���������ʾ
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
    
    command(0x89);      //����ʾ��1/16ռ�ձȡ�
    STB_H;
}

// TM1628��ʾ����ָʾ��
// function�����ܣ�state��״̬-ON��ʾ OFF����ʾ
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

// TM1628��ʾ¯��ָʾ��
void showLight(void)
{
    uint8_t data = 0x08;

    if(DeviceStatus.light == OFF) data = 0;
    if(DeviceStatus.preheat == ON) data |= 0x01;
  
    command(0xC2);
    send_8bit(data);

    STB_H;
}

// TM1628��ʾԤ��ָʾ��
void showPreheat(uint8_t sw)
{
    uint8_t data  = 0x01;       // Ԥ��ָʾ��
    
    if(DeviceStatus.preheat == ON) data = 0x01;
    else data = 0;
    
    if(DeviceStatus.light == ON) data |= 0x08;
    
    command(0xC2);
    if(sw == ON) send_8bit(data);
    else send_8bit(data & 0x08);
    
    STB_H;
}

//TM1628 ��ʾ�����
// index���ڼ�������ܣ�1-4�� number������
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

// TM1628�������ʾʱ��
// time��ʱ�䣬 sw-ON����ʾ��OFF����ʾ�� dp���Ƿ���ʾ�㣬 ON����ʾ��OFF����ʾ
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

// TM1628 �������ʾ�¶�ֵ
// temp���¶�ֵ�� sw��ON-��ʾ��OFF-����ʾ
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

// TM1628 �������ʾ�¶ȷ���C
// symbol ��Ĭ��λ�á���λ�á���λ��
void showSymbol(uint8_t symbol)
{
    command(0xC8);
    if(SYMBOL_DEFAULT == symbol) send_8bit(num[11]);          // Ĭ���¶�
    else if(SYMBOL_UP == symbol) send_8bit(0x85);       // �Ϲ��¶�
    else if(SYMBOL_DOWN == symbol) send_8bit(0x98);       // �¹��¶�
    STB_H;
}

// TM1628 �������ʾ��OFF��
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

// ��ȡTM1628����״̬
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

// TM1628�����������
void key_process(void)
{
    static uint8_t key1 = 0, key2 = 0;
    
    if(key1 != KEY[0])
    {
        if(KEY[0] & 0x08) // �������¹��¶�
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
        if(KEY[0] & 0x01) // ¯�ƿ���
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
        if(KEY[1] & 0x08) // ȡ����
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
        if(KEY[1] & 0x01) // ����ʱ��
        {
            if(DeviceStatus.enterMode != ENTER_DEFINE)
            {
                
                if(DeviceStatus.enterMode == ENTER_SET_UP_DOWN_TEMP)
                {
                    if(DeviceStatus.startWork == OFF)   // �Ƿ��ڹ���״̬
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

//��ȡ��ť�仯״̬
void CodingSwitchPolling(void)
{ 
    static unsigned char st = 0, Aold = 0, Bold = 0;
    if(CodingSwitch_A != RESET && CodingSwitch_B != RESET) st = SET;
    if(st == SET)
    {
        if(CodingSwitch_A == RESET && CodingSwitch_B == RESET)   //�����ǰ���뿪�ص��������Ŷ�Ϊ�׵�ƽִ������Ĳ��� 
        {
            if(Aold != RESET)                                  //BΪ��˵�����뿪������Ӵ�ķ���ת
            {
                st = RESET;
//                UART1_SendByte(DeviceStatus.state);
                CodeProcess(1);
            } 
            else if(Bold != RESET)                   //else����ʡ! ʡ��������ͬʱ�Ӽ�
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

//������ť�ķ������ť������״̬��ִ����Ӧ�Ĵ���
// direction: ����1��-1
void CodeProcess(int8_t direction)
{
    if(DeviceStatus.enterMode == ENTER_DEFINE)                      // Code����Ĭ��״̬
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
    else if(DeviceStatus.enterMode == ENTER_SET_UP_DOWN_TEMP)       // Code�����������¹��¶�״̬
    {
        if(DeviceStatus.startWork == OFF)
        {
            KeyBeep();
            if(DeviceStatus.hotUpDown == HOT_UP) //�Ϲ�
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
            else if(DeviceStatus.hotUpDown == HOT_DOWN)    //�¹�
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
    else if(DeviceStatus.enterMode == ENTER_SET_TIME)               // Code�������ù���ʱ��״̬
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
    else if(DeviceStatus.enterMode == ENTER_START_WORK)             // Code������ʼ����״̬
    {
        // δʹ�á�
    }
}