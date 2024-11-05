#pragma config FOSC = INTRCIO, WDTE = OFF, PWRTE = OFF, MCLRE = OFF, CP = OFF
#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, FCMEN = OFF
#include <xc.h>					// Carrega el fitxer de funcions necessari per al compilador XC8
#include <stdlib.h>				// Carrega el fitxer on hi ha la funció rand()
#define _XTAL_FREQ  4000000			// La freqüència del rellotge és 4 MHz
char Port;					// Gestió del port a la funció Envia_max
char Compta;					// Comptador de bits a la funció Envia_max
char Sortida[6];				// Valors a enviar al MAX7221 (48 bits)
char Sorti[6];					// Valors a enviar al MAX7221 des de la interrupció
char Actiu; 					// Variable que diu quin color està actiu
						// Actiu = 0		Apagat
						// Actiu = 1		Vermell
						// Actiu = 2		Verd
						// Actiu = 3		Blau
char Polsad;					// Polsador que s'ha premut
char figura[8];					// Aquí guardarem el dibuix (8 fileres)
char x = 3;					// Coordenada X del cursor (0 a 7)
                        // X = 0 és la columna de la dreta
char mascara = 0b11000000;
char mascara2 = 0b11000000;
char y = 0;					// Coordenada Y del cursor (0 a 7)
char mirar = 1;					// Espera que es deixi anar el polsador
char actualitza = 0;
char x2=3;
char y2=7;
char FilaBola = 3;					// Fila de la bola
char d= 0;// Direccion de la bola, entre 0 y 2 | 0 Izq  -  1 Recto  -  2 Dch
char Bola= 0b01000000;
char Sentido = 0; // Sentido de la Bola, Sentido=0 Para Abajo | Sentido=1 Para Arriba
// Definició de les funcions que farem servir 
void Envia3max(char Valor[]);			// Envia un joc de valors als tres MAX7221
						// desactivant interrupcions
void Envia_max(void);				// Envia un joc de valors als tres MAX7221
void Ini3max(void);				// Inicialitza els tres MAX7221
void Apaga(void);				// Apaga tots els LED
char Polsador(void);				// Funció de lectura dels polsadors
void main (void) {
	OPTION_REG = 0b10000101;       		// Configuració de Timer0
						// Com a temporitzador basat en rellotge
						// 101 - Factor d'escala de 64
						// I resistències de pull-up desactivades (valor per defecte)
	TRISC = 0;				// Tot el port C és de sortida
	TRISB = 0;				// Tot el port B és de sortida
	TRISA = 0xFF;				// Tot el port A és d'entrada
	ANSEL = 0b00000101;			// Configura AN0 i AN2 com entrada analògica
	ANSELH = 0;				// Desactiva les altres entrades analògiques
	PORTC = 0;				// Inicialitza a 0 el port C
	PORTB = 0;				// Inicialitza a 0 el port B
	ADCON1 = 0b00010000;			// Posa el conversor a 1/8 de la freqüència
	ADCON0 = 0b00001001;			// Activa el conversor A/D connectat a AN2
						// amb el resultat justificat per l'esquerra
	Ini3max();				// Inicialitza els tres MAX7221
	Actiu = 1;				// Activa el color vermell
	TMR0 = 100;				// Presselecció de 100, que són 156 iteracions
						// Correspon a una interrupció cada 7,5 ms
	INTCON = 0b10100000;			// Activem GIE i T0IE
	Apaga();				// Apaga tots els LED
	for (signed char k = 0; k < 8; k++){
		figura[k] = 0;			// Comencem amb tots els LED apagats
	}
	while (1) {
		// Mirem els polsadors
		// Un cop s'ha fet l'acció del polsador, no es tornarà a fer fins
		// que es detecti que s'han deixat anar
        if (actualitza > 8){
            if (Sentido == 0){		// Si estem a la filera del cursor
                if (FilaBola < 7){
                    char Comprobacio;
                    //IZQ
                    if(d==0){
                        if(FilaBola == 6){
                            Comprobacio = (Bola << 1) & mascara2; //DETECTA PALA
                            if(Comprobacio != 0){ 
                                Sentido = 1;
                                // Rand direccion
                        
                            }
                        }
                        Bola = Bola << 1 ;
                        FilaBola++ ;
                        if(Bola == 0x10000000){
                            d=2;
                            
                        }
                    }
                    else{
                        
                        //CENTRO
                        if(d==1){

                            if(FilaBola == 6){

                                Comprobacio = Bola & mascara2; //DETECTA PALA
                                if(Comprobacio != 0){ 
                                    Sentido = 1;
                                    // Rand direccion
                                }
                            }
                            FilaBola++ ;
                        }
                        //DCH
                        if(d==2){

                            if(FilaBola == 6){

                                Comprobacio = (Bola >> 1) & mascara2; //DETECTA PALA
                                if(Comprobacio != 0){ 
                                    Sentido = 1;
                                    // Rand direccion

                                }
                            }
                            Bola = Bola >> 1 ;
                            FilaBola++ ;
                            if(Bola == 0x00000001){
                                d=0;

                            }
                        }
                    }
                }
                

            }
            else{
                
            
                if (Sentido == 1){		// Si estem a la filera del cursor
                    if (FilaBola > 0){
                        char Comprobacio2;
                        if(d==0){
                            if(FilaBola == 1){
                                Comprobacio2 = (Bola << 1) & mascara; //DETECTA PALA
                                if(Comprobacio2 != 0){ 
                                    Sentido = 0;
                                    // Rand direccion

                                }
                            }
                            Bola = Bola << 1 ;
                            FilaBola-- ;
                            if(Bola == 0x10000000){
                                d=2;

                            }

                        }
                        else{
                            if(d==1){
                                if(FilaBola == 1){
                                    Comprobacio2 = Bola & mascara; //DETECTA PALA
                                    if(Comprobacio2 != 0){ 
                                        Sentido = 0;
                                        // Rand direccion

                                    }
                                }
                                if (FilaBola>1){
                                FilaBola-- ;
                                }
                            }
                            if(d==2){
                                if(FilaBola == 1){
                                    Comprobacio2 = (Bola >> 1) & mascara; //DETECTA PALA
                                    if(Comprobacio2 != 0){ 
                                        Sentido = 0;
                                        // Rand direccion

                                    }
                                }    
                                Bola = Bola >> 1 ;
                                FilaBola-- ;
                                if(Bola == 0x00000001){
                                    d=0;

                                }
                            }
                        }
                    }


                }
            }
            actualitza = 0;
        }
		Polsad = Polsador();		// Llegim els polsadors
		if (mirar == 1){
			if (Polsad == 1) {
                if (x>0 & x<=6) {
                    // Si s'ha premut el polsador 1
                    x = (x - 1);	// Incrementa x però la manté entre 0 i 7
                    mirar = 0;
                }
			}
			if (Polsad == 2) {
                if (x>=0 & x<6) {
                    // Si s'ha premut el polsador 1
                    x = (x + 1);	// Incrementa x però la manté entre 0 i 7
                    mirar = 0;
                }
			}
			if (Polsad == 3) {	// Si s'ha premut el polsador 3
						// Invertim el LED corresponent
				figura[y] = figura[y] ^ (1 << x);
				mirar = 0;
			}
			if (Polsad == 4) {	// Si s'ha premut el polsador 4
				if (x2>0 & x2<=6) {
                    // Si s'ha premut el polsador 1
                    x2 = (x2 - 1);	// Incrementa x però la manté entre 0 i 7
                    mirar = 0;		// No fem res
                }
			}
			if (Polsad == 5) {	// Si s'ha premut el polsador 5
				if (x2>=0 & x2<6) {
                    // Si s'ha premut el polsador 1
                    x2 = (x2 + 1);	// Incrementa x però la manté entre 0 i 7
                    mirar = 0;
                }
			}
		} else {
			if (Polsad == 0) {	// Si no s'ha premut cap polsador (o dos a la vegada)
				mirar = 1;
			}
		}
		// Anem a mostrar la figura actual a la matriu de LED
		for (unsigned char k = 0; k < 8; k++){
			
            FilaBola = FilaBola%8;
			Sortida[0] = 0;	// Vermells
			Sortida[2] = 0;	// Verds
			Sortida[4] = 0;	// Blaus
			// El cursor es mostra groc, independentment del punt
			if (y == k){		// Si estem a la filera del cursor
				mascara = (0b11000000 >> x);
				Sortida[0] = Sortida[0] | mascara;		// Encén vermell
				Sortida[2] = 0;		// Encén verd
				Sortida[4] = Sortida[4] | mascara;		// Apaga blau
			}
            if (y2 == k){		// Si estem a la filera del cursor
				mascara2 = (0b11000000 >> x2);
				Sortida[0] = 0;		// Encén vermell
				Sortida[2] = Sortida[2] | mascara2;		// Encén verd
				Sortida[4] = Sortida[4] | mascara2;		// Apaga blau
			}
            if (FilaBola == k){		// Si estem a la filera de la bola
                
                Sortida[0] = Sortida[0] | Bola;		// Encén vermell
                Sortida[2] = Sortida[2] | Bola;		// Encén verd
                Sortida[4] = 0;		// Apaga blau
            }
            
            
			Sortida[1] = k+1;	// Filera
			Sortida[3] = k+1;
			Sortida[5] = k+1;
			Envia3max(Sortida);	// Ho envia al MAX7221
			__delay_ms(1);
		}
        
        actualitza++;
	}
}
void __interrupt() temporit(void){
// void interrupt temporit(void) {		// Línia alternativa
	if (INTCONbits.T0IF) {			// Comprovem que hi ha interrupció per Timer 0
		TMR0 = 100;			// Preselecció de Timer0
		INTCONbits.T0IF = 0;		// Desactiva el bit que indica interrupció pel Timer0
		if (Actiu != 0) {		// Si la matriu no està apagada
			Actiu--;		// Passem a activar un altre color
			if (Actiu == 0) {	// Si hem arribat a zero
				Actiu = 3;	// Torna a posar el 3
			}
		}
						// D'entrada els desactivem els tres
		Sorti[0] = 0x00;		// Vermell
		Sorti[2] = 0x00;		// Verd
		Sorti[4] = 0x00;		// Blau
		if (Actiu == 1) {		// Si és vermell
			Sorti[0] = 0x01;	// Vermell activat
		}
		if (Actiu == 2) {		// Si és verd
			Sorti[2] = 0x01;	// Verd activat
		}
		if (Actiu == 3) {		// Si és blau
			Sorti[4] = 0x01;	// Blau activat
		}
		Sorti[1] = 0x0C;		// Shutdown mode
		Sorti[3] = 0x0C;		// Shutdown mode
		Sorti[5] = 0x0C;		// Shutdown mode
		Envia_max();		// Ho envia al MAX7221
	}
}
void Envia3max(char Valor[]) {			// Envia un joc de valors als tres MAX7221
	INTCONbits.T0IE = 0;			// Desactiva les interrupcions momentàniament
	char Port = 0;				// Variable on guardem l'estat del port B
	char Temp;				// Variable temporal
	for (signed char j = 5; j >= 0; j--){		// Hem d'enviar 6 bytes
		for (signed char k = 1; k < 9; k++){			// De 8 bits
			Temp = Valor[j] & 0b10000000;		// Agafa el bit de més a l'esquerra
								// Temp només podrà valer 0 o 128
			if (Temp == 0) {			// Si val 0
				Port = Port & 0b11101111;	// Desactiva Data (bit 4)
			} else {				// Si val 128
				Port = Port | 0b00010000;	// Activa Data (bit 4)
			}
			Valor[j] = Valor[j] << 1;		// Rodem els bits per situar el següent
			PORTB = Port;				// Ho posa al port B
			Port = Port | 0b00100000;		// Activa Clock (bit 5) i força lectura
			PORTB = Port;				// Ho posa al port B
			Port = Port & 0b11011111;		// Desactiva Clock (bit 5)
			PORTB = Port;				// Ho posa al port B
		}
	}
	Port = Port | 0b01000000;		// Activa Latch (bit 6) per copiar a les sortides
	PORTB = Port;				// Ho posa al port B
	INTCONbits.T0IE = 1;			// Reactiva les interrupcions a l'acabar
}
void Envia_max(void) {				// Envia un joc de valors als tres MAX7221
	asm("banksel _Port");
	asm("bcf (_Port&7fh),5");		// S'assegura que Clock està desactivat
	asm("bcf (_Port&7fh),6");		// S'assegura que Latch està desactivat
	asm("movf (_Port&7fh),w");		// Agafa el valor de Port
	asm("movwf PORTB");			// I el posa al port B
	asm("banksel _Compta");
	asm("movlw 48");			// Número de bits a enviar
	asm("movwf (_Compta&7fh)");		// Variable per comptar els bits
	asm("Bucle:");
	asm("banksel _Port");
	asm("bcf (_Port&7fh),4");		// Desactiva Data. Si toca activar-ho, ja ho farem
	asm("banksel _Sorti");
	asm("rlf (_Sorti&7fh),f");		// Fa sortir el bit de més a l'esquerra cap a C
	asm("rlf ((_Sorti+1)&7fh),f");		// i roda els altres a l'esquerra
	asm("rlf ((_Sorti+2)&7fh),f");
	asm("rlf ((_Sorti+3)&7fh),f");
	asm("rlf ((_Sorti+4)&7fh),f");
	asm("rlf ((_Sorti+5)&7fh),f");
	asm("banksel _Port");
	asm("btfsc STATUS,0");			// Mira si el bit de l'esquerra era un 1
	asm("bsf (_Port&7fh),4");		// Si era 1, activa Data
	asm("movf (_Port&7fh),w");		// Agafa el valor de Port. El valor que ha canviat és Data
	asm("movwf PORTB");			// I el posa al port B
	asm("bsf (_Port&7fh),5");		// Activa Clock, forçant a llegir el bit
	asm("movf (_Port&7fh),w");		// Agafa el valor de Port. El valor que ha canviat és Clock
	asm("movwf PORTB");			// I el posa al port B
	asm("bcf (_Port&7fh),5");		// Desactiva Clock
	asm("movf (_Port&7fh),w");		// Agafa el valor de Port. El valor que ha canviat és Clock
	asm("movwf PORTB");			// I el posa al port B
	asm("banksel _Compta");
	asm("decfsz (_Compta&7fh),f");		// Decrementa Compta
	asm("goto (Bucle&7ffh)");			// Si Compta no és zero, repeteix el bucle
	asm("banksel (_Port&7fh)");
	asm("bsf (_Port&7fh),6");		// Torna a activar Latch
						// Els valors es copiaran a la sortida del registre
	asm("movf (_Port&7fh),w");		// Agafa el valor de Port. El valor que ha canviat és Latch
	asm("movwf PORTB");			// I el posa al port B
}
void Ini3max(void) {				// Inicialitza els tres MAX7221
	char Bytes[6];				// Els sis bytes que cal enviar
	Bytes[0] = 0x00;			// Desactivat
	Bytes[1] = 0x0C;			// Shutdown mode
	Bytes[2] = 0x00;
	Bytes[3] = 0x0C;
	Bytes[4] = 0x00;
	Bytes[5] = 0x0C;
	Envia3max(Bytes);			// Els envia
	Bytes[0] = 0x00;			// No decode
	Bytes[1] = 0x09;			// Decode mode
	Bytes[2] = 0x00;
	Bytes[3] = 0x09;
	Bytes[4] = 0x00;
	Bytes[5] = 0x09;
	Envia3max(Bytes);			// Els envia
	Bytes[0] = 0x07;			// Vuit fileres
	Bytes[1] = 0x0B;			// Scan limit
	Bytes[2] = 0x07;
	Bytes[3] = 0x0B;
	Bytes[4] = 0x07;
	Bytes[5] = 0x0B;
	Envia3max(Bytes);			// Els envia
}
void Apaga(void) {				// Apaga tots els LED
	char Bytes[6];				// Els sis bytes que cal enviar
	for (unsigned char j = 1; j <= 8; j++){		// Hem d'enviar 8 fileres
		Bytes[1] = j;			// Filera
		Bytes[3] = j;
		Bytes[5] = j;
		Bytes[0] = 0x00;		// Vermells
		Bytes[2] = 0x00;		// Verds
		Bytes[4] = 0x00;		// Blaus
		Envia3max(Bytes);		// Els envia
	}
}
char Polsador(void) {
	char Pols = 0;
	ADCON0bits.GO = 1;		// Posa en marxa el conversor
	while (ADCON0bits.GO == 1)	// Mentre no acabi
		;    			// ens esperem
	if (ADRESH < 220 && ADRESH > 200) {
		Pols = 1;		// Comprova polsador 1
	}
	if (ADRESH < 194 && ADRESH > 174) {
		Pols = 2;		// Comprova polsador 2
	}
	if (ADRESH < 163 && ADRESH > 143) {
		Pols = 3;		// Comprova polsador 3
	}
	if (ADRESH < 90 && ADRESH > 70) {
		Pols = 4;		// Comprova polsador 4
	}
	if (ADRESH < 55 && ADRESH > 35) {
		Pols = 5;		// Comprova polsador 5
	}
	return Pols;
}
