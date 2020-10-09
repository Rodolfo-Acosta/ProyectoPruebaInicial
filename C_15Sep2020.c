/*
;***************************************************************************************

Autor: Rodolfo Acosta

;***************************************************************************************


;				Version: Diciembre 2015 16H47
				
;				Sistema Roceli  con SXCOM 2v0
                Mide Temperatura con Termocupla K y el MAX6675    
;				AN0 mide el Valor de Potenciometro
;				
;				Muestra la Temperatura en lado Derecho del LCD

// *************************************************************************************

Notas 26 marzo 
			Para el Horno trabajando con computador de Pentium III  con 256 MB
			se hicerorn las siguiente modificaciones:
			
             se cambio TimeOut RX  de 10ms  a 150 ms (kTempoRxTimeOut = 15 ;  //  x * 10mS  = 50ms)
             Velocidad de 19200 bps
              
;***************************************************************************************

 * REVISION HISTORICA
 * INICIO: 10/12/2019
 * De esto:
 *  			 	if(PIDbits.SensorTemp_Rem_Loc) TempHorno =  TempHorno_Remoto ;    // Selecciona Sensor de Temepratura Remoto	 
					else TempHorno =  TempHorno_Local ;// Selecciona Sensor de Temperatura Local PT100
		
 * A esto:
 * 1ro. 
 *                  if(PIDbits.SensorTemp_Rem_Loc) TempHorno =  TempHorno__Local ;    // Selecciona Sensor de Temepratura Remoto	 
					else TempHorno =  TempHorno_Local ;// Selecciona Sensor de Temperatura Local PT100

 * 2do.
 *      El CQD ( 0 - 50)	 se cambio por Potencia ( 0 - 100%)	
 * 3ro.
 *      El mensaje de Bienvenida en LCD muestra Ultima Modificacion: 10/12/2019
 * 
 * 
 * 
// Modificacion en el archivo LKR hecha el 12Dic2019 para ampliar el Buffer de Comunicacion
 * 
 * 
 * Cumana 13Dic2019
 *  Se agrego la Funciones de medida de Peso con el HX711
 *  
 * Cumana 14Dic2019
 *  Se Cambio CheckSUM por CRC16  en el modulo de comunicacion en SXCOM 2v0
 *  
*/


// ****************************************************************************************
// Prototipos de Funciones
// void funcion_MCC(unsigned char MCC_CanCiclos) ;

#define _DEBUG_sutronix  	// Entra en modo Debuger para ICD2
							//  Desabilita WDO, BOREN, LVP, CP0

#define p18f2525


#include <p18f2525.h>       
#include <stdlib.h>
#include <stdio.h>
#include <usart.h>
#include <delays.h>
#include <timers.h>
#include <spi.h>
#include <adc.h>
#include <math.h>
#include <pwm.h>


#include  "SXConfig18F2525_Debuger_15Sep2020.h"
#include  "CnTempLite3LED_15Sep2020_Definiciones.h"
#include  "CnTempLite3LED_15Sep2020_SerIntTmrComADC.h" 
#include  "CnTempLite3LED_15Sep2020_Funciones.h" 
#include  "CnTempLite3LED_15Sep2020_ConfigMicroPIC18F2525.h" 
#include  "CnTempLite3LED_15Sep2020_SFC.h"




unsigned char  	ReGen1, ReGen2, ReGen3 ;
float TempUnionFria, TemperaturaLineal ;
char cTempUnionFria ;
char  cReGen1, cReGen2 ;
int iReGen1, iReGen2, iReGen3;   
unsigned int uiReGen1, uiReGen2, uiReGen3; 	
unsigned short long uslReGen1, uslReGen2 ;
unsigned  long ulReGen1 ;
   
void main (void)
{
	unsigned char car, z, Periodo;
    char zi = '2'; 
	unsigned int i, j, x, y, y1, y2, TiempoL, TiempoH     ;
    short VarTempTabla, TempBin, CanElemTabla ;
    unsigned int d0 , d1, d2, d3, f0, f1, f2  ; 
    float Fx, Fy;
// *****************************************************************************

// *****************************************************************************
//				CONFIGURACION E INICIALIZACION 	DEL MICROCONTROLADOR					
// *****************************************************************************

	ConfiguraMicroControlador() ;
    
    
// *****************************************************************************************
//          Prueba Inicales
//          CONFIGURA LOS PUERTOS PARA PRUEBAS
/*
    PORTCbits.RC0 = 1;  
    PORTBbits.RB2 = 0;  
    PORTBbits.RB3 = 0;  
    PORTBbits.RB1 = 0; 
 	while (1){
  
        for (i=0; i < 2; i++) Delay10KTCYx (100); // i< 40
 
 bkp= 0 ;      
        PORTCbits.RC0 = ~PORTCbits.RC0;  
 //      PORTBbits.RB2 = ~PORTBbits.RB2;  // Rojo 
       PORTBbits.RB3 = ~PORTBbits.RB3;  // Verde

       Delay10KTCYx (rEnAna1) ;
        
 //       PORTBbits.RB1 = ~PORTBbits.RB1;  // Amarillo    
        
        for (i=0; i < 2; i++) Delay10KTCYx (100); // i< 40

  bkp= 0 ;       
     PORTBbits.RB3 = ~PORTBbits.RB3;   // Verde
 //    PORTBbits.RB2 = ~PORTBbits.RB2;      // Rojo    
       PORTCbits.RC0 = ~PORTCbits.RC0;  
        
         
     Delay10KTCYx (rEnAna1) ;
                
   //     PORTBbits.RB1 = ~PORTBbits.RB1;  //Amarillo      
   
    }    
    
    
*/ 
    
// *****************************************************************************************   
//  -----------------------------------
//  SFC

	SFC.rTmr1 = 0 ;
	SFC.kTmr1 = 1 ;  // 10 mS 	
	SFC.rTmr2 = 0 ;
	SFC.kTmr2 = 2 ;  // 20 mS 
	SFC.rTmr3 = 0 ;
	SFC.kTmr3 = 3 ;  // 30 mS 
			
	SFC.Rama.reg = 0 ; 
	SFC.TS.reg  = 0 ; 
	SFC.TS.b.S5 = 1 ;
	SFC.TS.b.T0 = 1 ; // Etapa de Inicilizacion de Variables SFC del Proceso 
	SFC.TS.b.S0 = 0 ;  		
	SFC.Rama.b.Inicializacion = 0 ;

    
/******************************************************************************
 *                  Inicializacion de los TON   Temporizador al Encendido
 *****************************************************************************/    
    _Sistema.S0.bCiTON = 0 ; 
    for(i=0 ; i<5 ;i++){
        TON[i].Bits.REG = 0 ; 
        TON[i].VAL = 0 ; 
        TON[i].VALPRE = 0 ;
    }
    
/******************************************************************************
 *                  Inicializacion de los TP Temporizador de Pulso
 *****************************************************************************/        
    _Sistema.S0.bCiTP = 0 ; 
    for(i=0 ; i<5 ;i++){
        TP[i].Bits.REG = 0 ; 
        TP[i].VAL = 0 ; 
        TP[i].VALPRE = 0 ;
    }   
     
/******************************************************************************
 *                  Inicializacion de los CTU   Contador Ascendente
 *****************************************************************************/         
    _Sistema.S0.bCiCTU = 0 ; 
    for(i=0 ; i<5 ;i++){
        CTU[i].Bits.REG = 0 ; 
        CTU[i].VAL = 0 ; 
        CTU[i].VALPRE = 0 ;
    }      
/******************************************************************************
 *                  Inicializacion de los PG   Generador de Pulsos
 *****************************************************************************/      
     
     
 PG[0].VAL_PRE_HIGH = 2 ;
 PG[0].VAL_PRE_LOW = 2 ;
 PG[0].VAL = 0 ;   
 PG[0].Bits.REG = 0 ;   
 
 PG[0].Bits.EN = TRUE ;       
     
 PG[1].VAL_PRE_HIGH = 2 ;
 PG[1].VAL_PRE_LOW = 2 ;
 PG[1].VAL = 0 ;   
 PG[1].Bits.REG = 0 ;   
 
/* *****************************************************************************
 *                  INICIALIZACION  PID y CQD
 * *****************************************************************************/
    
         _Sistema.S0.bON = 0 ;  
 
            IndiTempLEDS_LowTemp  =     FALSE ;
            IndiTempLEDS_NormalTemp =  FALSE ; //            
            IndiTempLEDS_HighTemp =    FALSE ;       
            
             for (i=0; i < 5; i++) Delay10KTCYx (45); // i< 40  
         
         
           // APAGA  el PID    
            PIDbits.ON = 0 ;            // PID siempre encendido
            _TimerPID.Bits.bEn = 0 ;
             PIDbits.Modo_Loc_Rem = 1 ;  // Modo Local
            TempRefHorno_Local = 0  ;  // Temperatura a Controlar en Modo Local
            PID_kp_Local = 0.0 ;
            PID_ki_Local = 0.0  ;
            PID_kd_Local = 0.0  ;

            PID_kErrorLimiteAlto = 400.0 ;
            PID_kErrorLimiteBajo = -400.0 ;
            PID_Limite = 1000.0 ;

            PID_Integral  = 0 ;
            PID_ki = 0 ; 
            PID_ErrorAcc = 0 ;

            PID_Proporcional  = 0 ;
            PID_Integral  = 0 ;
            PID_Derivada  = 0 ;

            PID_Integral  = 0 ;
            PID_ki  = 0 ;
            PID_ErrorAcc  = 0 ;

            PID_Proporcional  = 0 ;
            PID_kp  = 0 ;
            PID_Error  = 0 ; 
            PID_ErrorDif  = 0 ;
            PID_Error  = 0 ;
            PID_ErrorAnterior  = 0 ;
            PID_Derivada  = 0 ;
            PID_kd  = 0 ;
            PID_ErrorDif  = 0 ; 

            PID_Out  = 0 ; 

            PID_Out_10 = 0.0  ;
            PID_Out = 0.0  ;

            // Bits de Control REMOTO del  Encendido / Apagado del PID y CQD

            rSalBin0.Q4 = 1 ;	// 	Sensor temperatura Local Remoto (PIDbits.SensorTemp_Loc_Rem )
            rSalBin0.Q6 = 1 ;	//	PID y CQD (Selecciona Modo LOCAL / REMOTO)	
            rSalBin0.Q5 = 1 ;	//	Encender PID y Driver CQD
            rSalBin0.Q7 = 1 ;	// 	PID y CQD (Selecciona Modo AUTOMATICO / MANUAL)

            
        // APAGA  el CQD    
            CQDbits.ON = 0 ;
            CQDbits.Q = 0 ;
            CQD.CanCiclosActivo = 0 ; 
            CQDbits.Modo_Aut_Man = 1 ;  // Modo Autoamtico
            PotenciaRef_Remoto = 0 ;
            PotenciaMaxima_Remoto = 0 ;    
            PotenciaRef_Local = 0 ;
            PotenciaMaxima_Local = 0 ; 
            INTCONbits.INT0IF = 0 ;
			INTCONbits.INT0IE = 0 ;			//  Deshabilita Interrupcion INT0 (RB0)
		 	CQDbits.Inicio = 0 ;     
            CQD.ContadorCiclos = 0 ;
			CQD.CanCiclosMin = 0 ;
			CQD.CanCiclosMax = CQD.PeriodoNroCiclos ;      // Periodo 25 ciclos
            CQD.CanCiclosActivo_LocalAutomatico = 0;
            CQDbits.Modo_Loc_Rem = 1 ; //  Modo LOCAL
//            SalBin0 = 0 ;
            
         // APAGA  EL TEMPORIZADOR DEL SISTEMA  
            
    //  Inicializacion de medida de temperatura con NTC        
           ResisNTC = 0 ;
           Vbin = 0 ;       // Valor binario del CAD
           Rpu = 4700 ;     // Resistencia de Pul-Up
           
/* -----------------------------------------------------------------------------
 *                          Mensaje de Bienvenida 
 *                           Indicador 3 LEDs 
 * ---------------------------------------------------------------------------- */

        
           for (y=0; y < 5; y++){
            
    
            IndiTempLEDS_LowTemp  =     TRUE ;
            IndiTempLEDS_NormalTemp =  FALSE ; //            
            IndiTempLEDS_HighTemp =    FALSE ;      
            
            for (i=0; i < 5; i++) Delay10KTCYx (45); // i< 40  
            
            IndiTempLEDS_LowTemp  =     TRUE ;
            IndiTempLEDS_NormalTemp =  TRUE ; //            
            IndiTempLEDS_HighTemp =    FALSE ;
            
            for (i=0; i < 5; i++) Delay10KTCYx (45); // i< 40  
            
            IndiTempLEDS_LowTemp  =     TRUE ;
            IndiTempLEDS_NormalTemp =  TRUE ; //            
            IndiTempLEDS_HighTemp =    TRUE ;
            
            for (i=0; i < 5; i++) Delay10KTCYx (45); // i< 40  
            
            IndiTempLEDS_LowTemp  =     FALSE ;
            IndiTempLEDS_NormalTemp =  FALSE ; //            
            IndiTempLEDS_HighTemp =    FALSE ;       
            
             for (i=0; i < 5; i++) Delay10KTCYx (45); // i< 40  
            
        }
                       
        IndiTempLEDS_NormalTemp =  TRUE ; //   

// ***************************************************************
//          
//          Funcion Indicador de Temperatura de 3 LEDS
//
// ***************************************************************    
 
    // Inicializacion
        
        IndiTempLEDS.Error = 0 ;
        IndiTempLEDS.Bits.Q = 0 ;    
        IndiTempLEDS.Bits.EN = 0 ;  
        IndiTempLEDS_HighTemp = 0 ;  
        IndiTempLEDS_NormalTemp = 0 ;  
        IndiTempLEDS_LowTemp = 0 ;  
        IndiTempLEDS_LowTemp = FALSE ;  
  
        TempRefHorno_Local = 85 ;

        IndiTempLEDS.Bits.RegBits = 0;
//         PG[0].Bits.Q = 1;
           
        PG[0].Bits.EN = TRUE ;

/* -----------------------------------------------------------------------------
 *                          Mensaje de Bienvenida 
 *                           Indicador 3 LEDs 
 * ---------------------------------------------------------------------------- */
        for (y=0; y < 5; y++){
            
    
            IndiTempLEDS_LowTemp =     TRUE ;
            IndiTempLEDS_NormalTemp =  FALSE ; //            
            IndiTempLEDS_HighTemp =    FALSE ;      
            
            for (i=0; i < 5; i++) Delay10KTCYx (45); // i< 40  
            
            IndiTempLEDS_LowTemp =     TRUE ;
            IndiTempLEDS_NormalTemp =  TRUE ; //            
            IndiTempLEDS_HighTemp =    FALSE ;
            
            for (i=0; i < 5; i++) Delay10KTCYx (45); // i< 40  
            
            IndiTempLEDS_LowTemp =     TRUE ;
            IndiTempLEDS_NormalTemp =  TRUE ; //            
            IndiTempLEDS_HighTemp =    TRUE ;
            
            for (i=0; i < 5; i++) Delay10KTCYx (45); // i< 40  
            
            IndiTempLEDS_LowTemp =     FALSE ;
            IndiTempLEDS_NormalTemp =  FALSE ; //            
            IndiTempLEDS_HighTemp =    FALSE ;       
            
             for (i=0; i < 5; i++) Delay10KTCYx (45); // i< 40  
            
        }
                       
        
/* *****************************************************************************
 *                  INICI0 EJECUCION DE SISTEMA 
 *                      Indicador 3 LEDs 
 * *****************************************************************************/
	while (1){
        
		ClrWdt() ; 
        TempHorno_Local = rEnAna1 / 8 ;
         
         
        IndiTempLEDS.Error = fabs(TempHorno_Local - (float)TempRefHorno_Local) ; 

        if(TempHorno_Local >= TempRefHorno_Local)   IndiTempLEDS.Bits.SIGNO =  TRUE ;
        else IndiTempLEDS.Bits.SIGNO = FALSE ;    
     
bkp = 0 ;           
        if(IndiTempLEDS.Error <= 5){
            IndiTempLEDS_NormalTemp =  TRUE ; //
            IndiTempLEDS_LowTemp =     FALSE ;
            IndiTempLEDS_HighTemp =    FALSE ;
        }    
        else if(IndiTempLEDS.Error <= 10){
            IndiTempLEDS_NormalTemp =  PG[0].Bits.Q ;
            IndiTempLEDS_LowTemp =     FALSE ;
            IndiTempLEDS_HighTemp =    FALSE ;            
        }
        else if(IndiTempLEDS.Error <= 15){
            if(IndiTempLEDS.Bits.SIGNO){
                IndiTempLEDS_HighTemp =  PG[0].Bits.Q ;    
                IndiTempLEDS_LowTemp =   FALSE ;
                IndiTempLEDS_NormalTemp =  FALSE ;   
            } 
            else{
                
                IndiTempLEDS_LowTemp =     PG[0].Bits.Q ;
                IndiTempLEDS_HighTemp =    FALSE  ; 
                IndiTempLEDS_NormalTemp =  FALSE ;
            }    
        }    
        else{
            if(IndiTempLEDS.Bits.SIGNO){
                 IndiTempLEDS_HighTemp =   TRUE  ;     ;
                IndiTempLEDS_LowTemp =     FALSE  ; 
                IndiTempLEDS_NormalTemp =  FALSE ;
            }
               
            else{
                IndiTempLEDS_LowTemp =     TRUE  ; 
                IndiTempLEDS_HighTemp =    FALSE  ;    
                IndiTempLEDS_NormalTemp =  FALSE ; 
            }    
        }
        

   

bkp = 0 ;      
       // *************************************************************** 
       //          Funcion Generador de Pulsos (PG)
       // ***************************************************************            
       // Actualiza PG  cada 100 mS
        
        if (_Sistema.S0.bCiPG ){
            _Sistema.S0.bCiPG = 0 ;
            
            for(i=0 ; i<2 ;i++){
                if (PG[i].Bits.EN) {    
                    if (PG[i].Bits.Q){
                        if (PG[i].VAL == PG[i].VAL_PRE_HIGH){
                            PG[i].Bits.Q = 0 ;  
                            PG[i].VAL = 0 ;
                        } else PG[i].VAL++ ;      
                    }
                    else{
                        if (PG[i].VAL == PG[i].VAL_PRE_LOW){
                            PG[i].Bits.Q = 1 ;
                            PG[i].VAL = 0 ;
                        } else PG[i].VAL++ ;  
                    }
                }
                else{
                    PG[i].Bits.Q = 0 ; 
                    PG[i].VAL = 0 ;
                } 
            }
        }
 /*
        // **************************************************************
        //          Mando para Apagar el Sistema
        // **************************************************************
 
    if (!EnBin0){    // Mando de Parada de Sistema
            
        // APAGA  el PID    
            _Sistema.S0.bON = 0 ;       // bit de Estado del Sistema APAGADO 
            PIDbits.ON = 0 ;            // PID siempre encendido
            _TimerPID.Bits.bEn = 0 ;
 
            PIDbits.Modo_Loc_Rem = 1 ;  // Modo LOCAL
            TempRefHorno_Local = 0  ;  // Temperatura a Controlar en Modo Local
            PID_kp_Local = 0.0 ;
            PID_ki_Local = 0.0  ;
            PID_kd_Local = 0.0  ;
               
            PID_Out_10 = 0.0  ;
            PID_Out = 0.0  ;
            
        // APAGA  el CQD    

            CQDbits.ON = 0 ;
            CQDbits.Q = 0 ;  
            CQDbits.Modo_Aut_Man = 1 ;  // Modo Autoamtico            
            CQD.CanCiclosActivo = 0 ;
            
            PotenciaRef_Remoto = 0 ;
            PotenciaMaxima_Remoto = 0 ;    
            PotenciaRef_Local = 0 ;
            PotenciaMaxima_Local = 0 ; 
            
            INTCONbits.INT0IF = 0 ;
			INTCONbits.INT0IE = 0 ;			//  Deshabilita Interrupcion INT0 (RB0)
		 	CQDbits.Inicio = 0 ;     
            CQD.ContadorCiclos = 0 ;
			CQD.CanCiclosMin = 0 ;
			CQD.CanCiclosMax = CQD.PeriodoNroCiclos ;      // Periodo 25 ciclos
            CQD.CanCiclosActivo_LocalAutomatico = 0;
            CQDbits.Modo_Loc_Rem = 1 ; //  Modo LOCAL            
 
            SalBin0 = 0 ;
         // APAGA  EL TEMPORIZADOR DEL SISTEMA  
            
        }    
        
        // ******************************************************
        //          Mando para Arranque de Sistema
        // ******************************************************
 
        if ((!EnBin1) & (!_Sistema.S0.bON)){ 
            
        _Sistema.S0.bON = 1 ;       // bit de Estado del Sistema ENCENDIDO
        
        // Bits de Control REMOTO del  Encendido / Apagado del PID y CQD
        rSalBin0.Q4 = 1 ;	// 	Sensor temperatura Local Remoto (PIDbits.SensorTemp_Loc_Rem )
        rSalBin0.Q6 = 1 ;	//	PID y CQD (Selecciona Modo LOCAL / REMOTO)	
        rSalBin0.Q5 = 1 ;	//	Encender PID y Driver CQD
        rSalBin0.Q7 = 1 ;	// 	PID y CQD (Selecciona Modo AUTOMATICO / MANUAL) 
        
    // Inicializa y Arranca el PID

        PIDbits.ON = 1 ;            // PID siempre encendido
        _TimerPID.Bits.bEn = 1 ;
        PIDbits.Modo_Aut_Man = 1 ;  // Modo Autoamtico
        PIDbits.Modo_Loc_Rem = 1 ;  // Modo LOCAL
        // ******************************************************
        // ******************************************************
        //                Coeficientes del PID
        //                  Para Hervir agua.
        // ------------------------------------------------------ 
        TempRefHorno_Local = 260  ;  // Temperatura a Controlar

        PID_kp_Local = 20.0 ;     // 25.0   35 40 42
        PID_ki_Local = 0.50  ;    // 1.20  1.5  1.56
        PID_kd_Local = 100.0  ;     // 200

        // ******************************************************
        // ******************************************************
        //            Potencia Maxima Disponible (%)      
        //               Para Hervir agua
        // ------------------------------------------------------   
        PotenciaRef_Remoto = 70 ;
        PotenciaMaxima_Remoto = 80 ;    
        PotenciaRef_Local = 60 ;
        PotenciaMaxima_Local = 75 ;  // (75%)

        // ******************************************************

        // ******************************************************
        PID_kErrorLimiteAlto = 400.0 ;
        PID_kErrorLimiteBajo = -400.0 ;
        PID_Limite = 1000.0 ;            

        PID_Out_10 = 0.0  ;
        PID_Out = 0.0  ;

    // Inicializa y Arranca el CQD

        CQDbits.ON = 1 ; 		// Siempre Encendido
        CQDbits.Modo_Loc_Rem = 1 ; 		// Siempre En Modo Local     
        CQDbits.Modo_Aut_Man = 1 ;  // Modo Autoamtico
        CQDbits.Q = 0 ;
        CQD.CanCiclosActivo = 0 ;

        INTCONbits.INT0IF = 0 ;
        INTCONbits.INT0IE = 0 ;			//  Deshabilita Interrupcion INT0 (RB0)
        CQDbits.Inicio = 0 ;     
        CQD.ContadorCiclos = 0 ;
        CQD.CanCiclosMin = 0 ;
        CQD.CanCiclosMax = CQD.PeriodoNroCiclos ;      // Periodo 25 ciclos
        CQD.CanCiclosActivo_LocalAutomatico = 0;
        CQDbits.Modo_Loc_Rem = 1 ; //  Modo LOCAL


        bkp =0;
        TempAnterior = 0 ;
        FallaDeSistema = 0 ; 
        SalBin0 = 0 ;
            
        // ENCIENDE  EL TEMPORIZADOR DEL SISTEMA  

        } 
        
// rEnAna1 = 562 ;
// rEnAna0 = 398 ; 
        SFC_funcion() ;

// -------------------  Evalua el Proceso Continuo de Comunicacion -----------------------
*/

	}  		// fin de while
	
 			// Fin de main
}


