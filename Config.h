#ifndef CONFIG_H
#define CONFIG_H

// Frecuencia del micro
#define F_CPU 16000000UL

// Frecuencia de muestreo
//#define FSAMPLE 4000.0
#define FSAMPLE 3840.0
#define TS (1000000/FSAMPLE)-22   // Periodo de muestreo en microsegundos

// UART
#define BAUD 57600
#define MYUBRR F_CPU/16/BAUD-1

// FFT
#define N 64              // Cantidad de muestras
#define N_out ((N/2)+1)   // Salida FFT (mitad espectro +1)

// ---- Variables globales ----
// FFT buffers
extern float REX[N];   // parte real
extern float IMX[N];   // parte imaginaria
extern float Mag[N];   // magnitudes

// Variables auxiliares para FFT
extern int I, IP, JM1, K, L, LE, LE2, NM1, ND2, M, J;
extern float TR, TI, UR, UI, SR, SI;

// =============================
// CONFIGURACIONES DEL FC-03
// =============================
#define PULSOS_POR_REV 79
#define RADIO_RUEDA 66.4f //mm
#define DISTANCIA_ENTRE_RUEDAS_M 98.0f //mm
//#define PULSOS_POR_REV 20
//#define RADIO_RUEDA 0.00664f //mm
//#define DISTANCIA_ENTRE_RUEDAS_M 0.0098f //mm
#define MAX_VEL_SEG_DER 1079.45f    // mm/s
#define MAX_VEL_SEG_IZQ 907.81f     // mm/s
#define MIM_VEL_SEG_DER 768.39f    // mm/s
#define MIM_VEL_SEG_IZQ 589.89f     // mm/s
#define PWM_MAX 220
#define PWM_MIN 190


volatile uint16_t contador_izq;       // Conteo total de pulsos
volatile uint16_t contador_der;
extern float velocidad_izq;
extern float velocidad_der;
volatile uint8_t flag_nueva_muestra;
// =============================
// CONFIGURACIONES MOTORES
// =============================

// Mapeo de pines 
#define D1_PIN PB7   // Arduino pin 13  (Motor A ) (Naranja)
#define D2_PIN PB6   // Arduino pin 12  (Motor B ) (Amarillo)
#define D3_PIN PB5   // Arduino pin 11  (Motor B ) (Verde)
#define VT_PIN PB4   // Pin 10 del Arduino Mega    (Azul)
#define D0_PIN PH6   // Arduino pin 9  (Motor A )  (Gris)

// Movimiento recto
#define TIEMPO_MUESTREO 0.01f 
#define K_CORRECCION 100.0f 


#endif // CONFIG_H
