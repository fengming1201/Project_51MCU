/*
*********************************************************************************************************
*                               基于uC/OS-II的串口驱动及应用程序接口函数
*                                             版权所有
*
* 文件名 : COMM_51.C
* 作者   : 陈是知 csz-mail@126.com 北京中软 2005。3。11
*********************************************************************************************************
*/

#include "includes.h"


void CommInit() reentrant
{

		TMOD=TMOD&0x0F;
        TMOD=TMOD|0x20;
        TL1=0xFD,TH1=0xFD;//9600 , 11.0592MHz
        SCON=0x50;PCON=0x00;
        TR1=1;
		ES=1;
}


void  CommISRHandler () reentrant
{  
    INT8U   c;
    INT8U   err;
	extern INT8U FirstCharFlag;
	
    if(RI){
            RI=0;
            c=SBUF;
            CommPutRxChar(c);
		  }

    if(TI){
			TI=0;
			if(FirstCharFlag==0x01)
	         {
			  c = CommGetTxChar(&err);   
    		  FirstCharFlag=0x00;
              if(err==COMM_NO_ERR)
			  SBUF=c;
			 }
			else
			 {
			  c = CommGetTxChar(&err);
 			  if(err==COMM_NO_ERR)
			    SBUF=c;
			  else
			    FirstCharFlag=0x01;
			 }
          }
			  
}
 
