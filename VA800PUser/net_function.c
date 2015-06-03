#include "net_function.h"


NET_RECV    NetMode = {0};
uint8_t RxRecvBuffer[BUFFERSIZE] = {0};
uint16_t DataSize = 0;
enum {ModeReady, TestMode, SetMode, ConnectAP, ConnectServer, ServerJoin, ServerOK} NetStatus;

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

/*
名称: void TestNetMode(void)
功能: 发送测试命令
形参: 无
返回值：无
*/ 
void TestNetMode(void)
{
    UART1_SendString("AT\r\n", sizeof("AT\r\n")-1);
}

/*
名称: void SetNetMode(uint8_t Mode)
功能: 发送设置模式命令
形参: Mode 模式
返回值：无
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
名称: void ResetNetMode(void)
功能: 发送复位命令
形参: 无
返回值：无
*/ 
void ResetNetMode(void)
{
    UART1_SendString("AT+RST\r\n", sizeof("AT+RST\r\n")-1);
}

/*
名称: void ClosedConnectet(void)
功能: 发送关闭连接命令
形参: 无
返回值：无
*/ 
void ClosedConnectet(void)
{
    UART1_SendString("AT+CIPCLOSE\r\n", sizeof("AT+CIPCLOSE\r\n")-1);
}

/*
名称: void ClosedAP(void)
功能: 发送关闭连接命令
形参: 无
返回值：无
*/ 
void ClosedAP(void)
{
    UART1_SendString("AT+CWQAP\r\n", sizeof("AT+CWQAP\r\n")-1);
}

/*
名称: void NetModeConnectAP(void)
功能: 发送连接AP命令
形参: 无
返回值：无
*/ 
void NetModeConnectAP(void)
{
    UART1_SendString(AP_Message, sizeof(AP_Message)-1);
}

/*
名称: void NetModeConnectServer(void)
功能: 发送连接服务器命令
形参: 无
返回值：无
*/ 
void NetModeConnectServer(void)
{
    UART1_SendString(Server_Message, sizeof(Server_Message)-1);
}


/*
名称: void NetSendDataLength(void)
功能: 发送数据长度
形参: 无
返回值：无
*/ 
void NetSendDataLength(void)
{
    UART1_SendString("AT+CIPSEND=10\r\n", sizeof("AT+CIPSEND=10\r\n")-1);
}

/*
名称: void NetSendData(uint8_t *Data, uint16_t Length)
功能: 发送数据
形参: 无
返回值：无
*/ 
void NetSendData(uint8_t *Data, uint16_t Length)
{
    UART1_SendString(Data, Length);
}

/*
名称: void isRecvOK(uint8_t *RxRecvData)
功能: 判断是否接收的数据中是否含有"OK"
形参: *RxRecvData 数据指针
返回值：无
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
名称: void isRecvReady(uint8_t *RxRecvData)
功能: 判断是否接收的数据中是否含有"ready"
形参: *RxRecvData 数据指针
返回值：无
*/ 
void isRecvReady(uint8_t *RxRecvData)
{
    uint8_t READY[] = " ready";
    READY[0] = 5;

    if(Index_KMP(RxRecvData, READY, 1)) NetMode.Status |= NET_READY;
    //UART1_SendString("ready\n");
}

/*
名称: void isRecvError(uint8_t *RxRecvData)
功能: 判断是否接收的数据中是否含有"ERROR"
形参: *RxRecvData 数据指针
返回值：无
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
名称: void isRecvData(uint8_t *RxRecvData)
功能: 判断是否接收的数据中是否含有"+IPD"
形参: *RxRecvData 数据指针
返回值：无
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
名称: void isRecvSendOK(uint8_t *RxRecvData)
功能: 判断是否接收的数据中是否含有"SEND OK"
形参: *RxRecvData 数据指针
返回值：无
*/ 
void isRecvSendOK(uint8_t *RxRecvData)
{
    uint8_t SEND_OK[] = " SEND OK";
    SEND_OK[0] = 7;

    if(Index_KMP(RxRecvData, SEND_OK, 1)) NetMode.Status |= NET_SEND_OK;
    //UART1_SendString("SEND OK\n");
}

/*
名称: void isRecvSendFAIL(uint8_t *RxRecvData)
功能: 判断是否接收的数据中是否含有"SEND FAIL"
形参: *RxRecvData 数据指针
返回值：无
*/ 
void isRecvSendFAIL(uint8_t *RxRecvData)
{
    uint8_t SEND_FAIL[] = " SEND FAIL";
    SEND_FAIL[0] = 9;

    if(Index_KMP(RxRecvData, SEND_FAIL, 1)) NetMode.Status |= NET_SEND_FAIL;
    //UART1_SendString("SEND OK\n");
}

/*
名称: void isRecvClosed(uint8_t *RxRecvData)
功能: 判断是否接收的数据中是否含有"CLOSED"
形参: *RxRecvData 数据指针
返回值：无
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
名称: void isRecvBusy(uint8_t *RxRecvData)
功能: 判断是否接收的数据中是否含有"busy"
形参: *RxRecvData 数据指针
返回值：无
*/ 
void isRecvBusy(uint8_t *RxRecvData)
{
    uint8_t BUSY[] = " busy";
    BUSY[0] = 4;

    if(Index_KMP(RxRecvData, BUSY, 1)) NetMode.Status |= NET_BUSY;
    //UART1_SendString("Busy\n");
}

/*
名称: void isRecvAL_Connected(uint8_t *RxRecvData)
功能: 判断是否接收的数据中是否含有"ALREADY CONNECTED"
形参: *RxRecvData 数据指针
返回值：无
*/ 
void isRecvAL_Connected(uint8_t *RxRecvData)
{
    uint8_t AL_CONNECT[] = " ALREADY CONNECTED";
    AL_CONNECT[0] = 17;

    if(Index_KMP(RxRecvData, AL_CONNECT, 1)) NetMode.Status |= NET_ALREADY_CONNECTED;
}

/*
名称: void isRecvFail(uint8_t *RxRecvData)
功能: 判断是否接收的数据中是否含有"FAIL"
形参: *RxRecvData 数据指针
返回值：无
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
名称: void isRecvConnect(uint8_t *RxRecvData)
功能: 判断是否接收的数据中是否含有"CONNECT"
形参: *RxRecvData 数据指针
返回值：无
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
名称: void isConnectFailed(uint8_t *RxRecvData)
功能: 判断是否接收的数据中是否含有"CONNECT Failed"
形参: *RxRecvData 数据指针
返回值：无
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
名称: void GetNetFlags(uint8_t *RxRecvData)
功能: 解析数据
形参: *RxRecvData 数据指针
返回值：无
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
名称: void DataResolve(uint8_t *Data, uint16_t Length)
功能: 数据分解
形参: *Data 数据指针, Length 数据长度
返回值：无
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

#define MAX_FIX         10              // 最大尝试次数
#define MAX_TIME        5*60            // 重复尝试间隔时间
void NetModeErrorFix(void)
{
    static uint16_t TimeCount = 0;      // 计时
    static uint16_t FixCount = 0;       // 修复计数
    static uint8_t ModeBreakDown = 0;   // 模块损坏计数
    static uint8_t ErrorCode = 0;
    
    if(ErrorCode != NetMode.ErrorCode)  // 如果错误代码发生变化，计数变量清零。
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

    case 80:            // 重复尝试
        TimeCount++;
        break;
    case 81:
        // 添加模块损坏提示代码
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
        if(ModeBreakDown == MAX_FIX) NetMode.ErrorCode = 81;         // 标识模块损坏
    }
} 

/*
名称: void NetProcess(void)
功能: 网络通信过程
形参: 无
返回值：无
*/ 
void NetProcess(void)
{
    static uint8_t ReConnectCount = 0;
    switch(NetStatus)
    {
    case ModeReady:     // 上电发送模块复位命令后
        if(NetMode.Status & NET_READY)  // 模块准备OK
        {
            TestNetMode();
            NetStatus = TestMode;       // 进入测试模块
            NetMode.ErrorCode = 0;
            NetMode.Status &= ~NET_READY;
        }
        else if(NetMode.Status & NET_ERROR) 
        {
            NetMode.ErrorCode = 1;
            NetMode.Status &= ~NET_ERROR;
        }
        break;
    case TestMode:      // 测试模块
        if(NetMode.Status & NET_OK)     // 测试模块OK
        {
            NetStatus = SetMode;        
            SetNetMode(3);              // 配置模块
            NetMode.ErrorCode = 0;
        }
        else if(NetMode.Status & NET_ERROR) 
        {
            NetMode.ErrorCode = 2;
            NetMode.Status &= ~NET_ERROR;
        }
        break;
    case SetMode:               // 配置模块
        if(NetMode.Status & NET_OK)     // 配置OK
        {
            ResetNetMode();     
            NetStatus = ConnectAP;      // 复位模块，是配置生效，进入连接AP
            NetMode.ErrorCode = 0;
        }
        else if(NetMode.Status & NET_ERROR) 
        {
            NetMode.ErrorCode = 3;
            NetMode.Status &= ~NET_ERROR;
        }
        break;
    case ConnectAP:     // 连接AP
        if(NetMode.Status & NET_READY)  // 复位后，模块准备OK
        {
            NetModeConnectAP();         // 发送连接模块命令
            NetStatus = ConnectServer;  // 进入连接服务器
            NetMode.ErrorCode = 0;
            NetMode.Status &= ~NET_READY;
        }
        else if(NetMode.Status & NET_ERROR) 
        {
            NetMode.ErrorCode = 1;
            NetMode.Status &= ~NET_ERROR;
        }
        break;
    case ConnectServer:         // 连接服务器
        if(NetMode.Status & NET_OK)     // 连接AP OK
        {
            NetModeConnectServer();     // 发送连接服务器命令
            NetStatus = ServerJoin;     // 进入连接服务器
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