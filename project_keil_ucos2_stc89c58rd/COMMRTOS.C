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
* 文件名 : SERIAL.C
* 作者   : Jean J. Labrosse
* 改编   : 杨屹 gdtyy@ri.gdt.com.cn 巨龙公司系统集成开发部 2002.09.27
*********************************************************************************************************
*/

//**********************************************************************************
//杨屹    2002/08/20    第一版
//基于中断的串口驱动及显示程序
//联系方法：gdtyy@ri.gdt.com.cn（2003/07/31以前有效）
//**********************************************************************************
//程序特点：
//        1.基于中断，可并发执行
//        2.参数可配置（收发缓冲区大小，最大字符串长度，TAB键移动距离）
//**********************************************************************************
//使用方法：（此范例自包含，独立于其他程序。）
//        先配制收发缓冲区大小等可变参数（在serial.h中的宏定义）
//        1.开头加入#include <reg51.h>语句，一定要有。
//        2.初始化串口        InitSerial();//本例中为20MHz晶体，300波特率，模式2初始化
//        3.初始化串口缓冲区  InitSerialBuffer();
//        4.使用显示字节，字，长字，字符，字符串，清屏函数。
//自包含
//**********************************************************************************
//波特率计算公式：
//        TH1=256-(2^SMOD/32*Fosc/12*1/Bound)
//    其中：SMOD=0，1；Fosc=晶体或晶振频率；Bound=波特率
//    本例中，SMOD=0；Fosc=20*10E6；Bound=300，所以TH1=0x52。
//Baud rate(20Mhz)
//300(52);1200(D5);2400(EA);4800(F5);9600(FB);19200(FD);38400(FF);
//**********************************************************************************
//书写风格：
//        1.带yy前缀标志的函数为杨屹改写的等效C库函数。
//        2.单个单词用小写，yy定义为前缀，不算一个单词。
//        3.多个单词（2个及以上），每个单词首字母大写。(有时变量名第一个单词首字母小写)
//        4.采用内缩风格，每次缩进8个空格。
//**********************************************************************************
//应用举例：（可在KEIL仿真环境下运行）
//源程序文件：serial.h/serial.c/main.c
//main.c内容：
//#include <reg51.h>//Note:It must be added.必须在serial.c之前
//#include <serial.h>
//main()
//{
//	unsigned char ch;
//
//	InitSerial();
//	InitSerialBuffer();
//	while(1){
//		PrintStr("\n");
//		PrintByte(90);PrintStr(" ");
//		PrintWord(90);PrintStr(" ");
//		PrintLong(90);PrintStr(" ");
//		PrintChar('y');PrintChar(' ');//千万别写成双引号，否则打印0（乱字符）。
//		PrintStr("\nHello!\nI'm YangYi.\n");
//		PrintStr("Press any key to continue...");
//		while(!yygetch(&ch));
//	}
//}
//**********************************************************************************
//建议：
//    你完全可以把该子程序当作函数库使用，以便减少重复劳动，提高代码质量。
//**********************************************************************************

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

unsigned char TxBuf[LenTxBuf],RxBuf[LenRxBuf];//收发缓冲区实体
unsigned char *inTxBuf,*outTxBuf,*inRxBuf,*outRxBuf;//收发缓冲区读写指针
bit TIflag=1;//Note:It must be 1.

void InitSerial() reentrant//串口初始化
{
        TMOD=TMOD&0x0F;
        TMOD=TMOD|0x20;
        TL1=0xF3,TH1=0xF3;//19200 , 22.1184MHz
        SCON=0x50;PCON=0x00;
        TR1=1;
}

void InitSerialBuffer(void) reentrant//串口缓冲区初始化
{
        inTxBuf=TxBuf;outTxBuf=TxBuf;
        inRxBuf=RxBuf;outRxBuf=RxBuf;
        ES=1;
        //EA=1;
}

void serial(void) reentrant
{    //中断在汇编中实现，去掉interrupt 4{//串口中断服务子程序
        unsigned char *t;

        if(TI){
                TI=0;
                if(inTxBuf==outTxBuf) {TIflag=1;return;}//TxBuf Empty
                SBUF=*outTxBuf; outTxBuf++;
                if(outTxBuf==TxBuf+LenTxBuf) outTxBuf=TxBuf;	
        }
        if(RI){
                RI=0;
                t=inRxBuf;t++;
                if(t==RxBuf+LenRxBuf) t=RxBuf;
                if(t==outRxBuf) return;                 //RxBuf Full
                *inRxBuf=SBUF;
                inRxBuf=t;
        }
}

bit yygetch(unsigned char *ch) reentrant//从串口缓冲区读1字节数据
{
        //ES=0;        
        if(inRxBuf==outRxBuf) {ES=1;return 0;}          //RxBuf Empty
        *ch=*outRxBuf;  outRxBuf++;
        if(outRxBuf==RxBuf+LenRxBuf) outRxBuf=RxBuf;
        //ES=1;        
        return 1;
}

void PrintChar(unsigned char ch) reentrant//显示字符
{
        unsigned char *t;

        //ES=0;        
        if(TIflag){		
                TIflag=0;
                TI=1;
        }
        t=inTxBuf;t++;
        if(t==TxBuf+LenTxBuf) t=TxBuf;
        if(t==outTxBuf) {/*ES=1;*/return;}                  //TxBuf Full
        *inTxBuf=ch;
        inTxBuf=t;
        //ES=1;        
}

void PrintCh(unsigned char ch) reentrant//内部使用，不建议用户看到。
{
        if(ch>=0&&ch<=9) ch=ch+'0';
        else ch=ch+'A'-10;
        PrintChar(ch);
}

void insidePrintByte(unsigned char Byte) reentrant//内部使用，以十六进制格式显示1个字节数据
{
        unsigned char c;
        c=Byte;
        c=c>>4;        
        PrintCh(c);
        c=Byte;c=c&0x0F;PrintCh(c);        
}

void PrintByte(unsigned char Byte) reentrant//以十六进制格式显示1个字节数据
{
        EA=0;        
        insidePrintByte(Byte);                
        EA=1;
}

void insidePrintWord(unsigned int Word) reentrant//内部使用，以十六进制格式显示1个字数据
{	        
        insidePrintByte(Word>>8);
        insidePrintByte(Word&0xFF);                        
}

void PrintWord(unsigned int Word) reentrant//以十六进制格式显示1个字数据
{	
        EA=0;
        insidePrintWord(Word);
        EA=1;
}

void PrintLong(unsigned long LongWord) reentrant//以十六进制格式显示1个长字数据
{
        EA=0;
        insidePrintWord(LongWord>>16);
        insidePrintWord(LongWord&0xFFFF);
        EA=1;
}

void PrintStr(unsigned char *str) reentrant//显示字符串
{
        int i;
        unsigned char j;
        unsigned char ch;
	
        EA=0;
        for(i=0;i<MaxLenStr;i++){
                ch=*(str+i);
                if(ch=='\0') break;
                else if(ch=='\n'){PrintChar(10);PrintChar(13);} 
                else if(ch=='\t'){
                        for(j=0;j<TABNum;j++)
                                PrintChar(' ');
                }
                else PrintChar(ch);
        }
        EA=1;
}

void clrscr() reentrant//清屏
{        
        PrintStr("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");//25个回车换行清屏幕。
}