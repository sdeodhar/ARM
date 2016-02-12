/*

DEVELOPED BY:- Shreya Deodhar
WHAT PROGRAM DO:- Program Of Multiplex 8 Seven Segment Using
Max7221 And Controlled With ARM(LPC21XX) Using SPI Protocol

*/
 
#include<lpc21xx.h>
 
#define LCD (0xff<<16)
#define RS (1<<13)
#define RW (1<<14)
#define EN (1<<15)
 
void delay_fv(unsigned int x,int y);
void lcd_display(unsigned int x);
void cmd(unsigned char m);
void lcd_pos(int line,int pos);
void lcd_ini();
void lcd_str(unsigned char *x);
void init_rtc();

unsigned char flag=0; 
 
void main()
{
	unsigned int i,j,k,l,m,n;
	PINSEL0=0X00000000;
	PINSEL1=0X00000000;
	IO0DIR=0XFFFFFFFF;
	lcd_init();
	init_rtc();
	lcd_str("CLOCK:- 24");
	cmd(0xc0);
	lcd_str("00:00:00");
	while(1)
	{ 
	        if(flag)
	        {
			flag=0;
			lcd_pos(2,0);
			lcd_display(HOUR/10 + '0');
			lcd_display(HOUR%10 + '0'); 
			lcd_display(':') ;
			lcd_display(MIN/10 + '0');
			lcd_display(MIN%10 + '0');
			lcd_display(':') ;
			lcd_display(SEC/10 + '0');
		        lcd_display(SEC%10 + '0');
	        }
	}

}
 
 
void delay_fv(unsigned int x,int y)
{
	unsigned int i,j;
	for(i=0;i<x;i++){
		for(j=0;j<y;j++){};
	}
}
	
void lcd_display(unsigned int x)
{
	 IO0CLR|=(RS|RW|EN|LCD);
	 IO0SET|=(x<<16);
	 IO0SET|=RS;
	 IO0CLR|=RW;
	 IO0SET|=EN;
	 delay_fv(100,10);
	 IO0CLR|=EN;
	 delay_fv(10,10);
}
 
void cmd(unsigned char m)
{
	IO0CLR|=(RS|RW|EN|LCD);
	IO0SET|=(m<<16);
	IO0CLR|=RS;
	IO0CLR|=RW;
	IO0SET|=EN;
	delay_fv(100,250);
	IO0CLR|=EN;
	delay_fv(10,10);
}
 
void lcd_pos(int line,int pos)
{
    if(line==1)
      cmd(0x80+pos);
    else if(line==2)
      cmd(0xc0+pos);
}
 
void lcd_init()
{
	cmd(0X38);
	cmd(0X0e);
	cmd(0X06);
	cmd(0X01);
	cmd(0X80);
}
	
void lcd_str(unsigned char *x)
{
	while(*x!='\0')
	{
	     lcd_display(*x);
	     x++;
	}
}

void rtc_int(void)__irq
{
	ILR = 0X01;
	flag = 1;
	VICVectAddr = 0X00000000;
}
 
 
void init_rtc()
{
	ILR  = 0X01;
	CCR  = 0X13;
	CCR  = 0X11;
	CIIR = 0X01;
	SEC  = 0X00;
	HOUR = 12;
	MIN  = 11;
	VICIntEnable = 0x00002000;
	VICVectCntl0 = 0x0000002D;
	VICVectAddr0 = (unsigned)rtc_int;
}
