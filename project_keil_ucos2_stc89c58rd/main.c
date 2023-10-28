/*
*********************************************************************************************************
*   											uC/OS-II
*   											实时内核
*
*   					 (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*   											版权所有
*                                       STC89C58RD+ Specific code
*   										
*
* 文件名 : main.c  应用文件
* Modify	   : StrongZhai
* Date  	   : 27-Aug-11
*********************************************************************************************************
*/
#include"includes.h"
sbit LED0 = P3 ^ 0;
sbit LED1 = P3 ^ 1;

OS_STK Task1Stk[MaxStkSize]; //创建任务堆栈
OS_STK Task2Stk[MaxStkSize];
OS_STK Task3Stk[MaxStkSize];

void Task1(void* ddata) reentrant
{
	ddata = ddata;
	while (1)
	{
		LED0 = ~LED0;
		OSTimeDly(50);   //在11.0592M晶振下，需要约33.4MS切换一次任务，OSTimeDly(1);最小切换任务时间
	}
}
void Task2(void* ddata) reentrant
{
	ddata = ddata;
	while (1)
	{
	   
		LED1 = ~LED1;
		OSTimeDly(50); 
		  
	}
}

void main(void)
{
	P2 = 0xff;
	OSInit();
	InitTimer0();
	//init_UART();
	InitSerial();
	OSTaskCreate(Task1, (void *) 0, &Task1Stk[0], 1);
	OSTaskCreate(Task2, (void *) 0, &Task2Stk[0], 2);
	OSStart();
}