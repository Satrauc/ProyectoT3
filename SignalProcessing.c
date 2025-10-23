#include <avr/io.h>
#include <math.h>

#include "config.h"
///////////////////////////////////////////////////////////
void calc_FFT(){
	NM1 = N-1;
	ND2 = N/2;
	M = ceil(log(N)/log(2));
	J = ND2;
	for(I=1; I<=N-2; I++){
		
		if(I>=J){
			goto L1;
		}
		
		TR = REX[J];
		TI = IMX[J];
		
		REX[J] = REX[I];
		IMX[J] = IMX[I];
		REX[I] = TR;
		IMX[I] = TI;
		
		L1: K = ND2;
		
		L2: if(K>J){
			goto L3;
		}
		J = J-K;
		K = K/2;
		
		goto L2;
		
		L3: J = J+K;
	}

	for(L=1; L<=M; L++){
		LE = ceil(pow(2,L));//busca entero mas cercano
		LE2 = LE/2;
		UR = 1;
		UI = 0;
		SR = cos(M_PI/LE2);
		SI = -sin(M_PI/LE2);
		
		for(J=1; J<=LE2; J++){
		JM1 = J-1;
		
		for(I=JM1; I<=NM1; I=I+LE){
			IP = I+LE2;
			TR = REX[IP]*UR-IMX[IP]*UI;
			TI = REX[IP]*UI+IMX[IP]*UR;
			REX[IP] = REX[I]-TR;
			IMX[IP] = IMX[I]-TI;
			REX[I] = REX[I]+TR;
			IMX[I] = IMX[I]+TI;
		}
		
		TR = UR;
		UR = TR*SR-UI*SI;
		UI = TR*SI+UI*SR;
		}
	}
	
	REX[0]=REX[0]/2;
	IMX[0]=IMX[0]/2;
	
	for(L=0; L<=N-1; L++){
		REX[L] = REX[L]/ND2;
		IMX[L] = IMX[L]/ND2;
	}
	for(L=0; L<=((N/2)-1); L++){
		Mag[L] = sqrt(REX[L]*REX[L] + IMX[L]*IMX[L]);
	}
}
///////////////////////////////////////////////////////////
void ventana_hammin(void) {
	for (uint16_t i = 0; i < N; i++) {
		float w = 0.54 - 0.46 * cosf((2.0 * (float)M_PI * i) / (N - 1));
		REX[i] *= w;
	}
}
///////////////////////////////////////////////////////////
void ventana_hannin(void) {
	for (uint16_t i = 0; i < N; i++) {
		float w = 0.5 - 0.5 * cosf((2.0 * (float)M_PI * i) / (N - 1));
		REX[i] *= w;
	}
}
///////////////////////////////////////////////////////////