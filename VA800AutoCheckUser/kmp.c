#include "kmp.h"



//�������ܣ���ȡNext����
//���� T�����ַ���ָ��
//���� *next��Next����ָ��
void get_next(String T, int *next)
{
    int j = 0;  //ǰ׺
    int i = 1;  //��׺

    next[1] = 0;

    while (i < T[0])
    {
        if (0 == j || T[i] == T[j])
        {
            i++;
            j++;
            if (T[i] != T[j])
            {
                next[i] = j;
            }
            else
            {
                next[i] = next[j];
            }

        }
        else
        {
            j = next[j];
        }
    }
}


//�������ܣ��������ַ��������ַ�����pos���ַ�֮���λ�ã��������ڣ��򷵻�0
//���� S�����ַ���ָ��
//���� T�����ַ���ָ��
//���� pos�����ַ����ĵ�ǰ�±�
int Index_KMP(String S, String T, int pos)
{
    int i = pos;    //���ַ�����ǰ�±�
    int j = 1;      //���ַ�����ǰ�±�

    int next[255];

    get_next(T, next);

    while (i <= S[0] && j <= T[0])
    {
        if (0 == j || S[i] == T[j])
        {
            i++;
            j++;
        }
        else
        {
            j = next[j];
        }
    }

    if (j > T[0])
    {
        return i - T[0];
    }
    else
    {
        return 0;
    }
}
