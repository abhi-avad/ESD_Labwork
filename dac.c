#include<lpc214x.h>
#include<stdio.h>
#include<math.h>

void delay(int i){

	while(i){
		i--;
	}
}

void sawtooth(int del){
	
	int i;
	
	for(i = 0; i<1024; i++){
		
		DACR = i<<6; /* Left shift as 5:0 = Reserved, 15:6 = Value in DACR */
		delay(del);
	}
}

void square(int del){
	
	DACR = 0<<6; 			/* Left shift as 5:0 = Reserved, 15:6 = Value in DACR */
	delay(del*1024);	/* Multiplied by 1024 to keep the frequency same as the other signals */
	DACR = 1023<<6; 	/* Left shift as 5:0 = Reserved, 15:6 = Value in DACR */
	delay(del*1024);
}

void triangle(int del){

	int i;
	
	for(i = 0; i<1024; i++){
		
		DACR = i<<6; /* Left shift as 5:0 = Reserved, 15:6 = Value in DACR */
		delay(del);
	}
	for(i = 1023; i>=0; i--){
		
		DACR = i<<6; /* Left shift as 5:0 = Reserved, 15:6 = Value in DACR */
		delay(del);
	}
}

void dc(float val){

	int result;
	result = floor(val/3.3*1024);
	
	if(result>1024){
		DACR = 1023<<6;
	}
	else if(result<0){
		DACR = 0<<6;
	}
	else{
		DACR = result<<6;
	}
}

void sinewave(int del){
	
	int i, result;
	float s[32] = {0.5,0.597,0.691,0.778,0.853,0.916,0.962,0.99,1,0.99,0.962,0.916,0.854,0.778,0.692,0.598,0.501,0.403,0.309,0.223,0.147,0.085,0.038,0.01,0,0.009,0.038,0.084,0.145,0.221,0.307,0.401};
	/* s[] is just a lookup table for 32 evenly spaced sine values in 1 time period */
	/* DONT remember these values, generate them using excel and copy it here */
		
	for(i=0; i<32; i++){	
		
		result = 1023*s[i];
		DACR = result<<6;
		
		delay(del*32);
	}
}

int main(void){
	
	PINSEL1 = 0x00080000; /* 19:18 = 10 -> P0.25 is configured to be Aout(DAC) */
	
	while(1){ /* Uncomment the signal to be generated */
		
		//square(10000);
		//triangle(10000);
		//sawtooth(10000);
		//sinewave(10000);
		//dc(1.65);
	}	
	return 0;
}
