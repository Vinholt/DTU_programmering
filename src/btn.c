#include "btn.h"
#include <avr/io.h>
#include <util/delay.h>


char check_btn(char _reg, char _pin) {
	static char last_btn_state = 0;
	char btn_pushed;
	char current_btn_state = !(_reg & (1<<_pin));
	
	if (current_btn_state != last_btn_state) {
		last_btn_state = current_btn_state;
		
		_delay_ms(20); //debounce. Wait 20ms and read pin again
		current_btn_state = !(_reg & (1<<_pin));

		if (current_btn_state == last_btn_state) {
			if (current_btn_state == 1) {
				btn_pushed = 1;
			}
			else {
				btn_pushed = 0;
			}
			return btn_pushed;
		}
	}
	
	btn_pushed = 0;
	return btn_pushed;
}	