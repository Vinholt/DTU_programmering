#include <avr/io.h>
#include "ssd1306.h"


char readDIP(char dip_reg) {
	char dip_byte;
	dip_byte =~ dip_reg;
	return dip_byte;
}

void print_dip_pos(char dip_reg) {
	char c_buffer = readDIP(dip_reg);
	char c;

	for (int i=0; i<8; i++) {
		c = '0' + !!(c_buffer & (1 << i));
		sendCharXY(c,2,11-i);
	}
}