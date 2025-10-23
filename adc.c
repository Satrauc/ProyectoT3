#include <avr/io.h>
#include <util/delay.h>

#include "config.h"
///////////////////////////////////////////////////////////
void ADC_init() {
	DIDR0=0x00;
	DIDR2=0x00;
	ADMUX=0x40;
	ADCSRA=0x84;
	ADCSRB=0x00;
}
///////////////////////////////////////////////////////////
unsigned int ADC_read(unsigned char adc_input)
{
	ADMUX=(adc_input & 0x1F) | 0X40;
	
	if (adc_input & 0x20) ADCSRB|=(1<<MUX5);
	else ADCSRB&=~(1<<MUX5);
	_delay_us(1);
	
	ADCSRA|=(1<<ADSC);
	
	while ((ADCSRA & (1<<ADIF))==0);
	ADCSRA|=(1<<ADIF);
	
	return ADCW;
}
///////////////////////////////////////////////////////////
void capture_signal(unsigned char canal) {
	for (unsigned int i = 0; i < N; i++) {
		REX[i] = ADC_read(canal);
		_delay_us(TS);
	}
}
///////////////////////////////////////////////////////////