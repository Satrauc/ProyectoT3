//  Inicializa pines y PWM en VT (PB4 = OC2A)
void motores_init(void);

//  Avanzar (ambos motores hacia adelante)
void Avanzar(uint8_t pwm_izq, uint8_t pwm_der);

// Retroceder: ambos motores atrás
void Retroceder(uint8_t pwm_izq, uint8_t pwm_der);

// Detener ambos motores
void Detener(void);

void print();

// Nivela los motores usando diferencial de velocidades
void AvanzarRecto(float distancia_cm); 

void RetrocederRecto(float distancia_cm);