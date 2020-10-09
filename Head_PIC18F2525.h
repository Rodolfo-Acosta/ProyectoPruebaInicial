



void	ConfiguraMicroControlador(void)
{
	ClrWdt() ;
	INTCONbits.GIEH = 0 ;  	 // Desabilita interrupciones globales		
	INTCONbits.GIEL = 0 ; 	 // Desabilita interrupciones globales de baja prioridad


// ******************************************************************************
//						CONFIGURACION E INICIALIZACION
//				DEL MICROCONTROLADOR Y VARIABLES DEL PROCESO
// ******************************************************************************

// Inicializa Buffers

//	Delay10KTCYx(100) ;
	ClrWdt() ; 

// ***********************************************************

// ******************************************************************************
//						INICIALIZACION TIMER 0
// ******************************************************************************

	INTCON	= 0 ;		

	OpenTimer0 (TIMER_INT_ON & T0_SOURCE_INT & T0_16BIT & T0_PS_1_2) ;   //  & T0_PS_1_2

	kTMR0.R = kValTimer0 ;
	TMR0L = kTMR0.L   ;
	TMR0H = kTMR0.H   ;
	
	
// ******************  Configura Interrupcion TIMER 3  *****************************

	PIE2bits.TMR3IE = 0 ;  // Desabilita interrupción del TIMER 3
	IPR2bits.TMR3IP = 1 ;  // TMR3 alta prioridad =1,  baja prioridad = 0


// ******************************************************************************
//				INICIALIZACION INT0
// ******************************************************************************

	INTCONbits.INT0IF = 0 ;

	INTCON2bits.INTEDG0 = 0 ;		//  0 = Interrupt Flanco descendente

	INTCONbits.INT0IE = 0 ;			//  Desabilita Interrupcion INT0 (RB0)
	

// ******************************************************************************
//				INICIALIZACION PWM_1
// ******************************************************************************

//	TRISCbits.TRISC2 = 0 ;	//	Salida PWM1 activado

//	OpenTimer2( TIMER_INT_OFF & T2_PS_1_1 & T2_POST_1_1 ) ;

 /* **************** PWM Period; **************** */
 
	// void OpenPWM1(char period);

//	PeriodoSenal  = 249 ;  // 24940kHz (25uS)  @ Fosc = 40MHz  y TMR2 prescaler = 1 ; 
//	DutyCycle_1 = 0  ;      // 0% de Ciclo de Trabajo  

	
	// PWM_period =[(period ) + 1] x 4 x TOSC x TMR2_Prescaler
	
//	OpenPWM1(PeriodoSenal) ;  // 40 kHz  @ 40MHz
	
 /* **************** PWM Duty Cycle **************** */
 
	// PWMDutyCycle = (CCPRXL:CCPXCON<5:4>) x TOSC x (TMR2 Prescale Value)
	// void SetDCPWM2(unsigned int dutycycle);
	// PWM x Duty cycle = (DCx<9:0>) x TOSC
	// where DCx<9:0> is the 10-bit value specified in the call to this function.

//	SetDCPWM1(DutyCycle_1) ;  // 50% de Ciclo de Trabajo  

	// Resolution (bits) = log(FOSC/Fpwm) / log(2)

	// void SetOutputPWM2 (unsigned char outputconfig, unsigned char outputmode);
	// SetOutputPWM2 (SINGLE_OUT, PWM_MODE_1);
	


// ******************  CONFIGURA LAS INTERRUPCIONES   *****************************

	INTCONbits.TMR0IE = 1 ;  // Habilita interrupción del TIMER 0
	INTCON2 =0 ;
	INTCON2bits.TMR0IP = 1 ;  // TMR0 alta prioridad =1,  baja prioridad = 0

	INTCONbits.GIEH = 0 ;  	 // Desabilita interrupciones globales		
	INTCONbits.GIEL = 0 ; 	 // Desabilita interrupciones globales de baja prioridad

// ***********   CONFIGURACIÓN DEL CONVERTIDOR A/D  *******************************

	PIR1bits.ADIF = 0 ;
	IPR1bits.ADIP = 0 ;	
	PIE1bits.ADIE = 0 ;


	ADCON1 = 0b00001101 ;		//  RA0 y  RA1 en Analogico y el resto  son digitales
								//  VSS  y VDD como Vref.
	ADCON2 = 0b10101010 ;		//  ADCON2<7> = ADFM: A/D Result Format Select bit (1 = Right Justified 10 bits)
								//  20TAD, Fosc/64 32TOSC
	ADCON0 = 0b00000001 ;		//  Enciende el convertidor ADCON0.ADON = 1

	TRISAbits.TRISA0 = 1 ;	//		EnAn0
	TRISAbits.TRISA1 = 1 ;	//		EnAn1
	TRISAbits.TRISA2 = 1 ;	//		
	TRISAbits.TRISA3 = 1 ;	//		bMAX31856_!DRDY



	ClrWdt() ;	
// ***********   CONFIGURACIÓN DE LA INTERRUPCION DEL TRANSMISOR  *************************  

	RCONbits.IPEN = 1 ;       // Habilita prioridad de las interrupciones
	IPR1bits.TXIP = 0 ;		  // TX alta prioridad =1,  baja prioridad = 0
	PIE1bits.TXIE = 0 ;		 //  Desabilita la interrupción de la Transmision

// ***********   CONFIGURACIÓN DE LA INTERRUPCION DEL RECEPTOR   **************************  
	
	PIE1bits.RCIE = 0 ;		 //  Desabilita la interrupción de la Recepción.
	IPR1bits.RCIP = 0 ;		 //  Asignale  Interrupcion de baja prioridad
	
// ***********   CONFIGURACIÓN DE LA INTERRUPCION DEL SPI   **************************  
	
	PIE1bits.SSPIE	= 0 ; 		//  Desabilita la interrupción de la Recepción.						
	IPR1bits.SSPIP	= 0 ; 	 	//  Asignale  Interrupcion de baja prioridad	

// *****************************************************************************************
// CONFIGURA LOS PUERTOS

	TRISCbits.TRISC0 = 0 ;	//		I6
	TRISCbits.TRISC1 = 0 ;	//		I7
	TRISCbits.TRISC2 = 0 ;	//		Salida PWM1   SALIDA ANALOGICA SA0
	TRISCbits.TRISC3 = 0 ;	//		Q5
	TRISCbits.TRISC4 = 0 ;	//		Q6
	TRISCbits.TRISC5 = 0 ;	//		Q7
	TRISCbits.TRISC6 = 0 ;	//		TX
	TRISCbits.TRISC7 = 1 ;	//		RX	

	TRISBbits.TRISB0 = 1 ;	//		I5
	TRISBbits.TRISB1 = 0 ;	//		Q0
	TRISBbits.TRISB2 = 0 ;	//		Q1
	TRISBbits.TRISB3 = 0 ;	//		Q2
	TRISBbits.TRISB4 = 0 ;	//		Q3
	TRISBbits.TRISB5 = 0 ;	//		Q4
	TRISBbits.TRISB6 = 1 ;	//		PGC
	TRISBbits.TRISB7 = 1 ;	//		PGD
	
	TRISAbits.TRISA0 = 1 ;	//		AN0    	ENTRADA ANALOGICA    EA0
	TRISAbits.TRISA1 = 1 ;	//		I0		
	TRISAbits.TRISA2 = 1 ;	//		I1		
	TRISAbits.TRISA3 = 1 ;	//		I2
	TRISAbits.TRISA4 = 1 ;	//		I3
	TRISAbits.TRISA5 = 1 ;	//		I4

// ****************************************************************************************
// **********  Configuracion Sistema Roce2  18 Dic 2015   *********************************
// ****************************************************************************************

// *****************************************************************************************

// *****************************************************************************************
				
// *****************************************************************************************
// INICIALIZA Puertos

	PORTBbits.RB1 = 0;		// 	
	PORTBbits.RB2 = 0;		// 
	PORTBbits.RB3 = 0;		// 
    PORTBbits.RB4 = 0;		// 
	PORTBbits.RB5 = 0;		// 
	
	PORTCbits.RC0 = 1;		//  
	PORTCbits.RC1 = 1;		//  
	PORTCbits.RC2 = 1;	
// ****************************************************************************************

// ****************************************************************************************

// 	Inicializa Variables

	_Sistema.S0.bCiMaq = 0;
	_Sistema.S0.bCiPr0 = 1 ;
	_Sistema.S0.bCiPr1 = 0 ;

	_Sistema.rT10mS = 0 ;
	_Sistema.rT20mS = 0 ;
	_Sistema.rT50mS = 0 ;	
	_Sistema.rT100mS = 0 ;
	_Sistema.rT200mS = 0 ;
	_Sistema.rT250mS = 0 ;	
	_Sistema.rT1000mS = 0 ;
	
	_Sistema.kT10mS = 1 ; 
	_Sistema.kT20mS = 2 ; 	
	_Sistema.kT50mS = 5 ; 	
	_Sistema.kT100mS = 10 ;
	_Sistema.kT200mS = 20 ;
	_Sistema.kT250mS = 25 ;	
	_Sistema.kT1000mS = 100 ;

	_Sistema.S0.bT10mS  = 0 ;
	_Sistema.S0.bT50mS  = 0 ;
	_Sistema.S0.bT100mS  = 0 ;		
	_Sistema.S0.bT200mS  = 0 ;
	_Sistema.S0.bT250mS  = 0 ;
	_Sistema.S0.bT1000mS  = 0 ;
	_Sistema.LCD.bTempo = 0 ; 
	_Sistema.LCD_CntLin = 0 ;
	_Sistema.SPI.bErrorCom = 0 ;
	_Sistema.SPI.bTempo = 0 ;	
	_Sistema.CAD.bTempo = 1 ;
	


	rEnAna0 =	0 ; 
	rEnAna1 =	0 ; 
	rEnAna2 =	0 ; 
	rEnAna3 =	0 ; 

	
//  ***********************************************************

// ************************************************************

// -------------------------------------------------------------
// Desabilita interrupciones

	PIE1bits.TXIE = 0 ;
	PIE1bits.RCIE = 0 ;
	RCSTAbits.CREN  = 0 ;  			//  Deshabilita Recepcion
	
	_Sistema.CAD.bCan0 = 1 ;        // Activada la conversion 
	_Sistema.CAD.bIniCan0 = 0 ;	
	_Sistema.CAD.bCan1 = 0 ;
	_Sistema.CAD.bIniCan1 = 0 ;
	_Sistema.CAD.bCan2 = 0 ;
	_Sistema.CAD.bIniCan2 = 0 ;	

// ******************************************************************************
//			INICIALIZA VARIBLES DEL SISTEMA HORNO

	_Procesos.rCnt = 0 ;
	PIDbits.ON = 0 ;
	PIDbits.Modo_Loc_Rem = 1 ;  // Modo LOCAL
	PID_kp = 0 ; // 		 PID_kp_Local ;
	PID_ki = 0 ; // 		 PID_ki_Local ;
	PID_kd = 0 ; // 		 PID_kd_Local ;
	PID_Out= 0 ;
	PID_Out_10 = 0 ;
	PID_kp_Remoto = 0 ;
	PID_ki_Remoto= 0 ;
	PID_kd_Remoto = 0 ;
	PID_kp_Local = 0 ;
	PID_ki_Local= 0 ;
	PID_kd_Local = 0 ;	
	PID_Error  = 0 ;
	PID_ErrorAnterior = 0 ;
	_TimerPID.Bits.bQ = 0 ;	
	PID_ErrorAcc = 0 ;
	PIDbits.SensorTemp_Loc_Rem = 1 ;  // Selecciona Sensor Local 


	CQD.CanCiclosActivo = 0 ;
	CQD.CanCiclosActivo_RemotoAutomatico = 0 ;
	CQD.CanCiclosActivo_RemotoManual = 0 ;
	CQD.CanCiclosActivo_LocalAutomatico = 0 ;
 	CQD.CanCiclosMax = 50 ;  
	CQD.PeriodoNroCiclos = CQD.CanCiclosMax ;
	
	TempHorno = 0  ;
	TempHorno_Local = 0  ; 	
	TempHorno_Remoto = 0  ; 		
	TempRefHorno = 0 ;
	TempRefHorno_Local = 0 ;
	TempRefHorno_Remoto = 0 ;
    PotenciaMaxima_Remoto = 100  ; //  Potencia Maxima del Horno 100%
	PotenciaRef_Remoto = 10  ;  //  Potencia Referencia Manual Remoto del Horno  10%

    // Dado que no se ha implementado el modo de asignar valores locales
    // Temporalmente estos se colocan a  0  
    PID_kp_Local = 0 ; 
    PID_ki_Local = 0 ;
    PID_kd_Local = 0 ; 
    TempRefHorno_Local  = 0 ; 	

	PCDato_0 = 0 ;
	PCDato_1 = 0 ;
	PCDato_2 = 0 ;
	PCDato_3 = 0 ;
	PCDato_4 = 0 ;
	PCDato_5 = 0 ;
	PCDato_6 = 0 ;
	PCDato_7 = 0 ;
	

// ******************************************************************************

//				HABILITA LAS INTERRUPCIONES GLOBALES	
		
	INTCONbits.GIEL = 1 ; 	 // Habilita interrupciones globales de baja prioridad
	INTCONbits.GIEH = 1 ;  	 // Habilita interrupciones globales
	ClrWdt() ; 
}
