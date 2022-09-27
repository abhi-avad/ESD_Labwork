#include<lpc214x.h>
#include<stdio.h>
#include<string.h>

// https://www.youtube.com/watch?v=Cb21y910CKE&ab_channel=AndreasSpiess
// as shown ^, just tuning into the correct com port with the correct baud rate in PuTTY should be enough to receive the uart messages on the pc

void delay_ms(int ms){

	int i, j;

	for(j=0;j<ms;j++){
		for(i=0;i<12000;i++);
	}
}

void UART_INIT(void){

	PINSEL0 = 0x00000005; /* 1:0 = 01 -> P0.0 function set as TX, 3:2 = 01 -> P0.1 function set as RX, OR'ing is being done to make sure the function of other pins is undisturbed */
	
	U0LCR = 0x83; /* 1:0 = 11 -> 8 bit mode, 7 = 1 -> DLM and DLL can be edited, Data tx and rx OFF */

	U0DLM = 0x00; /* divisor = Pclk/(16*baudrate), for baudrate = 9600, pclk = 60Mhz divisor = 391 => usb = 0x01 */
	U0DLL = 0x60; /* divisor = Pclk/(16*baudrate), for baudrate = 9600, pclk = 60Mhz divisor = 391 => lsb = 0x87 */

	U0LCR = 0x03; /* 1:0 = 11 -> 8 bit mode, 7 = 0 -> DLM and DLL can NOT be edited, Data tx and rx ON */
}

void UART_TXCHAR(char data){
	
	U0RBR = data;
	while((U0LSR & 0x20)==0);
}

char UART_RX(void){

	char data;
	while((U0LSR & 0x01)==0);
	data = U0THR;
	return data;
}

void UART_TXSTRING(char* data){
	
	int i = 0;
	while(data[i] != 0){
		
		UART_TXCHAR(data[i]);
		i++;
	}
}

int main(void){
	
	int i = 0;
	
	UART_INIT();

	for(i=0; i<20; i++){
		
		UART_TXSTRING("Hello UART, LPC2148 to PC");
		UART_TXCHAR(0x0a); // Just the unicode for '\n' -> New Line
		UART_TXCHAR(0x0d); // Just the unicode for '\t' -> Carriage return
		delay_ms(2000);
	}
	return 0;
}
