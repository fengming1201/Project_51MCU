#include "main.h"

void led1_on()
{
	LED1 = 1;
	Delay_ms(100);
}

void led1_off()
{
	LED1 = 0;
	Delay_ms(100);
}

void led2_on()
{
	LED2 = 1;
	Delay_ms(100);
}

void led2_off()
{
	LED2 = 0;
	Delay_ms(100);
}

void main()
{
	while(1)
	{
		led1_on();
		led1_off();

		led1_on();
		led1_off();

		led1_on();
		led1_off();

		led2_on();
		led2_off();

		led2_on();
		led2_off();

		led2_on();
		led2_off();

		LED1 = 1;
		LED2 = 1;
		Delay_ms(100);
		LED1 = 0;
		LED2 = 0;
		Delay_ms(100);
	}
}

void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}

