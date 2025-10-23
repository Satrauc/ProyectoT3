#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "config.h"
#include "fc03.h"


// =============================
// INTERRUPCIÓN EXTERNA INT0
// =============================
ISR(INT0_vect) {
    contador_izq++; // Cada flanco de subida cuenta un pulso
}
// =====================
// ISR ENCODER DER (INT1)
// =====================
ISR(INT1_vect) {
    contador_der++;
}

// =============================
// TIMER1 COMPARE MATCH ISR
// =============================
ISR(TIMER1_COMPA_vect) {
     float rev_izq = (float)contador_izq / PULSOS_POR_REV;
    float rev_der = (float)contador_der / PULSOS_POR_REV;

    velocidad_izq = 2 * 3.1416f * RADIO_RUEDA * rev_izq;
    velocidad_der = 2 * 3.1416f * RADIO_RUEDA * rev_der;

    contador_izq = 0;
    contador_der = 0;
}

// =====================
// CONFIGURAR ENCODERS
// =====================
void Encoder_init(void) {
    // Configurar INT0 y INT1 como flanco de subida
    EICRA |= (1 << ISC01) | (1 << ISC00); // INT0 → flanco de subida
    EICRA |= (1 << ISC11) | (1 << ISC10); // INT1 → flanco de subida
    EIMSK |= (1 << INT0) | (1 << INT1);   // Habilitar ambas interrupciones
    // Habilitar interrupciones globales
    sei();
}

void Timer1_init(void) {
    // Timer1 modo CTC, 1 s periodo
    TCCR1B |= (1 << WGM12);
    OCR1A = 15624;
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
}