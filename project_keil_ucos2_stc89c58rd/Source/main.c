/* 本例程uCOS-II 版本为2.51*/  

#include "includes.h"


sbit LED1=P3^0;

sbit LED2=P3^1;  


OS_STK Task1Stk[MaxStkSize+1];

OS_STK Task2Stk[MaxStkSize+1];


INT8U err;

void Task1(void *ppdata) reentrant
{

	ppdata=ppdata;

	for(;;)
	{

		LED1=~LED1;
		OSTimeDly(OS_TICKS_PER_SEC/4);
	}    
}



void Task2(void *ppdata) reentrant
{

	ppdata=ppdata;
	for(;;)
	{

		LED2=~LED2;   
		OSTimeDly(OS_TICKS_PER_SEC);	
	}    
}

void main(void)
{
	OSInit();
	InitHardware();

	OSTaskCreate(Task1,(void*)0,&Task1Stk[0],0);

	OSTaskCreate(Task2,(void*)0,&Task2Stk[0],1);


	OSStart();

}
