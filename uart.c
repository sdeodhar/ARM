/*
	Author:- Shreya Deodhar
	LPC2148 Basic UART Test to start debugging a board 
*/

#include <lpc214x.h>

#define PLOCK 0x00000400 
#define THRE (1<<5) // Transmit Holding Register Empty
#define MULVAL 15
#define DIVADDVAL 1
#define NEW_LINE 0xA // Character for new line .. analogous to '\n'

void initUART0(void);
void U0Write(char data);
void initClocks(void);

int main(void)
{
	char msg[] = { 'H','e','l','l','o',' ','t','h','i','s',' ','a','','t','e','s','t','\0' }; 
	int c=0; // count
	initClocks(); // Set CCLK=60Mhz and PCLK=60Mhz 
	initUART0();
	
	for(;;)		
	{
		while( msg[c]!='\0' )
		{
			U0Write(msg[c]);
			c++;
		}
		U0Write(NEW_LINE); 
		c=0; // reset count		
	}
	return 0;
	
}

void initUART0(void)
{
	PINSEL0 = 0x5;       /* Select TxD for P0.0 and RxD for P0.1 */
	U0LCR = 3 | (1<<7) ; /* 8 bits, no Parity, 1 Stop bit | DLAB set to 1  */
	U0DLL = 110;
	U0DLM = 1;   
	U0FDR = (MULVAL<<4) | DIVADDVAL; /* MULVAL=15(bits - 7:4) , DIVADDVAL=0(bits - 3:0)  */
	U0LCR &= 0x0F;                  // Set DLAB=0 to lock MULVAL and DIVADDVAL
	
	//BaudRate is now ~9600 and we are ready for UART communication!
}

void U0Write(char data)
{
	while ( !(U0LSR & THRE ) ){}; // wait till the THR is empty
	                              // now we can write to the Tx FIFO
	U0THR = data;
}

