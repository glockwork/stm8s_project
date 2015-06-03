#include "net_function.h"


NET_RECV    NetMode = {0};
uint8_t RxRecvBuffer[BUFFERSIZE] = {0};
uint16_t DataSize = 0;
enum {ModeReady, TestMode, SetMode, ConnectAP, ConnectServer, ServerJoin, ServerOK} NetStatus;

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

/*
����: void TestNetMode(void)
����: ���Ͳ�������
�β�: ��
����ֵ����
*/ 
void TestNetMode(void)
{
    UART1_SendString("AT\r\n", sizeof("AT\r\n")-1);
}

/*
����: void SetNetMode(uint8_t Mode)
����: ��������ģʽ����
�β�: Mode ģʽ
����ֵ����
*/ 
void SetNetMode(uint8_t Mode)
{
    switch(Mode)
    {
    case 1:
        UART1_SendString("AT+CWMODE=1\r\n", sizeof("AT+CWMODE=1\r\n")-1);
        break;
    case 2:
        UART1_SendString("AT+CWMODE=2\r\n", sizeof("AT+CWMODE=2\r\n")-1);
        break;
    case 3:
        UART1_SendString("AT+CWMODE=3\r\n", sizeof("AT+CWMODE=3\r\n")-1);
        break;
    default:
        UART1_SendString("AT+CWMODE?\r\n", sizeof("AT+CWMODE?\r\n")-1);
        break;
    }
}

/*
����: void ResetNetMode(void)
����: ���͸�λ����
�β�: ��
����ֵ����
*/ 
void ResetNetMode(void)
{
    UART1_SendString("AT+RST\r\n", sizeof("AT+RST\r\n")-1);
}

/*
����: void ClosedConnectet(void)
����: ���͹ر���������
�β�: ��
����ֵ����
*/ 
void ClosedConnectet(void)
{
    UART1_SendString("AT+CIPCLOSE\r\n", sizeof("AT+CIPCLOSE\r\n")-1);
}

/*
����: void ClosedAP(void)
����: ���͹ر���������
�β�: ��
����ֵ����
*/ 
void ClosedAP(void)
{
    UART1_SendString("AT+CWQAP\r\n", sizeof("AT+CWQAP\r\n")-1);
}

/*
����: void NetModeConnectAP(void)
����: ��������AP����
�β�: ��
����ֵ����
*/ 
void NetModeConnectAP(void)
{
    UART1_SendString(AP_Message, sizeof(AP_Message)-1);
}

/*
����: void NetModeConnectServer(void)
����: �������ӷ���������
�β�: ��
����ֵ����
*/ 
void NetModeConnectServer(void)
{
    UART1_SendString(Server_Message, sizeof(Server_Message)-1);
}


/*
����: void NetSendDataLength(void)
����: �������ݳ���
�β�: ��
����ֵ����
*/ 
void NetSendDataLength(void)
{
    UART1_SendString("AT+CIPSEND=10\r\n", sizeof("AT+CIPSEND=10\r\n")-1);
}

/*
����: void NetSendData(uint8_t *Data, uint16_t Length)
����: ��������
�β�: ��
����ֵ����
*/ 
void NetSendData(uint8_t *Data, uint16_t Length)
{
    UART1_SendString(Data, Length);
}

/*
����: void isRecvOK(uint8_t *RxRecvData)
����: �ж��Ƿ���յ��������Ƿ���"OK"
�β�: *RxRecvData ����ָ��
����ֵ����
*/ 
void isRecvOK(uint8_t *RxRecvData)
{
    uint8_t OK[] = " OK";
    uint8_t SEND_OK[] = " SEND OK";
    SEND_OK[0] = 7;
    OK[0] = 2;
    uint16_t index1, index2;

    index1 = Index_KMP(RxRecvData, SEND_OK, 1);
    index2 = Index_KMP(RxRecvData, OK, 1);

    if(index1 == 0 && index2 > 0) NetMode.Status |= NET_OK;
        //UART1_SendString("OK\n");
}


/*
����: void isRecvReady(uint8_t *RxRecvData)
����: �ж��Ƿ���յ��������Ƿ���"ready"
�β�: *RxRecvData ����ָ��
����ֵ����
*/ 
void isRecvReady(uint8_t *RxRecvData)
{
    uint8_t READY[] = " ready";
    READY[0] = 5;

    if(Index_KMP(RxRecvData, READY, 1)) NetMode.Status |= NET_READY;
    //UART1_SendString("ready\n");
}

/*
����: void isRecvError(uint8_t *RxRecvData)
����: �ж��Ƿ���յ��������Ƿ���"ERROR"
�β�: *RxRecvData ����ָ��
����ֵ����
*/ 
void isRecvError(uint8_t *RxRecvData)
{
    uint8_t ERROR[] = " ERROR";
    ERROR[0] = 5;
    if(Index_KMP(RxRecvData, ERROR, 1)) 
    {
        NetMode.Status |= NET_ERROR;
        NetMode.Status &= ~NET_CONNECT;
    }
    //UART1_SendString("ERROR\n");
}

/*
����: void isRecvData(uint8_t *RxRecvData)
����: �ж��Ƿ���յ��������Ƿ���"+IPD"
�β�: *RxRecvData ����ָ��
����ֵ����
*/ 
void isRecvData(uint8_t *RxRecvData)
{
    uint8_t Index = 0, i;
    uint8_t IPD[] = " +IPD";
    IPD[0] = 4;
    Index = Index_KMP(RxRecvData, IPD, 1);
    if(Index != 0)
    {
        NetMode.RecvDataSize = RxRecvData[Index+5]-0x30;
        for(i = 0; i < NetMode.RecvDataSize; i++)
        {
            NetMode.RecvData[i] = RxRecvData[Index+7+i];
        }
        UART3_SendString((uint8_t *)NetMode.RecvData, NetMode.RecvDataSize);
        NetMode.Status |= NET_RECV_DATA;
    }
}

/*
����: void isRecvSendOK(uint8_t *RxRecvData)
����: �ж��Ƿ���յ��������Ƿ���"SEND OK"
�β�: *RxRecvData ����ָ��
����ֵ����
*/ 
void isRecvSendOK(uint8_t *RxRecvData)
{
    uint8_t SEND_OK[] = " SEND OK";
    SEND_OK[0] = 7;

    if(Index_KMP(RxRecvData, SEND_OK, 1)) NetMode.Status |= NET_SEND_OK;
    //UART1_SendString("SEND OK\n");
}

/*
����: void isRecvSendFAIL(uint8_t *RxRecvData)
����: �ж��Ƿ���յ��������Ƿ���"SEND FAIL"
�β�: *RxRecvData ����ָ��
����ֵ����
*/ 
void isRecvSendFAIL(uint8_t *RxRecvData)
{
    uint8_t SEND_FAIL[] = " SEND FAIL";
    SEND_FAIL[0] = 9;

    if(Index_KMP(RxRecvData, SEND_FAIL, 1)) NetMode.Status |= NET_SEND_FAIL;
    //UART1_SendString("SEND OK\n");
}

/*
����: void isRecvClosed(uint8_t *RxRecvData)
����: �ж��Ƿ���յ��������Ƿ���"CLOSED"
�β�: *RxRecvData ����ָ��
����ֵ����
*/ 
void isRecvClosed(uint8_t *RxRecvData)
{
    uint8_t CLOSED[] = " CLOSED";
    CLOSED[0] = 6;

    if(Index_KMP(RxRecvData, CLOSED, 1))
    {
        NetMode.Status &= ~NET_CONNECT;
        NetMode.Status |= NET_CLOSED;
    }
    //UART1_SendString("CLOSED\n");
}

/*
����: void isRecvBusy(uint8_t *RxRecvData)
����: �ж��Ƿ���յ��������Ƿ���"busy"
�β�: *RxRecvData ����ָ��
����ֵ����
*/ 
void isRecvBusy(uint8_t *RxRecvData)
{
    uint8_t BUSY[] = " busy";
    BUSY[0] = 4;

    if(Index_KMP(RxRecvData, BUSY, 1)) NetMode.Status |= NET_BUSY;
    //UART1_SendString("Busy\n");
}

/*
����: void isRecvAL_Connected(uint8_t *RxRecvData)
����: �ж��Ƿ���յ��������Ƿ���"ALREADY CONNECTED"
�β�: *RxRecvData ����ָ��
����ֵ����
*/ 
void isRecvAL_Connected(uint8_t *RxRecvData)
{
    uint8_t AL_CONNECT[] = " ALREADY CONNECTED";
    AL_CONNECT[0] = 17;

    if(Index_KMP(RxRecvData, AL_CONNECT, 1)) NetMode.Status |= NET_ALREADY_CONNECTED;
}

/*
����: void isRecvFail(uint8_t *RxRecvData)
����: �ж��Ƿ���յ��������Ƿ���"FAIL"
�β�: *RxRecvData ����ָ��
����ֵ����
*/ 
void isRecvFail(uint8_t *RxRecvData)
{
    uint8_t FAIL[] = " FAIL";
    uint8_t SEND_FAIL[] = " SEND FAIL";
    SEND_FAIL[0] = 9;
    FAIL[0] = 4;
    uint16_t index1, index2;
    
    index1 = Index_KMP(RxRecvData, SEND_FAIL, 1);
    index2 = Index_KMP(RxRecvData, FAIL, 1);

    if(index2 > 0 && index1 == 0) NetMode.Status |= NET_FAIL;
}

/*
����: void isRecvConnect(uint8_t *RxRecvData)
����: �ж��Ƿ���յ��������Ƿ���"CONNECT"
�β�: *RxRecvData ����ָ��
����ֵ����
*/ 
void isRecvConnect(uint8_t *RxRecvData)
{
    uint8_t CONNECT[] = " CONNECT";
    uint8_t AL_CONNECT[] = " ALREADY CONNECTED";
    uint8_t CONNECT_FAILED[] = " CONNECT Failed";
    uint16_t index1, index2, index3;
    AL_CONNECT[0] = 17;
    CONNECT[0] = 7;
    CONNECT_FAILED[0] = 14;

    
    index2 = Index_KMP(RxRecvData, AL_CONNECT, 1);
    index3 = Index_KMP(RxRecvData, CONNECT_FAILED, 1);
    index1 = Index_KMP(RxRecvData, CONNECT, 1);

    if(index1 > 0 && index2 == 0 && index3 == 0) NetMode.Status |= NET_CONNECT;
}

/*
����: void isConnectFailed(uint8_t *RxRecvData)
����: �ж��Ƿ���յ��������Ƿ���"CONNECT Failed"
�β�: *RxRecvData ����ָ��
����ֵ����
*/ 
void isConnectFailed(uint8_t *RxRecvData)
{
    uint8_t CONNECT_FAILED[] = " CONNECT Failed";
    CONNECT_FAILED[0] = 14;

    if(Index_KMP(RxRecvData, CONNECT_FAILED, 1)) 
    {
        NetMode.Status &= ~NET_CONNECT;
        NetMode.Status |= NET_CONNECT_FAILED;
    }
}

/*
����: void GetNetFlags(uint8_t *RxRecvData)
����: ��������
�β�: *RxRecvData ����ָ��
����ֵ����
*/ 
void GetNetFlags(uint8_t *RxRecvData)
{
    isRecvReady(RxRecvData);
    isRecvBusy(RxRecvData);
    isRecvClosed(RxRecvData);
    isRecvSendOK(RxRecvData);
    isRecvOK(RxRecvData);
    isRecvConnect(RxRecvData);
    isRecvAL_Connected(RxRecvData);
    isRecvError(RxRecvData);
    isRecvFail(RxRecvData);
    isRecvData(RxRecvData);
    isConnectFailed(RxRecvData);
}

/*
����: void DataResolve(uint8_t *Data, uint16_t Length)
����: ���ݷֽ�
�β�: *Data ����ָ��, Length ���ݳ���
����ֵ����
*/ 
void DataResolve(uint8_t *Data, uint16_t Length)
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
                RxDataBuffer[Index][0] = RxLength[Index]-1;
                Index++;
            }
            End = 0;
        }
        else
        {
            if(Recv == '\r') End = 1;
            else if(Recv == '>') NetMode.Status |= NET_ACTION_SEND;
            else
            {   
                if(RxLength[Index] == 0) RxLength[Index]++;
                RxDataBuffer[Index][RxLength[Index]++] = Recv;
                if(RxLength[Index] > BUFFERSIZE-1) RxLength[Index] = 0;
            }
        }
    }

    for(i = 0; i < Index; i++)
    {
        GetNetFlags(RxDataBuffer[i]);
//        UART3_SendString(RxDataBuffer[i], 10);
    }

    Length = 0;
    Index = 0;
}

#define MAX_FIX         10              // ����Դ���
#define MAX_TIME        5*60            // �ظ����Լ��ʱ��
void NetModeErrorFix(void)
{
    static uint16_t TimeCount = 0;      // ��ʱ
    static uint16_t FixCount = 0;       // �޸�����
    static uint8_t ModeBreakDown = 0;   // ģ���𻵼���
    static uint8_t ErrorCode = 0;
    
    if(ErrorCode != NetMode.ErrorCode)  // ���������뷢���仯�������������㡣
    {
        TimeCount = 0;
        FixCount = 0;
        ErrorCode = NetMode.ErrorCode;
    }
    
    switch(NetMode.ErrorCode)
    {
    case 1:
        FixCount++;
        ResetNetMode();
        break;
    case 2:
        FixCount++;
        TestNetMode();
        break;
    case 3:
        FixCount++;
        SetNetMode(3); 
        break;
    case 4:
        FixCount++;
        NetModeConnectAP();
        break;
    case 5:
        if((NetMode.Status & NET_ERROR) && (NetMode.Status & NET_CLOSED)) NetModeConnectServer();
        
//        else if(NetMode.Status & NET_FAIL) NetMode.ErrorCode = 8;
        
        else if(NetMode.Status & NET_CONNECT_FAILED) 
        {
            NetStatus = ConnectServer;
            NetModeConnectAP();
        }
        FixCount++;
        break;

    case 80:            // �ظ�����
        TimeCount++;
        break;
    case 81:
        // ���ģ������ʾ����
        break;
    default:
        break;
    }
    
    if(FixCount == MAX_FIX) 
    {
        NetMode.ErrorCode = 80;
        ModeBreakDown++;
    }
    
    if(TimeCount == MAX_TIME) 
    {
        NetStatus = ModeReady;
        NetMode.ErrorCode = 0;
        if(ModeBreakDown == MAX_FIX) NetMode.ErrorCode = 81;         // ��ʶģ����
    }
} 

/*
����: void NetProcess(void)
����: ����ͨ�Ź���
�β�: ��
����ֵ����
*/ 
void NetProcess(void)
{
    static uint8_t ReConnectCount = 0;
    switch(NetStatus)
    {
    case ModeReady:     // �ϵ緢��ģ�鸴λ�����
        if(NetMode.Status & NET_READY)  // ģ��׼��OK
        {
            TestNetMode();
            NetStatus = TestMode;       // �������ģ��
            NetMode.ErrorCode = 0;
            NetMode.Status &= ~NET_READY;
        }
        else if(NetMode.Status & NET_ERROR) 
        {
            NetMode.ErrorCode = 1;
            NetMode.Status &= ~NET_ERROR;
        }
        break;
    case TestMode:      // ����ģ��
        if(NetMode.Status & NET_OK)     // ����ģ��OK
        {
            NetStatus = SetMode;        
            SetNetMode(3);              // ����ģ��
            NetMode.ErrorCode = 0;
        }
        else if(NetMode.Status & NET_ERROR) 
        {
            NetMode.ErrorCode = 2;
            NetMode.Status &= ~NET_ERROR;
        }
        break;
    case SetMode:               // ����ģ��
        if(NetMode.Status & NET_OK)     // ����OK
        {
            ResetNetMode();     
            NetStatus = ConnectAP;      // ��λģ�飬��������Ч����������AP
            NetMode.ErrorCode = 0;
        }
        else if(NetMode.Status & NET_ERROR) 
        {
            NetMode.ErrorCode = 3;
            NetMode.Status &= ~NET_ERROR;
        }
        break;
    case ConnectAP:     // ����AP
        if(NetMode.Status & NET_READY)  // ��λ��ģ��׼��OK
        {
            NetModeConnectAP();         // ��������ģ������
            NetStatus = ConnectServer;  // �������ӷ�����
            NetMode.ErrorCode = 0;
            NetMode.Status &= ~NET_READY;
        }
        else if(NetMode.Status & NET_ERROR) 
        {
            NetMode.ErrorCode = 1;
            NetMode.Status &= ~NET_ERROR;
        }
        break;
    case ConnectServer:         // ���ӷ�����
        if(NetMode.Status & NET_OK)     // ����AP OK
        {
            NetModeConnectServer();     // �������ӷ���������
            NetStatus = ServerJoin;     // �������ӷ�����
            NetMode.ErrorCode = 0;
            
        }
        else if(NetMode.Status & NET_ALREADY_CONNECTED) NetStatus = ServerOK;
        else if(NetMode.Status & NET_FAIL) 
        {
            NetMode.ErrorCode = 4;
            NetMode.Status &= ~NET_FAIL;
        }
        break;
    case ServerJoin:
        if(NetMode.Status & NET_CONNECT) 
        {
            NetStatus = ServerOK;
            NetMode.ErrorCode = 0;
            ReConnectCount = 0;
        }
        else if(NetMode.Status & NET_CLOSED) 
        {
            ReConnectCount++;
            if(ReConnectCount >= 20)
            {
                NetMode.ErrorCode = 5;
                NetStatus = ServerJoin;
            }
            else 
            {
                NetModeConnectServer();
                NetMode.Status &= ~NET_CLOSED;
            }
        }
        else NetMode.ErrorCode = 5;
        break;
    case ServerOK:
        if(NetMode.Status & NET_ACTION_SEND)
        {
            NetSendData((uint8_t *)&NetMode.SendData, 10);
            NetMode.Status &= ~NET_ACTION_SEND;
        }
        else if(NetMode.Status & NET_CLOSED) 
        {
            ReConnectCount++;
            if(ReConnectCount >= 20)
            {
                NetMode.ErrorCode = 5;
                NetStatus = ServerJoin;
            }
            else 
            {
                NetModeConnectServer();
                NetMode.Status &= ~NET_CLOSED;
            }
        }
        else if(NetMode.Status & (NET_CLOSED | NET_ERROR | NET_CONNECT_FAILED)) 
        {
            NetStatus = ServerJoin;
            NetMode.ErrorCode = 5;
        }
        break;
    }
    NetMode.Status &= ~NET_OK;
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