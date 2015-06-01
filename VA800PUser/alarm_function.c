#include "alarm_function.h"

/*
Alarm ��ʽ: 
	0: ����
	1: ʱ
	2: ��
	3: ����
	4: ���� 1.���� 2.�ػ�

����:
8 4 2 1 8 4 2 1
  7 6 5 4 3 2 1  =����
  
����:
1: �� 2: ��

Alarm_Switch ����:
H				L
8 4 2 1 8 4 2 1 8 4 2 1 8 4 2 1 
F E D C B A 9 8 7 6 5 4 3 2 1 0 =>���ӿ������б��
*/
#define WEEK_MASK	0x7F
//unsigned char Alarm[10][5] = {0};

// AlarmWork:���ӹ���ִ��״̬, ÿһλ��Ӧһ�����ӡ�
unsigned int AlarmWork = 0x0001;

/*
����: void GetTime(unsigned char *TimeBuffer)
����: ��ȡ��ǰʱ��
�β�: *TimeBuffer ʱ�仺������ַ
����: ��
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
����: unsigned char ArrayCMP(unsigned char *Array1, unsigned char *Array2, unsigned char Length)
����: ʱ������Ƚ�
�β�: *Array1 ����1, *Array2 ����2, Length ���鳤��
����: ��ͬ����0, ����ͬ����1
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
����: void ArrayCopy(unsigned char *Array1, unsigned char *Array2, unsigned char Length)
����: Array2���Ƶ�Array1
�β�: *Array1 ����1, *Array2 ����2 Length ���Ƶĳ���
����: ��
*/
void ArrayCopy(unsigned char *Array1, unsigned char *Array2, unsigned char Length)
{
    unsigned int i;
    for(i = 0; i < Length; i++) Array1[i] = Array2[i];
}

/*
����: unsigned int TimerCMP(unsigned char *Time1, unsigned char *Time2, unsigned char Number)
����: ʱ������Ƚ�
�β�: *Time1 ʱ������1, Time2 ʱ������2, Number ʱ����ֵ1�ĸ���
����: �ȽϽ����ÿһλ��Ӧһ��ʱ��ĶԱȽ��
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
����: unsigned char CheckAlarmTime(void)
����: ��鵱ǰʱ���Ƿ�Ϊ����ʱ��
�β�: ��
����: ��ǰ���ӵ�����ֵ, ���򷵻�0
*/
unsigned int CheckAlarmTime(unsigned char (*Alarm)[3])
{
    static unsigned char LastTime[3] = {0}, returnValue = 0;
    unsigned char NowTime[3], AlarmIndex = 0;
    static uint8_t AlarmState = 0;
    static uint8_t AlarmDelay = 1;
	
    GetTime(NowTime);
    
    if(ArrayCMP(LastTime, NowTime, 3))    //ʱ���б仯
    {
//        ArrayCopy(LastTime, NowTime, 3);
//		AlarmState = TimerCMP(Alarm, NowTime, 5);// & Alarm_Switch; //�ж����ӿ���
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