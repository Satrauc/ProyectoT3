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
#define PULSOS_POR_REV 20
#define RADIO_RUEDA 0.063f // metros

extern volatile uint16_t contador_izq;
extern volatile uint16_t contador_der;
extern volatile float velocidad_izq;
extern volatile float velocidad_der;


#endif