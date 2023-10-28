/*
*********************************************************************************************************
*                                               uC/OS-II
*                                               实时内核
*
*                        (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*                                               版权所有
*
*                                            MCU-51 专用代码
*                                           KEIL C51大模式编译
*
* 文件名 : SERIAL.H
* 作者   : Jean J. Labrosse
* 改编   : 杨屹 gdtyy@ri.gdt.com.cn 巨龙公司系统集成开发部 2002.09.27
*********************************************************************************************************
*/

//**********************************************************************************
//杨屹    2002/08/20    第一版
//基于中断的串口驱动及显示程序
//联系方法：gdtyy@ri.gdt.com.cn（2003/07/31以前有效）
//**********************************************************************************
//使用方法：（此范例自包含，独立于其他程序。）
//        1.开头加入#include <reg51.h>语句，一定要有。
//        2.初始化串口        InitSerial();//本例中为20MHz晶体，300波特率，模式2初始化
//        3.初始化串口缓冲区  InitSerialBuffer();
//        4.使用显示字节，字，长字，字符，字符串，清屏函数。
//**********************************************************************************
//波特率计算公式：
//        TH1=256-(2^SMOD/32*Fosc/12*1/Bound)
//    其中：SMOD=0，1；Fosc=晶体或晶振频率；Bound=波特率
//    本例中，SMOD=0；Fosc=20*10E6；Bound=300，所以TH1=0x52。
//**********************************************************************************
//书写风格：
//        1.带yy前缀标志的函数为杨屹改写的等效C库函数。
//        2.单个单词用小写，yy定义为前缀，不算一个单词。
//        3.多个单词（2个及以上），每个单词首字母大写。(有时变量名第一个单词首字母小写)
//        4.采用内缩风格，每次缩进8个空格。
//**********************************************************************************

void InitSerial() reentrant;//串口初始化
void InitSerialBuffer(void) reentrant;//串口缓冲区初始化
bit  yygetch(unsigned char *ch) reentrant;//从串口缓冲区读1字节数据
void PrintChar(unsigned char ch) reentrant;//显示字符
void PrintCh(unsigned char ch) reentrant;//内部使用，不建议用户使用。
void PrintByte(unsigned char Byte) reentrant;//以十六进制格式显示1个字节数据
void PrintWord(unsigned int Word) reentrant;//以十六进制格式显示1个字数据
void PrintLong(unsigned long LongWord) reentrant;//以十六进制格式显示1个长字数据
void PrintStr(unsigned char *str) reentrant;//显示字符串
void clrscr() reentrant;//清屏

void serial(void) reentrant;//串口中断服务子程序

//收发缓冲区长度
#define LenTxBuf        2000    //Don't too small,because it will cause display abnormal.
#define LenRxBuf        50

#define MaxLenStr       100     /*buf[MaxLenStr+1] for '\0'*/

#define	TABNum          4       //TAB键移动长度

