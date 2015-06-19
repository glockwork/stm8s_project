#include "kmp.h"



//函数功能：获取Next数组
//参数 T：子字符串指针
//参数 *next：Next数组指针
void get_next(String T, int *next)
{
    int j = 0;  //前缀
    int i = 1;  //后缀

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


//函数功能：返回子字符串在主字符串第pos个字符之后的位置，若不存在，则返回0
//参数 S：主字符串指针
//参数 T：子字符串指针
//参数 pos：主字符串的当前下标
int Index_KMP(String S, String T, int pos)
{
    int i = pos;    //主字符串当前下标
    int j = 1;      //子字符串当前下标

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
