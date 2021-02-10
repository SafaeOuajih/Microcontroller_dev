/*
 * File:   lib_LCD.h
 * Author: souajih
 *
 * Created on 29 avril 2019, 15:23
 */


#include <xc.h>
#define _XTAL_FREQ 4000000





# ifndef LIBLCD_H
# define LIBLCD_H 
void lcd_init();
void lcd_busy();
void lcd_write_instr(unsigned char c);
void lcd_putch(unsigned char c);
void lcd_puts(const unsigned char *s);
void lcd_pos(unsigned char ligne,unsigned char pos);
void lcd_clear();
void lcd_home();
#endif












