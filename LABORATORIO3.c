/* 
 * File:   LABORATORIO3.c
 * Author: Jose Pablo Petion
 *                                  ~~Maestro~~
 * Created on August 4, 2022, 6:09 PM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h> 
#include <stdio.h>
#include "header_spi.h"
#include "ADC_H.h"
#include "lcd_header.h"
#include "header_timer0.h"

#define _XTAL_FREQ 1000000

#define RS RC6
#define EN RC7
#define IN_MIN 0
#define IN_MAX 1025 
#define OUT_MIN 0
#define OUT_MAX 500


/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/

uint16_t ADRESL = 0;
uint16_t ADRESL1 = 0;
uint8_t i = 0;
uint8_t valor = 0;
uint8_t valor2 = 0;
uint8_t unidad;
uint8_t decena;
uint8_t unidad2;
uint8_t decena2;
char VALORES1[10];
char VALORES2[10];

/*------------------------------------------------------------------------------
 * INTERRUPCIONES 
 ------------------------------------------------------------------------------*/
void __interrupt() isr (void){
    if(INTCONbits.T0IF){
        ADREST1++;          
        if (ADREST1 == 10){
            i = 1;  
            ADREST = 0;                
        }
        tmr0_reload();
    }
    return;
}

/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void setup(void);
unsigned short map(uint16_t  val, uint8_t  in_min, uint16_t in_max, 
            unsigned short out_min, unsigned short out_max);

/*------------------------------------------------------------------------------
 * MAIN LOOP
 ------------------------------------------------------------------------------*/
void main(void) {

    setup();
    tmr0_init(255);
    tmr0_reload();

    Lcd8_Init();
    while(1){
        if (MOSTRAR){
            //secuecnia de inciio de envio de datos
            PORTEbits.RE0 = 0;   
            __delay_ms(1); 
            spiWrite(72);   
        

            PORTEbits.RE0 = 0;          
            spiWrite(76);         
            ADRESH1 = spiRead(); 
            __delay_ms(1); 
            PORTEbits.RE0 = 1;  
            __delay_ms(100);

            PORTEbits.RE0 = 0; 
            spiWrite(65);      
            ADRESL1 = spiRead();    
            __delay_ms(1); 
            PORTEbits.RE0 = 1;  


           // trasnformacion de valor de salida
            
            Valor = (uint16_t)(map(ADREST1, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX));
            unidad = (MAP1/100);
            decena = (MAP1 - unidad*100);
            sprintf(valor, "%d.%d V", unidad, decena);

            MAP2 = (uint16_t)(map(ADREST2, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX));
            unidad2 = (MAP2/100);
            decena2 = (MAP2 - UNI2*100);
            sprintf(valor, "%d.%d V", UNI2, decena);
            MOSTRAR = 0;
            
            //----------------------------------------------
        }
        
        //DESPLIEGUE EN LCD
        Lcd8_Set_Cursor(1,1);
        Lcd8_Write_String("Pot 1:   Pot 2:");
        __delay_ms(100);
        Lcd8_Set_Cursor(2,1);
        Lcd8_Write_String(valor);
        Lcd8_Set_Cursor(2,10);
        Lcd8_Write_String(valor2);
        __delay_ms(100);
    }
    return;
}

/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/
void setup(void){       
    ANSEL = 0;     
    ANSELH = 0;   
    TRISD = 0;   
    TRISEbits.TRISE0 = 0;
    PORTD = 0;   
    PORTE = 0b1;     
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0; 
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    //interrupciones
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1; 
    return;
}

unsigned short map(uint16_t x, uint8_t x0, uint16_t x1, 
            unsigned short y0, unsigned short y1){
    return (unsigned short)(y0+((float)(y1-y0)/(x1-x0))*(x-x0));
}