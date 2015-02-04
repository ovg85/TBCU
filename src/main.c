#include <avr/io.h>
#include <util/delay.h>
#include <display.h>
#include <avr/interrupt.h>

#include "OWIPolled.h"
#include "OWIHighLevelFunctions.h"
#include "OWIBitFunctions.h"
#include "common_files\OWIcrc.h"

#define HI 0
#define LO 1
#define CUR 2
#define ITEMS_COUNT 3
#define LIT_COUNT 2
#define DIG_COUNT 4
#define PLACE_COUNT 6

#define TCNT1_INITIAL 61535;

#define FCPU 8000000

//команды датчика DS18B20
#define DS18B20_SKIP_ROM 0xcc
#define DS18B20_CONVERT_T 0x44
#define DS18B20_READ_SCRATCHPAD 0xbe
#define DS18B20_BUS OWI_PIN_7
//Init
void Init (void);
//Prepare data to send to display
unsigned int GetDataToDisplay(void);
//Copy Hi temperature to currentDisplay array
void ShowHi(void);
//Copy Low temperature to currentDisplay array
void ShowLow(void);
//Copy Current temperature to currentDisplay array
void ShowCurrent(void);
//Displaying one digit of currentDisplay array at display
void ShowCurrentDigit(void);
//Sets Hi, Low or Current temperature to currentDisplay array
void SetNextDisplayItem(void);

void ReadSensorData(void);  


//Current data to display
char currentDisplay[]={0,0,0,0,0,0};
// Current dislpayed digit (from 0 to 5)
char currentDislpayIndex=0;
// HI, Low or Current temperature
char currentDislpayItemIndex=0;
//Store ds18b20 data
unsigned char scratchpad[9];

int main(void)
{
	Init();
	
	ShowHi();
	
	sei();
	
	while(1)
	{
		ReadSensorData();  

	}
	return 1;
}

ISR(TIMER0_OVF_vect)
{
	ShowCurrentDigit();
}

ISR( TIMER1_OVF_vect )
{
	TCNT1 = TCNT1_INITIAL;  //выставляем начальное значение TCNT1
	SetNextDisplayItem();
}


void Init (void)
{
	DDR_DISP=0x07;	//PD0-PD2 как выход
	PORT_DISP=0x00;	//Первоначально выключаем выход
	
	TCCR0=(1<<CS01);		//Prescaler 8
	TCNT0=0x00;				//initial counter = 0
	TIMSK=(1<<TOIE0); 		//Timer/Counter0 Overflow Interrupt Enable
	TIFR=(1<<TOV0);		//Timer/Counter0 Overflow Flag
	
	//1000000/1024/4000 ~ 0.244 sec
	TCCR1B = (1<<CS12)|(1<<CS10);	//Prescaler 1024
	TIMSK |= (1<<TOIE1); 			//Timer/Counter1 Overflow Interrupt Enable
	TCNT1 = TCNT1_INITIAL;        	// start counter value 
  
	OWI_Init(DS18B20_BUS);
  
	//for TIMER2_COMP_vect
	//TCCR2=(1<<CS22)|(1<<CS21);	//Prescaler 256
	//TCNT2=0x00;					//initial counter = 0
	//TIMSK=(1<<OCIE2); 			//  Timer/Counter2 Output Compare Match Interrupt Enable
	//OCR2 = 20; 					// interrupt at counter = 20
	
}

void ReadSensorData(void)
{
	OWI_DetectPresence(DS18B20_BUS);
	OWI_SkipRom(DS18B20_BUS);
	OWI_SendByte(DS18B20_CONVERT_T ,DS18B20_BUS);
		
	_delay_ms(FCPU);
	
	OWI_DetectPresence(DS18B20_BUS);
	OWI_SkipRom(DS18B20_BUS);
	OWI_SendByte(DS18B20_READ_SCRATCHPAD, DS18B20_BUS);
		
	scratchpad[0] = OWI_ReceiveByte(DS18B20_BUS);
	scratchpad[1] = OWI_ReceiveByte(DS18B20_BUS);
}

void ShowCurrentDigit(void)
{
	unsigned int data=GetDataToDisplay();
	SendData(data);	
	currentDislpayIndex=currentDislpayIndex + 1;
	if(currentDislpayIndex==PLACE_COUNT)
	{
		currentDislpayIndex=0;
	}
}

void SetNextDisplayItem(void)
{
	currentDislpayItemIndex=currentDislpayItemIndex+1;
	if(currentDislpayItemIndex==ITEMS_COUNT)
	{
		currentDislpayItemIndex=0;
	}
	
	  switch (currentDislpayItemIndex)
	  {
	    case (HI):
	      ShowHi();
	      break;
	
	    case (LO):
	      ShowLow();
	      break;
	
	    case (CUR):
	      ShowCurrent();
	      break;
	
	    default:
	      ShowCurrent();
	      break;
	  }
	
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
	currentDisplay[2]=SYMBOL_EMPTY;
	currentDisplay[3]=SYMBOL_DASH;
	currentDisplay[4]=digits[2]&SYMBOL_DP;
	currentDisplay[5]=digits[7];
}

void ShowCurrent(void)
{
	currentDisplay[0]=SYMBOL_C;
	currentDisplay[1]=SYMBOL_U;
	currentDisplay[2]=SYMBOL_EMPTY;
	currentDisplay[3]=SYMBOL_DASH;
	currentDisplay[4]=digits[1]&SYMBOL_DP;
	currentDisplay[5]=digits[2];
}