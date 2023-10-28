/*
*********************************************************************************************************
*                               基于uC/OS-II的串口驱动及应用程序接口函数
*                                             版权所有
*
* 文件名 : COMM_51.H
* 作者   : 陈是知 csz-mail@126.com 北京中软 2005。3。11
*********************************************************************************************************
*/
void     CommISR(void);

INT8U    CommCfgPort(INT8U ch, INT16U baud, INT8U bits, INT8U parity, INT8U stops);
void     CommISRHandler() reentrant;
void     CommRxFlush(INT8U ch);
void     CommRxIntDis(INT8U ch);
void     CommRxIntEn(INT8U ch);
void     CommTxIntDis(INT8U ch);
void     CommTxIntEn(INT8U ch);
void     CommRclIntVect(INT8U ch);
void     CommSetIntVect(INT8U ch);
void     CommInit(void) reentrant;
