
#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>
#include "config.h"
#include "uart.h"
#include "Motores.h"

//  Inicializa pines 
void motores_init(void) {
    // Configurar pines como salida
    DDRH |= (1 << D0_PIN);
    DDRB |= (1 << D1_PIN) | (1 << D2_PIN) | (1 << D3_PIN) | (1 << VT_PIN);

    // Habilitar puente
    PORTB |= (1 << VT_PIN);

    // ---------- PWM Timer2 -> Motor Izquierdo ----------
    // OC2B = PH6 = Pin 9
    TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);  // Fast PWM 8-bit
    TCCR2B = (1 << CS21);   // Prescaler 8 → frecuencia PWM ≈ 3.9 kHz
    OCR2B = 0;              

    // ---------- PWM Timer1 -> Motor Derecho ----------
    // OC1A = PB5 (Pin 11)
    TCCR1A = (1 << COM1A1) | (1 << WGM10);  // Fast PWM 8-bit
    TCCR1B = (1 << WGM12) | (1 << CS11);    // Prescaler 8
    OCR1A = 0;              
}

// Avanzar: ambos motores adelante
void Avanzar(uint8_t pwm_izq, uint8_t pwm_der) {
    // Motor izquierdo (A)
    PORTB &= ~(1 << D1_PIN); // D1 = 0 (sentido adelante)
    OCR2B = pwm_izq;         // PWM en PH6 (pin 9)

    // Motor derecho (B)
    PORTB &= ~(1 << D2_PIN); // D2 = 0 (sentido adelante)
    OCR1A = pwm_der;         // PWM en PB5 (pin 11)
}

// Retroceder: ambos motores atrás
void Retroceder(uint8_t pwm_izq, uint8_t pwm_der) {
    // Motor izquierdo (A)
    PORTB |= (1 << D1_PIN);  // D1 = 1 (sentido atrás)
    OCR2B = pwm_izq;         // PWM en PH6 (pin 9)

    // Motor derecho (B)
    PORTB |= (1 << D2_PIN);  // D2 = 1 (sentido atrás)
    OCR1A = pwm_der;         // PWM en PB5 (pin 11)
}


// Detener ambos motores
void Detener(void) {
    // Poner PWM en 0
    OCR2B = 0;
    OCR1A = 0;
}

void AvanzarRecto(float distancia_cm) {
    
    float distancia_obj = distancia_cm / 100.0;  // pasar a metros
    float distancia_recorrida = 0;
    float pwm_der = 220;  // valor inicial de PWM
    float pwm_izq = 220;
    flag_nueva_muestra = 0;  
    char buffer[128];
    Detener();
    _delay_ms(200);

    Avanzar((int)pwm_izq, (int)pwm_der);
    //_delay_ms(200);  

    while (distancia_recorrida < distancia_obj) {
        

        // Calcula diferencia de velocidades
        float error = velocidad_der - velocidad_izq;

        // Corrección proporcional
        pwm_der -= K_CORRECCION * error;
        pwm_izq += K_CORRECCION * error;

        // Limitar valores PWM
        if (pwm_der > 255) pwm_der = 255;
        if (pwm_der < 0) pwm_der = 0;
        if (pwm_izq > 255) pwm_izq = 255;
        if (pwm_izq < 0) pwm_izq = 0;

        // Aplicar nueva velocidad
        Avanzar((int)pwm_izq, (int)pwm_der);

        // Calcular distancia recorrida aproximada (promedio)
        float vel_prom = (velocidad_der + velocidad_izq) / 2.0;
        distancia_recorrida += vel_prom * TIEMPO_MUESTREO;

        //while (!flag_nueva_muestra);  
        //flag_nueva_muestra = 0;        // limpiar bandera

        _delay_ms(5);
        
        snprintf(buffer, sizeof(buffer),
                 "distancia: %.3f \n", distancia_recorrida*100);
        uart_print(buffer);
    }

    Detener();
}

void RetrocederRecto(float distancia_cm) {
    float distancia_obj = distancia_cm / 100.0;  // pasar a metros
    float distancia_recorrida = 0;
    float pwm_der = 220;  // valor inicial de PWM
    float pwm_izq = 220;

    // Asegurar que los motores estén quietos antes de arrancar
    Detener();
    _delay_ms(200);

    // Arrancar en retroceso
    Retroceder((int)pwm_izq, (int)pwm_der);
    _delay_ms(200);  // Dar tiempo a que empiece el movimiento y el timer actualice velocidades

    while (distancia_recorrida < distancia_obj) {
        // Diferencia de velocidades
        float error = velocidad_der - velocidad_izq;

        // Corrección proporcional inversa
        pwm_der -= K_CORRECCION * error;
        pwm_izq += K_CORRECCION * error;

        // Limitar valores de PWM entre 0–255
        if (pwm_der > 255) pwm_der = 255;
        if (pwm_der < 0) pwm_der = 0;
        if (pwm_izq > 255) pwm_izq = 255;
        if (pwm_izq < 0) pwm_izq = 0;

        // Aplicar los nuevos valores de PWM
        Retroceder((int)pwm_izq, (int)pwm_der);

        // Calcular la distancia recorrida (en valor absoluto)
        float vel_prom = (velocidad_der + velocidad_izq) / 2.0;
        distancia_recorrida += vel_prom * TIEMPO_MUESTREO;

        _delay_ms((int)(TIEMPO_MUESTREO * 1000));
    }

    Detener();
}