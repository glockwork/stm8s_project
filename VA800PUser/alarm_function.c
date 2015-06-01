#include "alarm_function.h"

/*
Alarm 格式: 
	0: 星期
	1: 时
	2: 分
	3: 开关
	4: 动作 1.开机 2.关机

星期:
8 4 2 1 8 4 2 1
  7 6 5 4 3 2 1  =星期
  
开关:
1: 开 2: 关

Alarm_Switch 含义:
H				L
8 4 2 1 8 4 2 1 8 4 2 1 8 4 2 1 
F E D C B A 9 8 7 6 5 4 3 2 1 0 =>闹钟开关序列编号
*/
#define WEEK_MASK	0x7F
//unsigned char Alarm[10][5] = {0};

// AlarmWork:闹钟功能执行状态, 每一位对应一组闹钟。
unsigned int AlarmWork = 0x0001;

/*
名称: void GetTime(unsigned char *TimeBuffer)
功能: 获取当前时间
形参: *TimeBuffer 时间缓冲区地址
返回: 无
*/
void GetTime(unsigned char *TimeBuffer)
{
    uint8_t temp[7];

    Get_RT8563(temp, 2, 7);
    TimeBuffer[0] = BCD2Dec(temp[4]);
    TimeBuffer[1] = BCD2Dec(temp[2]);
    TimeBuffer[2] = BCD2Dec(temp[1]);
}

/*
名称: unsigned char ArrayCMP(unsigned char *Array1, unsigned char *Array2, unsigned char Length)
功能: 时间数组比较
形参: *Array1 数组1, *Array2 数组2, Length 数组长度
返回: 相同返回0, 不相同返回1
*/
unsigned char ArrayCMP(unsigned char *Array1, unsigned char *Array2, unsigned char Length)
{
    unsigned char i;
    for(i = 0; i < Length; i++)
    {
        if(Array1[i] != Array2[i]) return 1;
    }
    return 0;
}

/*
名称: void ArrayCopy(unsigned char *Array1, unsigned char *Array2, unsigned char Length)
功能: Array2复制到Array1
形参: *Array1 数组1, *Array2 数组2 Length 复制的长度
返回: 无
*/
void ArrayCopy(unsigned char *Array1, unsigned char *Array2, unsigned char Length)
{
    unsigned int i;
    for(i = 0; i < Length; i++) Array1[i] = Array2[i];
}

/*
名称: unsigned int TimerCMP(unsigned char *Time1, unsigned char *Time2, unsigned char Number)
功能: 时间数组比较
形参: *Time1 时间数组1, Time2 时间数组2, Number 时间数值1的个数
返回: 比较结果，每一位对应一组时间的对比结果
*/
unsigned int TimerCMP(unsigned char (*Time1)[3], unsigned char *Time2, unsigned char Number)
{
    unsigned char i, result = 0;

    for(i = 0; i < Number; i++)
    {
        if(ArrayCMP(Time1[i], Time2, 3) == 0) return 1;
//            result |= (1 << i);
    }
    return result;
}

/*
名称: unsigned char CheckAlarmTime(void)
功能: 检查当前时间是否为闹钟时间
形参: 无
返回: 当前闹钟的序列值, 无则返回0
*/
unsigned int CheckAlarmTime(unsigned char (*Alarm)[3])
{
    static unsigned char LastTime[3] = {0}, returnValue = 0;
    unsigned char NowTime[3], AlarmIndex = 0;
    static uint8_t AlarmState = 0;
    static uint8_t AlarmDelay = 1;
	
    GetTime(NowTime);
    
    if(ArrayCMP(LastTime, NowTime, 3))    //时间有变化
    {
//        ArrayCopy(LastTime, NowTime, 3);
//		AlarmState = TimerCMP(Alarm, NowTime, 5);// & Alarm_Switch; //判断闹钟开关
//        return 1
        ArrayCopy(LastTime, NowTime, 3);
        AlarmIndex = TimerCMP(Alarm, NowTime, 5) ;
        if(AlarmIndex != 0 && AlarmState == 0) 
        {
            returnValue = 1;
            Alarm[AlarmIndex][2] += AlarmDelay;
            AlarmState = 1;
        }
        else if(AlarmIndex != 0 && AlarmState == 1)
        {
            AlarmState = 0;
            Alarm[AlarmIndex][2] -= AlarmDelay;
        }
    }
    else returnValue = 0;
    return returnValue;
}