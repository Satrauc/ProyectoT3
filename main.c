/*
* main.c
*
* Created: 2/11/2025 8:39:03 PM
* Author: SANDRA NOPE FABIO RIVADENEIRA
*/
//#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "uart.h"
#include "adc.h"
#include "SignalProcessing.h"
#include "Motores.h"
#include "fc03.h"

float REX[N];// PARTE REAL
float IMX[N];// PARTE IMAGINARIA
float Mag[N];// MAGNITUDES
int I, IP, JM1, K, L, LE, LE2, NM1, ND2, M, J;
float TR, TI, UR, UI, SR, SI;

volatile uint16_t contador_izq = 0;
volatile uint16_t contador_der = 0;
float velocidad_izq= 0.0f;
float velocidad_der = 0.0f;
volatile uint8_t flag_nueva_muestra = 0;
/*
int main(void){
	char buffer[64];

	ADC_init();
	uart_init(MYUBRR);
	Encoder_init();
    Timer1_init();

	
	
	
	DDRB = 255;
	PORTB=0;
	while(1){
		/*PORTB=1;
		capture_signal(0);
		calc_FFT();
		envia_info();
		_delay_ms(300);
		PORTB=0;
		_delay_ms(300);*

		snprintf(buffer, sizeof(buffer),
                 "Vel L: %.3f m/s | Vel R: %.3f m/s\r\n",
                 velocidad_izq, velocidad_der);
        uart_print(buffer);
        _delay_ms(500);
	}
}
*/

int main(void) {
    uart_init(MYUBRR);
    Encoder_init();
    Timer4_init();
    motores_init();
    Detener();
    sei();

     _delay_ms(1000);
    char texto[50];

    while (1) {
        uart_receive_string(texto, sizeof(texto));
        if (texto[0] != '\0') {
            //GirarSobreEje(atof(texto));
            AvanzarRecto(atof(texto));
            _delay_ms(1000);
            
            memset(texto, 0, sizeof(texto));
        }
        //AvanzarRecto(10);
        //_delay_ms(1000);
        Detener();
        _delay_ms(2000);
    
    }
}