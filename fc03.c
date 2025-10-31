#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "config.h"
#include "fc03.h"

// ==========================
// ISR – Interrupciones externas
// ==========================
ISR(INT2_vect) {  // Pin 19 → Derecha
    contador_der++;
}

ISR(INT3_vect) {  // Pin 18 → Izquierda
    contador_izq++;
}

// ==========================
// Configuración de encoders
// ==========================
void Encoder_init(void) {
    // INT2 e INT3 → flanco descendente
    EICRA |= (1 << ISC21) | (1 << ISC31);
    EICRA &= ~((1 << ISC20) | (1 << ISC30));

    // Habilitar interrupciones INT2 e INT3
    EIMSK |= (1 << INT2) | (1 << INT3);

    // Pines como entrada con pull-up
    DDRD &= ~((1 << PD2) | (1 << PD3)); // PD2=INT2, PD3=INT3
    PORTD |= (1 << PD2) | (1 << PD3);
}

// ==========================
// Configuración del Timer4
// ==========================
void Timer4_init(void) {
    TCCR4A = 0;               // Limpia registros
    TCCR4B = 0;
    TCCR4B |= (1 << WGM42);   // Modo CTC
    OCR4A = 2499;             // 0.01 s a 16 MHz con prescaler 64
    TIMSK4 |= (1 << OCIE4A);  // Habilita interrupción por comparación
    TCCR4B |= (1 << CS41) | (1 << CS40); // Prescaler 64
}

// ==========================
// ISR del Timer4 
// ==========================
ISR(TIMER4_COMPA_vect) {
    flag_nueva_muestra = 1;

    float rev_izq = (float)contador_izq / PULSOS_POR_REV;
    float rev_der = (float)contador_der / PULSOS_POR_REV;

    velocidad_izq = 2.0f * M_PI * RADIO_RUEDA * rev_izq * 100; // mm/s
    velocidad_der = 2.0f * M_PI * RADIO_RUEDA * rev_der * 100; // mm/s

    contador_izq = 0;
    contador_der = 0;
}