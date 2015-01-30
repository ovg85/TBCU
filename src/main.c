#include <avr/io.h>
#include <util/delay.h>
#include <display.h>


#define HI 0
#define LO 1
#define CUR 2
#define LIT_COUNT 2
#define DIG_COUNT 4
#define PLACE_COUNT 6

void Init (void);
unsigned int GetDataToDisplay(void);

char currentDisplay[]={0,0,0,0,0,0};
char currentDislpayIndex=0;

int main()
{
	unsigned int data=0;
	Init();
	currentDisplay[0]=SYMBOL_H;
	currentDisplay[1]=SYMBOL_I;
	currentDisplay[2]=SYMBOL_DASH;
	currentDisplay[3]=digits[1];
	currentDisplay[4]=digits[2]&SYMBOL_DP;
	currentDisplay[5]=digits[3];
	
	
	while(1)
	{
		for(int i=0;i<PLACE_COUNT;i++)
		{
			_delay_ms(30);
			data=GetDataToDisplay();
			SendData(data);	
			currentDislpayIndex=currentDislpayIndex + 1;
			if(currentDislpayIndex==PLACE_COUNT)
			{
				currentDislpayIndex=0;
			}
		}
	}
	return 1;
}

void Init (void)
{
	DDRD=0x07;	//PD0-PD2 как выход
	PORTD=0x00;	//Первоначально выключаем выход
}

unsigned int GetDataToDisplay(void)
{
	unsigned int data=0;
	data=(1<<(currentDislpayIndex+8));
	data=data|currentDisplay[currentDislpayIndex];
	return data;
}