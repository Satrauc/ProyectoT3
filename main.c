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
volatile float velocidad_izq = 0.0f;
volatile float velocidad_der = 0.0f;
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
    motores_init();

    while (1) {
        motores_avanzar();
        _delay_ms(3000);

        motores_detener();
        _delay_ms(1000);

        motores_retroceder();
        _delay_ms(3000);

        motores_detener();
        _delay_ms(1000);
    }
    return 0;
}

/*
// =============================
// ISR para INT4 → Pin 2 (Encoder Izquierdo)
// =============================
ISR(INT4_vect) {
    contador_izq++;
}

// =============================
// ISR para INT5 → Pin 3 (Encoder Derecho)
// =============================
ISR(INT5_vect) {
    contador_der++;
}

// =============================
// Inicialización de encoders
// =============================
void SetupEncoder(void) {
    // Configurar INT4 e INT5 para flanco descendente (FC-03 activa en LOW)
    EICRB |= (1 << ISC41);     // ISC41 = 1, ISC40 = 0 → flanco descendente INT4
    EICRB &= ~(1 << ISC40);
    EICRB |= (1 << ISC51);     // ISC51 = 1, ISC50 = 0 → flanco descendente INT5
    EICRB &= ~(1 << ISC50);

    // Habilitar interrupciones INT4 e INT5
    EIMSK |= (1 << INT4) | (1 << INT5);

    // Configurar pines como entrada con pull-up
    DDRE &= ~((1 << PE4) | (1 << PE5));
    PORTE |= (1 << PE4) | (1 << PE5);

    // Habilitar interrupciones globales
    sei();
}

// =============================
// Main
// =============================
int main(void) {
    char buffer[64];

    uart_init(MYUBRR);   // Tu función UART ya implementada
    SetupEncoder();      // Configurar los encoders

    uart_print("=== Prueba FC-03 con INT4 e INT5 ===\r\n");

    while (1) {
        snprintf(buffer, sizeof(buffer), "Izq: %u  |  Der: %u\r\n", contador_izq, contador_der);
        uart_print(buffer);
        _delay_ms(500);
    }
}*/