
// Mapeo de pines según tu conexión real
#define D0_PIN PE5   // Arduino pin 3  (Motor A )
#define D1_PIN PH4   // Arduino pin 7  (Motor A )
#define D2_PIN PH3   // Arduino pin 6  (Motor B )
#define D3_PIN PE3   // Arduino pin 5  (Motor B )

// Pin PWM para control de velocidad (usaremos OC2B → Pin 9 → PH6)
#define PWM_DDR DDRG
#define PWM_PIN PG5

//void motores_init(void); 

/* -------- Funciones para motor A (D0/ D1) --------
   motor A: D0 = OC3C (OCR3C)  -> avance cuando PWM en D0 y D1 = 0
            D1 = OC4B (OCR4B)  -> retroceso cuando PWM en D1 y D0 = 0
*/
//void motorA_set(uint8_t speed, bool forward);

/* -------- Funciones para motor B (D2 / D3) --------
   motor B: D2 = OC4A (OCR4A) -> avance si PWM en D2 y D3=0
            D3 = OC3A (OCR3A) -> retroceso si PWM en D3 y D2=0
*/
//void motorB_set(uint8_t speed, bool forward);


//void motores_detener(void);


// 🔹 Inicializa los pines de salida
void motores_init(void);

// 🔹 Avanzar (ambos motores en la misma dirección)
void motores_avanzar(void);

// 🔹 Retroceder
void motores_retroceder(void);

// 🔹 Detener
void motores_detener(void);

