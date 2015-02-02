#define SEG_A 0
#define SEG_B 1
#define SEG_C 2
#define SEG_D 3
#define SEG_E 4
#define SEG_F 5
#define SEG_G 6
#define SEG_DP 7

//For common anode!!
#define SYMBOL_H 0x89
#define SYMBOL_I 0xCF
#define SYMBOL_L 0xC7
#define SYMBOL_O 0xC0
#define SYMBOL_C 0xC6
#define SYMBOL_U 0xC1
#define SYMBOL_DASH 0xBF
#define SYMBOL_DP 0x7F

#define DATA 0
#define SCL 1
#define REC 2
#define PORT_DISP PORTD
#define DDR_DISP DDRD




#define SetPortBit(port, bit) port |= (1<<bit)

#define ClearPortBit(port, bit) port &= ~(1<<bit)

//For common anode!!
static char digits[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xf8,0x80,0x90};

void SendData(unsigned int data);
