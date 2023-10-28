#ifndef __MAIN_H__
#define __MAIN_H__
#ifdef __C51__
#warning "defined __C51__"
#include <reg52.h>
#include <intrins.h>

sbit LED1 = P3 ^ 0;
sbit LED2 = P3 ^ 1;
#elif defined(__SDCC)
#warning "defined __SDCC"
#include <8052.h>

#define LED1 P3_0
#define LED2 P3_1
#else
#error "Unsupported compiler"
#endif

void Delay_ms(unsigned int n);

#endif