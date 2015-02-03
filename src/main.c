#include <avr/io.h>
#include <util/delay.h>
#include <display.h>
#include <avr/interrupt.h>

#define HI 0
#define LO 1
#define CUR 2
#define LIT_COUNT 2
#define DIG_COUNT 4
#define PLACE_COUNT 6

#define SWITCH_DELAY 5000

void Init (void);
unsigned int GetDataToDisplay(void);
void ShowHi(void);
void ShowLow(void);
void ShowCurrent(void);

char currentDisplay[]={0,0,0,0,0,0};
char currentDislpayIndex=0;

int main(void)
{
	Init();
	
	ShowHi();
	
	sei();
	
	while(1)
	{
		ShowHi();
		_delay_ms(SWITCH_DELAY);
		ShowLow();
		_delay_ms(SWITCH_DELAY);
		ShowCurrent();
		_delay_ms(SWITCH_DELAY);
	}
	return 1;
}


ISR(TIMER2_COMP_vect)
{
	unsigned int data=GetDataToDisplay();
	SendData(data);	
	currentDislpayIndex=currentDislpayIndex + 1;
	if(currentDislpayIndex==PLACE_COUNT)
	{
		currentDislpayIndex=0;
	}
}

void Init (void)
{
	DDR_DISP=0x07;	//PD0-PD2 как выход
	PORT_DISP=0x00;	//Первоначально выключаем выход
	
	TCCR2=(1<<CS22)|(1<<CS21)|(1<<CS20);	//Prescaler 1024
	TCNT2=0x00;								//initial counter = 0
	TIMSK=(1<<OCIE2); 						// Enable CTC interrupt
	OCR2 = 10; 								// interrupt at counter = 1
	
}

unsigned int GetDataToDisplay(void)
{
	unsigned int data=0;
	data=(1<<(currentDislpayIndex+8));
	data=data|currentDisplay[currentDislpayIndex];
	return data;
}

void ShowHi(void)
{
	currentDisplay[0]=SYMBOL_H;
	currentDisplay[1]=SYMBOL_I;
	currentDisplay[2]=SYMBOL_EMPTY;
	currentDisplay[3]=digits[1];
	currentDisplay[4]=digits[5]&SYMBOL_DP;
	currentDisplay[5]=digits[4];
}

void ShowLow(void)
{
	currentDisplay[0]=SYMBOL_L;
	currentDisplay[1]=SYMBOL_O;
	currentDisplay[2]=SYMBOL_DASH;
	currentDisplay[3]=SYMBOL_EMPTY;
	currentDisplay[4]=digits[2]&SYMBOL_DP;
	currentDisplay[5]=digits[7];
}

void ShowCurrent(void)
{
	currentDisplay[0]=SYMBOL_C;
	currentDisplay[1]=SYMBOL_U;
	currentDisplay[2]=SYMBOL_DASH;
	currentDisplay[3]=SYMBOL_EMPTY;
	currentDisplay[4]=digits[1]&SYMBOL_DP;
	currentDisplay[5]=digits[2];
}