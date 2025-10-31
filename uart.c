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
void uart_receive_string(char *buffer, uint8_t max_length) {
    uint8_t i = 0;
    char c = 0;

    while (1) {
        while (!(UCSR0A & (1 << RXC0)));  
        c = UDR0;

        
        if (c == '\r' || c == '\n') {
            buffer[i] = '\0';            
            uart_print("\r\n");            
            break;
        }

        if (i < (max_length - 1)) {
            buffer[i++] = c;
            uart_transmit(c);              
        }
    }
}
///////////////////////////////////////////////////////////
void print(){
    char buffer[128];
    snprintf(buffer, sizeof(buffer),
                 "Izquierda: %u  Derecha: %u   \r\n",
                 contador_izq, contador_der);
    /*snprintf(buffer, sizeof(buffer),
                 "Vel Derecha: %.3f m/s     Vel Izquierda: %.3f m/s  \r\n",
                 velocidad_der,  velocidad_izq);*/

    uart_print(buffer);
}