/*
*********************************************************************************************************
*   											uC/OS-II
*   											ʵʱ�ں�
*
*   					 (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*   											��Ȩ����
*                                       STC89C58RD+ Specific code
*   										
*
* �ļ��� : main.c  Ӧ���ļ�
* Modify	   : StrongZhai
* Date  	   : 27-Aug-11
*********************************************************************************************************
*/
#include"includes.h"
sbit LED0 = P3 ^ 0;
sbit LED1 = P3 ^ 1;

OS_STK Task1Stk[MaxStkSize]; //���������ջ
OS_STK Task2Stk[MaxStkSize];
OS_STK Task3Stk[MaxStkSize];

void Task1(void* ddata) reentrant
{
	ddata = ddata;
	while (1)
	{
		LED0 = ~LED0;
		OSTimeDly(50);   //��11.0592M�����£���ҪԼ33.4MS�л�һ������OSTimeDly(1);��С�л�����ʱ��
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