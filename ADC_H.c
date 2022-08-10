/* 
 * File:   ADC_H.c
 * Author: Jose Pablo Petion
 *
 * Created on August 5, 2022, 9:07 AM
 */

#include "ADC_H.h"

void adc_init (uint8_t adc_cs, uint8_t vref_plus, uint8_t vref_minus){
    switch(adc_cs){
        case 1: //FOSC/2
            ADCON0bits.ADCS = 0b00; 
            
        case 2: //FOSC/8
            ADCON0bits.ADCS = 0b01; 
            
        case 3: //FOSC/32
            ADCON0bits.ADCS = 0b10; 
            
        case 4: //FRC
            ADCON0bits.ADCS = 0b11; 
            
        default: 
           ADCON0bits.ADCS = 0b11;
    }
    if (vref_plus>=0 && vref_plus<=1)
        ADCON1bits.VCFG1 = vref_plus;
    else 
        ADCON1bits.VCFG1 =0;
    
    if(vref_minus>=0 && vref_minus<=1)
        ADCON1bits.VCFG0 = vref_minus;
    else
       ADCON1bits.VCFG0 = 0;  
        
    }

void adc_start(uint8_t channel){
   
    if (channel<=0b0000 && channel<=0b1111)
        ADCON0bits.CHS = channel;    
    
        ADCON1bits.ADFM = 0;        // Justificado a la izquierda
        ADCON0bits.ADON = 1;        // Habilitamos modulo ADC
        while(1){
             if(ADCON0bits.GO == 0){
                 ADCON0bits.GO = 1;              // Iniciamos proceso de conversi n
             }
        }
        
}


    uint16_t adc_read (void)    {
        uint16_t inf;
        if(PIR1bits.ADIF)
        {
            inf = ADRESH << 2;
            inf = inf | ADRESL >> 6;
            PIR1bits.ADIF = 0;
        }
        return inf;
            
        }