/*
 * File:   test_led.c
 * Author: souajih
 *
 * Created on 8 avril 2019, 14:13
 */
 // CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#include <xc.h>
#define _XTAL_FREQ 4000000

init (void)
{
  //TRISA=0x00;
  TRISB=0x00; // configure le registre PORTD en sortie 
  T1CKPS0=0; // choix du préscaler (prédiviseur)
  T1CKPS1=0;
  TMR1IE=1; // bit d'activation de l'interruption du timer1
  GIE=1;  // activer toutes les interruptions
  PEIE=1; // autorisation des intéruptions des périphériques
  TMR1CS=1; /* in Timer  mode, Timer1  increments  every  instruction cycle.  In  Counter  mode,  it  increments  on  every  rising edge of the external clock input.
 TMR1CS 1 =timer mode;  0 =counter mode     */ 

  TMR1ON=1; //   bit d'activation du timer1
  CCPR1 = 0x8000; // la valeur à laquelle on compare le timer1 pour incrémenter à la seconde
  nT1SYNC = 0; // si TMR1CS=0 T1SYNC est ignoré ,sinon il permet la synchronisation pour 0.
    
  CCP1M0 = 0; // mode de comparaison pour générer l'interruption 
  CCP1M1 = 0;
  CCP1M2 = 0;
  CCP1M3 = 1;
    
  CCP1IE = 1; // activer la comparaison
  RB0 = 0;
}



void __interrupt isr(void)        // le nom 'IT' n'a pas d'importance tant qu'il y a le prefixe 'interrupt'     
{
    
    
  if (TMR1IF == 1)          // teste si c'est le timer qui a déclenché l'IT
    {
      RB0 = !RB0;
      TMR1IF = 0;   // désactiver le bit du flag d'interruption 
    }
  if (CCP1IF==1)          // teste si c'est le comparateur qui a déclenché l'IT
    {
      RB0 = !RB0;
        
      CCP1IF = 0;
    }
}



