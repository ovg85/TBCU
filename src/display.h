#define SYMBOL_H 0x40
#define SYMBOL_I 0x40
#define SYMBOL_L 0x40
#define SYMBOL_O 0x40
#define SYMBOL_DASH 0x40

#define DATA 0
#define SCL 1
#define REC 2
#define PORT_DISP PORTD

//decimal point bit number
#define DP_BIT 9

#define SetPortBit(port, bit) port |= (1<<bit)

#define ClearPortBit(port, bit) port &= ~(1<<bit)

void SendData(unsigned int data);
