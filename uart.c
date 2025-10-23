#include <avr/io.h>
#include <stdio.h>

#include "config.h"
///////////////////////////////////////////////////////////
void uart_init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
///////////////////////////////////////////////////////////
void uart_transmit(unsigned char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}
///////////////////////////////////////////////////////////
void uart_print(const char *str) {
	while (*str) {
		uart_transmit(*str++);
	}
}
///////////////////////////////////////////////////////////
void envia_info(){
	char buffer[35];
	uart_print("FFT MAGN_FREC:\r\n\r\n");
	for (int i = 0; i <N_out; i++) {
		
		float frequency = (i * FSAMPLE) / N;
		sprintf(buffer,"F:%.1f Hz",frequency);
		uart_print(buffer);
		
		sprintf(buffer,", M:%.1f",Mag[i]);
		uart_print(buffer);
		uart_print("\r\n");
	}
}
///////////////////////////////////////////////////////////