#include "net_function.h"


NET_RECV    NetMode = {0};
uint8_t RxRecvBuffer[BUFFERSIZE] = {0};
uint16_t DataSize = 0;
extern DEVICE_STATUS    DeviceStatus;

/*
  函数功能：获取Next数组
  参数 T：子字符串指针
  参数 *next：Next数组指针
  */
void get_next(String T, uint16_t *next)
{
    uint16_t j = 0;  //前缀
    uint16_t i = 1;  //后缀
    next[1] = 0;

    while (i < T[0])
    {
        if (0 == j || T[i] == T[j])
        {
            i++;
            j++;
            if (T[i] != T[j]) next[i] = j;
            else next[i] = next[j];
        }
        else j = next[j];
    }
}

/*
  函数功能：返回子字符串在主字符串第pos个字符之后的位置，若不存在，则返回0
  参数 S：主字符串指针
  参数 T：子字符串指针
  参数 pos：主字符串的当前下标
  */
uint16_t Index_KMP(String S, String T, uint16_t pos)
{
    uint16_t i = pos;    //主字符串当前下标
    uint16_t j = 1;      //子字符串当前下标

    uint16_t next[255];
    get_next(T, next);

    while (i <= S[0] && j <= T[0])
    {
        if (0 == j || S[i] == T[j])
        {
            i++;
            j++;
        }
        else j = next[j];
    }
    if (j > T[0]) return i - T[0];
    else return 0;
}

uint8_t stringCMP(uint8_t *str1, uint8_t *str2, uint8_t len)
{
    uint8_t i;
    for(i = 0; i < len; i++)
    {
        if(str1[i] != str2[i]) return 1;
    }
    return 0;
}

void ResolveMessage(uint8_t *Message)
{
    switch(Message[0])
    {
    case 0x40:
        PingResponse(&Message[2]);
        break;
        
    case 0x41:
        if(0x02 == Message[1])
        {
            GetMessageFunction(Message);
            FunctionResponse(&Message[2], Message[4]);
        }
        break;
    }
}

void GetMessageFunction(uint8_t* Message)
{
    if('f' == Message[6])
    {
        if(0x03 == Message[7])
        {
            if(0 == stringCMP(&Message[8], "AAA", 3))
            {
                // 计时加热
                if('0' == Message[12])
                {
                    DeviceStatus.startWork = OFF;                               // 清除工作指示标志
        //            showTime(DeviceStatus.workTime, ON, ON);                    // 显示工作时间
                    DeviceStatus.knob = OFF;                                    // 启用旋钮              
                    DeviceStatus.setMode = SET_TIME;                            // 标志位设置时间
                    DeviceStatus.enterMode = ENTER_PAUSE_WORK;
                }
                else if('1' == Message[12])
                {
                    if(0 == DeviceStatus.workTime) DeviceStatus.workTime = Timing[DeviceStatus.workState];      // 获取默认工作时间
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
                        DeviceStatus.startWork = ON;                            // 设置工作指示标志
                        DeviceStatus.enterMode = ENTER_START_WORK;              // 倒计时加热
                    }
                }
                KeyBeep();
            }
            else if(0 == stringCMP(&Message[8], "AAB", 3))
            {
                SetFunction(16);
            }
            else if(0 == stringCMP(&Message[8], "AAC", 3))
            {
                SetFunction(15);
            }
            else if(0 == stringCMP(&Message[8], "AAD", 3))
            {
                SetFunction(14);
            }
            else if(0 == stringCMP(&Message[8], "AAE", 3))
            {
                SetFunction(13);
            }
            else if(0 == stringCMP(&Message[8], "AAF", 3))
            {
                SetFunction(12);
            }
            else if(0 == stringCMP(&Message[8], "AAG", 3))
            {
                SetFunction(11);
            }
            else if(0 == stringCMP(&Message[8], "AAH", 3))
            {
                SetFunction(10);
            }
            else if(0 == stringCMP(&Message[8], "AAI", 3))
            {
                SetFunction(9);
            }
            else if(0 == stringCMP(&Message[8], "AAJ", 3))
            {
                SetFunction(8);
            }
            else if(0 == stringCMP(&Message[8], "AAK", 3))
            {
                SetFunction(7);
            }
            else if(0 == stringCMP(&Message[8], "AAL", 3))
            {
                SetFunction(6);
            }
            else if(0 == stringCMP(&Message[8], "AAM", 3))
            {
                SetFunction(5);
            }
            else if(0 == stringCMP(&Message[8], "AAN", 3))
            {
                SetFunction(4);
            }
            else if(0 == stringCMP(&Message[8], "AAO", 3))
            {
                SetFunction(3);
            }
            else if(0 == stringCMP(&Message[8], "AAP", 3))
            {
//                DeviceStatus.workState = 2;
//                DeviceStatus.enterMode = ENTER_CHOICE_FUNCTION;
                showPreheat(ON);
                KeyBeep();
            }
            else if(0 == stringCMP(&Message[8], "AAQ", 3))
            {
                //DeviceStatus.workState = 1;
                //DeviceStatus.enterMode = ENTER_CHOICE_FUNCTION;
                RELAY_1_H;
                DeviceStatus.light = 1;
                showLight();
                KeyBeep();
            }
            else if(0 == stringCMP(&Message[8], "AAR", 3))
            {
                // DIY
              KeyBeep();
            }
        }
    }
}

void PingResponse(uint8_t* MessageID)
{
  uint8_t response[6] = {0};
  
  response[0] = 0;
  response[1] = 4;
  
  response[2] = 0x60;
  response[3] = 0x00;
  response[4] = MessageID[0];
  response[5] = MessageID[1];
  
  SEND(response, 6);
}

void FunctionResponse(uint8_t *MessageID, uint8_t Token)
{
  uint8_t response[12] = {0};
  
  response[0] = 0;
  response[1] = 10;     // response Lenght
  
  response[2] = 0x51;   // 
  response[3] = 0x44;
  response[4] = MessageID[0];
  response[5] = MessageID[1];
  response[6] = Token;
  response[7] = 0xFF;
  response[8] = 0x00;
  response[9] = 0x00;
  response[10] = 0x00;
  response[11] = 0x01;
  
  SEND(response, 12);   // 
}

  

void FunctionReport(uint8_t function)
{
    static uint16_t messageid = 0;
    uint8_t report[13] = {0};
    
    if(++messageid >= 1024) messageid = 0;
    
    report[0] = 0;
    report[1] = 13;
    report[2] = 0x50;
    report[3] = 0x02;
    report[4] = messageid >> 8;
    report[5] = messageid & 0x00FF;
    report[6] = 0xB1;
    report[7] = 'e';
    report[8] = 0x02;
    report[9] = 'A';
    report[10] = 'A';
    report[11] = 0xFF;
    report[12] = ;
    
    SEND(report, 13);
}

/*
名称: void DataResolve(uint8_t *Data, uint16_t Length)
功能: 数据分解
形参: *Data 数据指针, Length 数据长度
返回值：无
*/ 
void DataResolve(String Data, uint16_t Length)
{
    uint8_t RxDataBuffer[10][BUFFERSIZE] = {0};
    uint16_t RxLength[10] = {0};
    uint16_t Index = 0;
    uint16_t Recv, i;
    static uint8_t End = 0;

    for(i = 0; i < Length; i++)
    {
        Recv = Data[i];

        if(End == 1)
        {
            if(Recv == '\n')
            {
//                RxDataBuffer[Index][0] = RxLength[Index]-1;
                Index++;
            }
            End = 0;
        }
        else
        {
            if(Recv == '\r') End = 1;
            else
            {   
//                if(RxLength[Index] == 0) RxLength[Index]++;
                RxDataBuffer[Index][RxLength[Index]++] = Recv;
                if(RxLength[Index] > BUFFERSIZE-1) RxLength[Index] = 0;
            }
        }
    }

    for(i = 0; i < Index; i++)
    {
        ResolveMessage(RxDataBuffer[i]);
        UART3_SendString(RxDataBuffer[i], RxLength[i]);
    }

    Length = 0;
    Index = 0;
}


/*
名称: uint16_t strlen(String str)
功能: 计算字符串长度
形参: 字符串指针
返回值：字符串长度
*/ 
uint16_t strlen(String str)
{
    uint16_t i = 0;
    while((*str++) != '\0') i++;
    return i;
}