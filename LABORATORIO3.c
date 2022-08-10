/* 
 * File:   LABORATORIO3.c
 * Author: Jose Pablo Petion
 *                                  ~~Maestro~~
 * Created on August 4, 2022, 6:09 PM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT    // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF               // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF              // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF              // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF                 // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF                // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF              // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF               // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF              // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF                // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V           // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF                // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include "header_timer0.h"
#include "header_spi.h"
#include "ADC_H.h"
#define _XTAL_FREQ 125000

/*------------------------------------------------------------------------------
 * variables 
 ------------------------------------------------------------------------------*/
uint8_t contador;
uint8_t receptor;
/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void setup(void);
/*------------------------------------------------------------------------------
 * INTERRUPCIONES 
 ------------------------------------------------------------------------------*/
void __interrupt() isr (void){
    
    if(PIR1bits.ADIF){
        PORTD = adc_read();
        PIR1bits.ADIF = 0;
    }
    if (PIR1bits.SSPIF){
        contador = spiRead();
        spiWrite(contador);
        PIR1bits.SSPIF = 0;
        
        
    }
}
/*------------------------------------------------------------------------------
 * CICLO PRINCIPAL
 ------------------------------------------------------------------------------*/
void main(void) {
    setup();
    while(1){

    }
}
/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/
void setup(void){
    ANSEL=0B11;
    ANSELH=0;
    TRISA=0B11;
    TRISB=0;
    TRISD=0;
    PORTA=0;
    PORTB=0;
    PORTD=0;
    //RELOJ INTERNO
    OSCCONbits.IRCF = 0b001;    // 125kHz
    OSCCONbits.SCS = 1;         // Reloj interno
    //SPI
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    //SSPCONbits.SSPEN =       1;
    
    //ADC
    adc_init(1, 0, 0);
    adc_start(0);
    //INTERRUPCIONES GLOBALES
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}
/*
 * 
 */

