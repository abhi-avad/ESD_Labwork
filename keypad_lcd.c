#include <lpc214x.h>
#include <stdint.h>
#include <stdio.h>
#include "lcd.h"

char keys[4][4] = 	{{'1', '2', '3', '4'},	/* Keypad standard arrays */
										{'5', '6', '7', '8'},
										{'9', '0', 'a', 'b'},
										{'c', 'd', 'e', 'f'}};

uint32_t rows[4] = {0x00fe0000, 0x00fd0000, 0x00fb0000, 0x00f70000};
uint32_t cols[4] = {0x00100000, 0x00200000, 0x00400000, 0x00800000};

char KEYPAD_CHECK(void){

	int i,j;
	
	for(i=0; i<4; i++){

		IO1SET = rows[i];
		delay_ms(5);

		for(j=0; j<4; j++){
			
			if((IO1PIN&cols[j]) == 0){

				while((IO1PIN&cols[j]) == 0); /* Uncomment this if expected op is not observed */
				return(keys[i][j]);		/* If inside col-i row-j is pressed */
			}			
		}

		IO1CLR = rows[i];
		delay_ms(5);
	}
	return 'x';
}

int main(void)
{
	char c_prt;
	
	LCD_INIT();
	LCD_CHAR(0xC0, 0); /* Clear screen */
	
	/* Keypad I/O setup */
	IO1DIR = 0x000F0000; /* C4-C1 = P1.23-P1.20 --> Output, R4-R1 = P1.19-P1.16 --> Input */
	
	while(1){
		
		delay_ms(5);
		c_prt = KEYPAD_CHECK();
		if(c_prt != 'x'){
			LCD_CHAR(0xC0, 0);
			LCD_CHAR(c_prt, 1);
		}
	}
	return 0;
}
