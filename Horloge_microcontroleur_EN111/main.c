/*
 * File:   newmain.c
 * Author: souajih
 *
 * Created on 21 mai 2019, 15:04
 */


#include <xc.h>
#define _XTAL_FREQ 4000000

#include "lib_LCD.h"
#include "horloge.h"



void main(void) {
   TRISB = 0x00;
    
    init();
    

    lcd_init();
   //lcd_puts("l'heure est :");
    horlogeinit(1,20,00);
    while(1){
        
   // lcd_pos('2',5);
      
    horloge();
    lcd_home();
   // lcd_pos('2',5);
    }
    return;
}
