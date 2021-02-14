
#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"
#define V_MAX 3.3
#define TIMING 50
#define BPM_TIMING 10000

void init_adc(void)
{
	ADCSRA |= _BV(ADEN)
	 			 | _BV(ADPS2) | _BV(ADPS1);
}

uint16_t read_adc(void)
{
	ADCSRA |= _BV(ADSC);
	while(ADCSRA & _BV(ADSC));
	return ADC;

}

int main(void)
{
	uint16_t result;
	uint16_t state = 0;
	uint16_t heart_beat = 0;
	uint16_t count = 0;
	double bpm = 0;

  double voltage;
	init_debug_uart0();
	init_adc();

	for (;;)
	{

		 result = read_adc();
		 voltage = (V_MAX/1023) * result;

		printf("%4d : %6.5fV", result, voltage);

			if((result > 900) && (state == 0)){
					state = 1;
					printf(" heart beat %d", heart_beat);
    			heart_beat++;
			}

			if(result < 400){
				state = 0;
			}

		_delay_ms(TIMING);
 		count += TIMING;

		if(count >= BPM_TIMING){
			bpm = heart_beat*4.8;
			heart_beat = 0;
			count = 0;
			printf(" bpm %f", bpm);

		}
		printf("\n");

	}
}
