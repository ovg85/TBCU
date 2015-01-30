#include <display.h>
#include <avr/io.h>
#include <util/delay.h>

void SendData(unsigned int data)
{
	for (int i=0;i<16;i++)
	{
		if ((data&0x8000)==0x00)
			ClearPortBit(PORT_DISP, DATA);
		else
			SetPortBit(PORT_DISP, DATA);
		
		asm("nop");
		SetPortBit(PORT_DISP, SCL);//Импульс на SCL
		asm("nop");
		ClearPortBit(PORT_DISP, SCL);
		
		asm("nop");
		data=(data<<1);
	}
	asm("nop");
	SetPortBit(PORT_DISP, REC);//Импульс на Latch clock
	asm("nop");
	ClearPortBit(PORT_DISP, REC);
}