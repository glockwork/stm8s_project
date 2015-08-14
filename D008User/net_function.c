#include "net_function.h"


NET_RECV    NetMode = {0};
uint8_t RxRecvBuffer[BUFFERSIZE] = {0};
uint16_t DataSize = 0;
extern DEVICE_STATUS    DeviceStatus;

/*
  �������ܣ���ȡNext����
  ���� T�����ַ���ָ��
  ���� *next��Next����ָ��
  */
void get_next(String T, uint16_t *next)
{
    uint16_t j = 0;  //ǰ׺
    uint16_t i = 1;  //��׺
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
  �������ܣ��������ַ��������ַ�����pos���ַ�֮���λ�ã��������ڣ��򷵻�0
  ���� S�����ַ���ָ��
  ���� T�����ַ���ָ��
  ���� pos�����ַ����ĵ�ǰ�±�
  */
uint16_t Index_KMP(String S, String T, uint16_t pos)
{
    uint16_t i = pos;    //���ַ�����ǰ�±�
    uint16_t j = 1;      //���ַ�����ǰ�±�

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
            if(0 == stringCMP(&Message[8], "AAA", 3))   // ��ʱ�������� 41H
            {
                // ��ʱ����
                if('0' == Message[12])
                {
                    CancelKey();
                    Relay_Off_All();
                    FunctionReport(16);
                }
                else if('1' == Message[12])
                {
                    if(ENTER_START_WORK != DeviceStatus.enterMode)
                    {
                        SelectedFunction();                                         // ��ȡĬ�ϵ�ʱ�䡢�¶ȵȲ�����

                        showFunction(DeviceStatus.workState, ON);                   // �ù���ָʾ�Ƴ���
                        showTime(DeviceStatus.workTime, ON, ON);                    // ��ʾ����ʱ��
                        
                        DeviceStatus.beepSW = ON;
                        DeviceStatus.startWorkBeep = ON;                            // ������ʾ��
                        DeviceStatus.beep = 0;
                        
                        DeviceStatus.knob = KNOB_DISABLE;                           // ������ť
                        DeviceStatus.setMode = SET_TIME;                            // ����ʱ��
                        DeviceStatus.preheat = OFF;                                 // ��App����ʱ��Ԥ�ȹ��ܡ�
                        showPreheat(ON);                                            // ��Ԥ��ָʾ����ʾ�� �����Ԥ�����������򲻻���
                        if(DeviceStatus.preheat == ON)
                        {
                            DeviceStatus.startWork = OFF;                           // �������ָʾ��־
                            DeviceStatus.enterMode = ENTER_PREHEAT;                 // Ԥ�ȹ���״̬
                        }
                        else
                        {
                            DeviceStatus.startWork = ON;                            // ���ù���ָʾ��־
                            DeviceStatus.enterMode = ENTER_START_WORK;              // ����ʱ����
                        }
                        if(DeviceStatus.workState == 8) RELAY_3_H;                  // �������� ��ת��
                    }
                    else
                    {
                        KeyBeep();
                    }
                }
            }
            else if(0 == stringCMP(&Message[8], "AAB", 3))      // �Ե�   42H
            {
                SetFunction(16);
            }
            else if(0 == stringCMP(&Message[8], "AAC", 3))      // Ԥ���Ե� 43H
            {
                SetFunction(15);
            }
            else if(0 == stringCMP(&Message[8], "AAD", 3))      // ���� 44H
            {
                SetFunction(14);
            }
            else if(0 == stringCMP(&Message[8], "AAE", 3))      // ���� 45H
            {
                SetFunction(13);
            }
            else if(0 == stringCMP(&Message[8], "AAF", 3))      // ���� 46H
            {
                SetFunction(12);
            }
            else if(0 == stringCMP(&Message[8], "AAG", 3))      // ���� 47H
            {
                SetFunction(11);
            }
            else if(0 == stringCMP(&Message[8], "AAH", 3))      // ��̢ 48H
            {
                SetFunction(10);
            }
            else if(0 == stringCMP(&Message[8], "AAI", 3))      // ��� 49H
            {
                SetFunction(9);
            }
            else if(0 == stringCMP(&Message[8], "AAJ", 3))      // ���� 4AH
            {
                SetFunction(8);
            }
            else if(0 == stringCMP(&Message[8], "AAK", 3))      // ���� 4BH
            {
                SetFunction(7);
            }
            else if(0 == stringCMP(&Message[8], "AAL", 3))      // �Ź� 4CH
            {
                SetFunction(6);
            }
            else if(0 == stringCMP(&Message[8], "AAM", 3))      // ���� 4DH
            {
                SetFunction(5);
            }
            else if(0 == stringCMP(&Message[8], "AAN", 3))      // ���� 4EH
            {
                SetFunction(4);
            }
            else if(0 == stringCMP(&Message[8], "AAO", 3))      // ���� 4FH
            {
                SetFunction(3);
            }
            else if(0 == stringCMP(&Message[8], "AAP", 3))      // Ԥ�� 50H
            {
//                DeviceStatus.workState = 2;
//                DeviceStatus.enterMode = ENTER_CHOICE_FUNCTION;
                showPreheat(ON);
                KeyBeep();
            }
            else if(0 == stringCMP(&Message[8], "AAQ", 3))      // ¯�� 51H
            {
                //DeviceStatus.workState = 1;
                //DeviceStatus.enterMode = ENTER_CHOICE_FUNCTION;
                // ¯�ƿ���
                if('0' == Message[12])
                {
                  RELAY_1_L;
                  DeviceStatus.light = 0;
                }
                else if('1' == Message[12])
                {
                  RELAY_1_H;
                  DeviceStatus.light = 1;
                }
                showLight();
                KeyBeep();
            }
            else if(0 == stringCMP(&Message[8], "AAR", 3))  // 52H
            {
                // DIY
                KeyBeep();
            }
            else
            {
                KeyBeep();
            }
        }
    }
}

static uint8_t PINGR_ESPONSE[6] = {0, 4, 0x40, 0, 0, 0};

void PingResponse(uint8_t* MessageID)
{

  PINGR_ESPONSE[4] = MessageID[0];
  PINGR_ESPONSE[5] = MessageID[1];
  SEND(PINGR_ESPONSE, 6);
}

static uint8_t FUNCTION_RESPONSE[12] = {0, 10, 0x51, 0x44, 0, 0, 0, 0xFF, 0, 0, 0, 1};

void FunctionResponse(uint8_t *MessageID, uint8_t Token)
{
  FUNCTION_RESPONSE[4] = MessageID[0];
  FUNCTION_RESPONSE[5] = MessageID[1];
  FUNCTION_RESPONSE[6] = Token;
  
  SEND(FUNCTION_RESPONSE, 12);   // 
}

static uint8_t FUNCTION_REPORT[13] = {0, 11, 0x50, 0x02, 0, 0, 0xB1, 'e', 0x01, 'A', 0xFF, 'A', 0};
// ������͹�������״̬��ֻҪ�ں���Ӳ�����1���͡�0�� ���ɡ��磺00 0D 50 02 00 00 B1 65 01 41 FF 41 41 2C 31 
static uint8_t FUNCTION_REPORT_VALUE[15] = {0, 13, 0x50, 0x02, 0, 0, 0xB1, 'e', 0x01, 'A', 0xFF, 'A', 'A', ',', 0};
static uint8_t FUNCTION_NAME[17] = {0, 0, 0, 'O', 'N', 'M', 'L', 'K', 'J', 'I', 'H', 'G', 'F', 'E', 'D', 'C', 'B'};
static uint16_t messageid = 0;
void FunctionReport(uint8_t function)
{
    if(function < 3) return;
    
    if(++messageid >= 1024) messageid = 0;

    FUNCTION_REPORT[4] = messageid >> 8;
    FUNCTION_REPORT[5] = messageid & 0x00FF;
    FUNCTION_REPORT[12] = FUNCTION_NAME[function];
    
    SEND(FUNCTION_REPORT, 13);
}

void FunctionReportValue(uint8_t function, uint8_t value)
{
    if(++messageid >= 1024) messageid = 0;

    FUNCTION_REPORT_VALUE[4] = messageid >> 8;
    FUNCTION_REPORT_VALUE[5] = messageid & 0x00FF;
    FUNCTION_REPORT_VALUE[12] = function;
    FUNCTION_REPORT_VALUE[14] = value;
    
    SEND(FUNCTION_REPORT_VALUE, 15);
}

/*
����: void DataResolve(uint8_t *Data, uint16_t Length)
����: ���ݷֽ�
�β�: *Data ����ָ��, Length ���ݳ���
����ֵ����
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
#if 0
        UART3_SendString(RxDataBuffer[i], RxLength[i]);
#endif
    }

    Length = 0;
    Index = 0;
}


/*
����: uint16_t strlen(String str)
����: �����ַ�������
�β�: �ַ���ָ��
����ֵ���ַ�������
*/ 
uint16_t strlen(String str)
{
    uint16_t i = 0;
    while((*str++) != '\0') i++;
    return i;
}