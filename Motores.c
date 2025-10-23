
#include <avr/io.h>
#include <stdbool.h>

#include "config.h"
#include "Motores.h"

// 🔹 Inicializa los pines de salida
void motores_init(void) {
    DDRE |= (1 << D0_PIN) | (1 << D3_PIN);
    DDRH |= (1 << D1_PIN) | (1 << D2_PIN);

    // Todos apagados inicialmente
    PORTE &= ~((1 << D0_PIN) | (1 << D3_PIN));
    PORTH &= ~((1 << D1_PIN) | (1 << D2_PIN));
}

// 🔹 Avanzar (ambos motores en la misma dirección)
void motores_avanzar(void) {
    // Si actualmente giran al revés, simplemente intercambia los valores
    PORTE &= ~(1 << D0_PIN);   // IN1 = 0
    PORTH |= (1 << D1_PIN);    // IN2 = 1
    PORTH &= ~(1 << D2_PIN);   // IN3 = 0
    PORTE |= (1 << D3_PIN);    // IN4 = 1
}

// 🔹 Retroceder
void motores_retroceder(void) {
    PORTE |= (1 << D0_PIN);    // IN1 = 1
    PORTH &= ~(1 << D1_PIN);   // IN2 = 0
    PORTH |= (1 << D2_PIN);    // IN3 = 1
    PORTE &= ~(1 << D3_PIN);   // IN4 = 0
}

// 🔹 Detener
void motores_detener(void) {
    PORTE &= ~((1 << D0_PIN) | (1 << D3_PIN));
    PORTH &= ~((1 << D1_PIN) | (1 << D2_PIN));
}