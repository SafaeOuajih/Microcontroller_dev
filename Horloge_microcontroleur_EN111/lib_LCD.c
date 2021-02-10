/*
 * File:   lib_LCD.c
 * Author: souajih
 *
 * Created on 29 avril 2019, 15:22
 */


// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#define _XTAL_FREQ 4000000

#include <xc.h>
#include <stdlib.h>
#include "lib_LCD.h"



#define D7 RD3
#define D6 RD2
#define D5 RD1
#define D4 RD0
#define RW RD5
#define RS RD4
#define E RD6
#define VCC RD7


#include "lib_LCD.h"
// contrôler l'approche ENABLE permettant de mémoriser une donnée avant de l'écrire dans un registre 

void edge(void) { 
    E = 1;
    __delay_us(1);
    E  = 0;
}

// Initialisation générale de l'afficheur en mode 4 bits

void lcd_init(void){

  TRISD = 0x00;   // configure le registre PORTD en sortie 
  PORTD = 0x00;
  VCC = 1;
  E = 0 ;
  __delay_ms(40); // retarder l'exécution de l'instruction suivante

  RW = 0;  // bit de configuration de choix entre lecture et écriture de la donnée (0 = écrire et 1 = lire)
  RS = 0;  // bit de configuration de choix entre traitement d'instruction ou de donnée (0 = traitement d'instruction et 1 = traitement de donnée)

  D7 = 0;
  D6 = 0;
  D5 = 1;
  D4 = 1;
                
  edge();
  __delay_ms(5);

  D7 = 0;
  D6 = 0;
  D5 = 1;
  D4 = 1;
        
  edge();
  __delay_us(200);
 

  D7 = 0;
  D6 = 0;
  D5 = 1;
  D4 = 1;
        
  edge();
  __delay_us(80);

  D7 = 0;
  D6 = 0;
  D5 = 1;
  D4 = 0;
        
  edge();
  __delay_us(80);

  D7 = 0;
  D6 = 0;
  D5 = 1;
  D4 = 0;
  edge();
  __delay_us(5);
  D7 = 1;
  D6 = 1;
  D5 = 0;
  D4 = 0;
  edge();
  __delay_us(80);

  D7 = 0;
  D6 = 0;
  D5 = 0;
  D4 = 0;
  edge();
  __delay_us(5);
  D7 = 1;
  D6 = 0;
  D5 = 0;
  D4 = 0;
        
  edge();
  __delay_us(80);

  D7 = 0;
  D6 = 0;
  D5 = 0;
  D4 = 0;
  edge();
  __delay_us(5);
  D7 = 0;
  D6 = 0;
  D5 = 0;
  D4 = 1;
  edge();
  __delay_ms(2);

       
  D7 = 0;
  D6 = 0;
  D5 = 0;
  D4 = 0;
  edge();
  __delay_us(5);
  D7 = 0;
  D6 = 1;
  D5 = 1;
  D4 = 0;
  edge();
  __delay_us(80) ;
       
       
  D7 = 0;
  D6 = 0;
  D5 = 0;
  D4 = 0;
  edge();
  __delay_us(5);
  D7 = 1;
  D6 = 1;
  D5 = 1;
  D4 = 1;
  edge();
  __delay_us(80) ;
}

// Test le bit BF jusqu'à ce que l'afficheur soit prêt
 
void lcd_busy(void){
    int test;
    TRISD = 0x0F;
    RS = 0; // choix de traitement d'instruction
    RW = 1; // choix de lecture de donnée
    test = D7;
    while(D7){
        TRISD = 0x0F;
        RS = 0;
        RW = 1;
        test = D7;
    }
    TRISD=0x00;
    }

// Envoi d'une instruction vers le module LCD 

void lcd_write_instr(unsigned char c){
    
  RS = 0; 
  RW = 0;
  //séparation du caractère c de 8 bits en deux caractères de 4 bits car l'afficheur est configuré en mode 4 bits
  char c1 = c >> 4; 
  char c2 = c & 0x0F; 

  PORTD &= 0xF0; // initialiser les 4 premiers bits du registre PORTD 
  PORTD |= c1;  // affecter le contenu du c1 au 4 derniers bits du registre PORTD
  edge();
  __delay_us(80);
    
  PORTD &= 0xF0; 
  PORTD |= c2;
  edge();
  __delay_us(80);

}

//Ecriture d'un caractère sur l'afficheur

void lcd_putch(unsigned char c){
    
  RS = 1; // choix de traitement de donnée
  RW = 0; // choix d'écriture de donnée
  char c1 = c >> 4; 
  char c2 = c & 0x0F; 
  PORTD &= 0xF0; 
  PORTD |= c1; 
  edge();
  __delay_us(80);
    
  PORTD &= 0xF0; 
  PORTD |= c2;
  edge();
  __delay_us(80);


}

//Ecriture d'une chaine de caractère sur l'afficheur

void lcd_puts(const unsigned char *s){
    int i=0;
    while(s[i]){
        lcd_putch(s[i]);
        i++;
    }



}

//Positionnement du curseur en (x,y) - origine en (1,1)

void lcd_pos(unsigned char ligne,unsigned char pos){
    if (ligne == '1'){
      lcd_write_instr(0x80 + pos-1); // envoi d'une instruction vers le module LCD pour une écriture dans la ligne 1 avec une adresse comprise entre 0x80 et 0x8F
    }
     if (ligne == '2'){
       lcd_write_instr(0xC0 + pos-1);  // envoi d'une instruction vers le module LCD pour une écriture dans la ligne 1 avec une adresse comprise entre 0xC0 et 0xCF
    }

}


// Effacement de l'écran et curseur en position initial (1,1)

void lcd_clear(){
    lcd_write_instr(1);
     __delay_ms(2);
}


// Curseur en position initial (1,1)

void lcd_home(){
    lcd_write_instr(2);
        __delay_ms(2);

}



