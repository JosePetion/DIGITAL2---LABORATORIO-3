#ifndef lcd_header
#define	lcd_header

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#ifndef RS
#define RS RC6
#endif

#ifndef EN
#define EN RC7
#endif

#include <xc.h> 
//LCD Functions Developed by electroSome

void Lcd8_Port(char a);

void Lcd8_Cmd(char a);

void Lcd8_Clear(void);

void Lcd8_Set_Cursor(char a, char b);

void Lcd8_Init(void);

void Lcd8_Write_Char(char a);

void Lcd8_Write_String(char *a);

void Lcd8_Shift_Right(void);

void Lcd8_Shift_Left(void);

#endif	/* LCD_H */