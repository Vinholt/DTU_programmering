/*
----------------------------
 Datateknik & programmering 
 		Opgave 2
----------------------------

Oscar Vinholt s182500
Mehrdad
Kasper Nyhus Kaae s195087

*/ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "I2C.h"
#include "ssd1306.h"
#include "dipswitch.h"


void on_btn_push();

volatile char intr_flag = 0; // interrupt flag

void init() {
	I2C_Init();
	InitializeDisplay();
	clear_display();
  	sendStrXY("Dip Switch", 1,3);
	
	// initialize external interrupt INT4
	EICRB |= (1<<ISC41);  // 1 
	EICRB &=~ (1<<ISC40); // 0      NOTE: trigger on FALLING edge
	EIMSK |= (1<<INT4);   // enable INT4

	// set PE4 as INPUT PULLUP
	DDRE &=~ (1<<4); // set PE4 INPUT 0
	PORTE |= (1<<4); // set PE4 PULLUP 1
	
	// Dip-Switch
	// set register K as INPUT PULLUP
	DDRK = 0x00;  // 0000 0000
	PORTK = 0xFF; // 1111 1111
}

int main(void)
{
	init();
	sei(); // global interrupt enable
	
	print_dip_pos(PINK);
	sendCharXY('1',3,1);

	while (1)
    {
		if (intr_flag == 1) {
			_delay_ms(80);
			intr_flag = 0;
			on_btn_push();
      }
    }
}


ISR(INT4_vect) { // Interupt Service Routine (INT4)
  intr_flag = 1;
}


void on_btn_push() {
	static uint8_t num = 0;
	char c_buffer;
	static char c_array[] = {' ', ' ', ' ', '\0'};

	sendStrXY("     ",5,6);
	c_buffer = readDIP(PINK);
	c_array[num] = c_buffer;
	sendCharXY(c_buffer,3,6+num);
	
	// Increment counter
	if (num < 2) {
		num++;
	}
	else {
		num = 0;
		sendStrXY(c_array,5,6);
		sendStrXY("     ",3,6);
	}
	c_buffer = '1' + num;
	sendCharXY(c_buffer,3,1);

	print_dip_pos(PINK);
}


