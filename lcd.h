#include<lpc214x.h>

void delay_ms(int ms){

	int i, j;

	for(j=0;j<ms;j++){
		for(i=0;i<12000;i++);
	}
}

void LCD_CHAR(char data, int lcd_reg){
	if (lcd_reg == 1){		
		IO0SET = 0x00010000; /* RS = 1 => Data Reg of LCD selected*/
	}
	else if (lcd_reg == 0) {
		IO0CLR = 0x00010000; /* RS = 0 => Instruction Reg of LCD selected*/
	}
	
	IO0PIN = ( (IO0PIN & 0xFF0FFFFF) | ((data & 0xF0)<<16) ); /* Upper nibble of command */
	IO0SET = 0x00020000; /* EN = 1 */
	delay_ms(5);
	
	IO0CLR = 0x00020000; /* EN = 0*/
	delay_ms(5);
	
	IO0PIN = ( (IO0PIN & 0xFF0FFFFF) | ((data & 0x0F)<<20) ); /* Lower nibble of command */
	IO0SET = 0x00020000; /* EN = 1 */
	delay_ms(5);
	
	IO0CLR = 0x00020000; /* EN = 0*/
	delay_ms(5);
	
	/* RW is always 0, we dont have access*/	
}

void LCD_INIT(void){
	IO0DIR = 0x00F30000; /* P0.20 to P0.23 LCD Data. P0.16,17 as RS,EN */
	delay_ms(20);
	LCD_CHAR(0x02, 0);  /* Initialize lcd in 4-bit mode */
	LCD_CHAR(0x28, 0);  /* Configures LCD in 2-line, 4-bit mode, and 5x8 dots. */
	LCD_CHAR(0x0C, 0);  /* Display ON cursor OFF */
	LCD_CHAR(0x06, 0);  /* Auto increment cursor */
	LCD_CHAR(0x01, 0);  /* Display clear */
	LCD_CHAR(0x80, 0);  /* Cursor at home position */
}


void LCD_STRING (char* msg){
	
	int i = 0;
	while(msg[i]!=0)
	{
		LCD_CHAR(msg[i], 1);
		i++;
	}
}
