
#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>
#include "config.h"
#include "uart.h"
#include "Motores.h"

//  Inicializa pines 
void motores_init(void) {
    // Configurar pines como salida
    DDRH |= (1 << D0_PIN);
    DDRB |= (1 << D1_PIN) | (1 << D2_PIN) | (1 << D3_PIN) | (1 << VT_PIN);

    // Habilitar puente
    //PORTB |= (1 << VT_PIN);

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

float CalcularPWM_Der(float velocidad_mm_s) {
    if (velocidad_mm_s <= MIM_VEL_SEG_DER)
        return PWM_MIN;
    else if (velocidad_mm_s >= MAX_VEL_SEG_DER)
        return PWM_MAX;

    // Regla de tres lineal
    float pwm = PWM_MIN + 
        ((velocidad_mm_s - MIM_VEL_SEG_DER) * (PWM_MAX - PWM_MIN)) /
        (MAX_VEL_SEG_DER - MIM_VEL_SEG_DER);
    return pwm;
}

float CalcularPWM_Izq(float velocidad_mm_s) {
    if (velocidad_mm_s <= MIM_VEL_SEG_IZQ)
        return PWM_MIN;
    else if (velocidad_mm_s >= MAX_VEL_SEG_IZQ)
        return PWM_MAX;

    // Regla de tres lineal
    float pwm = PWM_MIN + 
        ((velocidad_mm_s - MIM_VEL_SEG_IZQ) * (PWM_MAX - PWM_MIN)) /
        (MAX_VEL_SEG_IZQ - MIM_VEL_SEG_IZQ);
    return pwm;
}

void AvanzarRecto(float distancia_cm) {

    //float distancia_cm = 40.0;
    float offset = 50.0;
    float distancia_obj = distancia_cm * 10.0 + offset; 
    float distancia_recorrida = 0;
    float pwm_der = PWM_MIN;  // valor inicial de PWM
    float pwm_izq = PWM_MIN;
    flag_nueva_muestra = 0;  
    char buffer[128];
    Detener();
    _delay_ms(200); 

    while (distancia_recorrida < distancia_obj) {
        

        // Calcula diferencia de velocidades
        float error = CalcularPWM_Der(velocidad_der) - CalcularPWM_Izq(velocidad_izq);

        // Corrección proporcional
        pwm_der -= K_CORRECCION * error;
        pwm_izq += K_CORRECCION * error;
        
        /*pwm_der -= K * error;
        pwm_izq += K * error;*/

        // Limitar valores PWM
        if (pwm_der > PWM_MAX) pwm_der = PWM_MAX;
        if (pwm_der < PWM_MIN) pwm_der = PWM_MIN;
        if (pwm_izq > PWM_MAX) pwm_izq = PWM_MAX;
        if (pwm_izq < PWM_MIN) pwm_izq = PWM_MIN;

        // Aplicar nueva velocidad
        Avanzar((int)pwm_izq, (int)pwm_der);

        // Calcular distancia recorrida aproximada (promedio)
        float vel_prom = ((velocidad_der + velocidad_izq) / 2.0 );
        distancia_recorrida += vel_prom * TIEMPO_MUESTREO;

        
        
        snprintf(buffer, sizeof(buffer),
                 "distancia: %.3f mm Valocidad: %.3f mm/s distancia objetivo: %.3f  mm \n", distancia_recorrida, vel_prom, distancia_obj);
        uart_print(buffer);
    }

    Detener();
    _delay_ms(200); 
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

void GirarSobreEje(float angulo_grados) {
    char buffer[128];
    // 1. Calcular distancia objetivo
    // Convertir grados a radianes
    float angulo_rad = angulo_grados * (M_PI / 180.0);
    
    // Calcular la distancia de arco que debe recorrer la RUEDA MÓVIL
    // Radio (r) = DISTANCIA_ENTRE_RUEDAS_M
    // Distancia (s) = r * angulo_rad
    float distancia_obj = fabsf(angulo_rad * DISTANCIA_ENTRE_RUEDAS_M);

    float distancia_recorrida = 0;
    const int pwm_giro = 230; // Usamos un PWM constante

    // Asegurar que esté detenido
    Detener();
    _delay_ms(200); 

    // 2. Iniciar el giro en la dirección correcta
    if (angulo_grados > 0) {
        // Gira Izquierda: Pivote izquierdo, mueve rueda DERECHA
        Avanzar(0, pwm_giro);
    } else {
        // Gira Derecha: Pivote derecho, mueve rueda IZQUIERDA
        Avanzar(pwm_giro, 0);
    }
    _delay_ms(100); // Dar tiempo a que arranque
    snprintf(buffer, sizeof(buffer),
                 "distancia: %.3f  distancia objetivo: %.3f\n", distancia_recorrida, distancia_obj);
    uart_print(buffer);
    // 3. Bucle de control hasta alcanzar el ángulo
    while (distancia_recorrida < distancia_obj) {
        
        float velocidad_movil;

        // Leemos la velocidad SOLO de la rueda que se está moviendo
        if (angulo_grados > 0) {
            // Girando Izquierda, medimos rueda DERECHA
            velocidad_movil = velocidad_der;
        } else {
            // Girando Derecha, medimos rueda IZQUIERDA
            velocidad_movil = velocidad_izq;
        }

        // Calcular distancia recorrida (solo de la rueda que se mueve)
        distancia_recorrida += velocidad_movil * TIEMPO_MUESTREO;
        
        // Esperar el tiempo de muestreo
        _delay_ms((int)(TIEMPO_MUESTREO * 1000));

        snprintf(buffer, sizeof(buffer),
                 "distancia: %.3f  distancia objetivo: %.3f\n", distancia_recorrida, distancia_obj);
        uart_print(buffer);
        
    }

    // 4. Detener motores
    Detener();
    _delay_ms(200); // Pausa post-giro
}
