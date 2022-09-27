#include <lpc214x.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"

int main(void){
	// Variable decleration must happen before any executable code in C90 (ARM Compiler is based on C90)
	unsigned int result;
	char voltage[16];
	
	LCD_INIT();

	PINSEL1 = 0x01000000; /* P0.28 function set as AD0.1 */
	AD0CR = 0x001200402;  /* 7:0 = 00000010 -> AD0.1 is activated, 15:8 = 00000100 -> ?, 16 = 0 -> Normal conversion needs 11 clks, 19:17 = 000, 21 = 1 -> is like EN for ADC, 26:24 = 001 -> Start Conversion  */
	
	while(1){
		
		AD0CR = 0x001200402; 							// !! Comment to be added
		
		while ( !(AD0DR1 & 0x80000000) ); /* Last bit is set 1 if conversion is done -> Wait till DONE */
		result = AD0DR1;

		result = (result >> 6); 					/* 15:6 = Result, hence 6 bit right shift to get data */
		result = (result & 0x000003FF); 	/* 9:0 bits are data rest are made 0*/

		snprintf(voltage, 16, "V=%f", ((result/1023.0) * 3.3 )); /* Convert ADC value by scaling with Vref = 3.3V */

		LCD_CHAR(0x01, 0);  /* Display clear */
		LCD_CHAR(0x80, 0);  /* Cursor at home position */
		delay_ms(10);
		LCD_STRING(voltage);

		delay_ms(90);
	}
	return 0;
}
