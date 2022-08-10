/* 
 * File:   lcd_header.c
 * Author: Jose Pablo Petion
 *
 * Created on August 9, 2022, 5:54 PM
 */

#include <stdint.h>
#include "lcd_header.h"

void Lcd8_Port(char a)      
{                          
    PORTD = a;
}

void Lcd8_Cmd(char a)
{
    RS = 0;    
    PORTD = a;
    EN = 1;
    __delay_ms(4);
    EN = 0;
}

void Lcd8_Clear(void) { 
    Lcd8_Cmd(0);
    Lcd8_Cmd(1);
}

void Lcd8_Set_Cursor(char a, char b){ 
    char x;
    if (a == 1) {                                             
        x = 0x80 + b - 1;
        Lcd8_Cmd(x);
    }
    else if (a == 2) {         
        x = 0xC0 + b - 1;
        Lcd8_Cmd(x);
    }
}

void Lcd8_Init(void) {
    Lcd8_Cmd(0x00);    
    __delay_ms(20);
    Lcd8_Cmd(0x30);    
    __delay_ms(5);
    Lcd8_Cmd(0x30);
    __delay_ms(10);     
    Lcd8_Cmd(0x30);
    __delay_us(100);
    Lcd8_Cmd(0x38);
    Lcd8_Cmd(0x0C);
    Lcd8_Cmd(0x01);
    Lcd8_Cmd(0x06);
}

void Lcd8_Write_Char(char a) {
    RS = 1;
    PORTD = a;
    EN = 1;
    __delay_us(40);
    EN = 0;
}

void Lcd8_Write_String(char *a) {
    char i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd8_Write_Char(a[i]);
}

void Lcd8_Shift_Right(void) {
    Lcd8_Cmd(0x01);
    Lcd8_Cmd(0x0C);
}

void Lcd8_Shift_Left(void) {
    Lcd8_Cmd(0x01);
    Lcd8_Cmd(0x08);
}
