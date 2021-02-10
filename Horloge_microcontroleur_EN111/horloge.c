/*
 * File:   horloge.c
 * Author: souajih
 *
 * Created on 9 mai 2019, 10:55
 */


#include <xc.h>
#include "lib_LCD.h"
#include "horloge.h"


#define _XTAL_FREQ 4000000
#define S2 RA4
#define S3 RB0

// initialisation des registres

init (void)
{
    TRISA=0xFF;
    TRISB=0xFF;

    T1CKPS0=0;
    T1CKPS1=0;
    TMR1IE=1;
    GIE=1;
    PEIE=1;
    TMR1CS=1;
    TMR1ON=1;
    CCPR1 = 0x8000;
    nT1SYNC = 0;

    CCP1M0 = 0;
    CCP1M1 = 0;
    CCP1M2 = 0;
    CCP1M3 = 1;

    CCP1IE = 1;
    RB0 = 0;
}

int s=0; // secondes
int m=0; // minutes
int h=0; // heures
 int blinkHH=0;
 int blinkMM=0;
 int incrH=0;
 int incrM=0;

// fonction d'activation des interruptions permettant d'incrémenter l'heure

void __interrupt isr(void)        // le nom 'IT' n'a pas d'importance tant qu'il y a le prefixe 'interrupt'
{

  // incrémentation de l'heure à partir du timer1

    if (TMR1IF == 1)          // teste si c'est le timer qui a déclenché l'IT
    {
      s++; // incrémentation des secondes
        if(s>=60){
            s=0;
            m++; // incrémentation des minutes
        }
        if(m>=60){
            m=0;
            h++; // incrémentation des heures
        }
        if(h>=24){
	  s=0; // initialisation
	  h=0;
	  m=0;
        }

     TMR1IF = 0;
     }


  // incrémentation de l'heure à partir de la comparaison du timer1 avec la valeur 0x8000
    // cette comparaison permet d'incrémenter toutes les secondes


    if (CCP1IF==1)   {     // teste si c'est le timer qui a déclenché l'IT

        s++;
        if(s>=60){
            s=0;
            m++;
        }
        if(m>=60){
            m=0;
            h++;
        }
        if(h>=24){
            s=0;
            h=0;
            m=0;
        }

        CCP1IF = 0;

    }
}



// initialisation de l'horloge

void horlogeinit(int h0, int m0, int s0){

    s = s + s0;
    h = h + h0;
    m = m + m0;

}

// affichage de l'heure sur l'afficheur LCD

void horlogeaffichH(int i){

  lcd_putch((i/10)+'0'); // le zéro ajouté a pour but de convertir l'entier en caractére puisque la fonction lcd_putch prend un caractére en parmétre
  lcd_putch((i%10)+'0');
}

// affichage des minutes sur l'afficheur LCD

void horlogeaffichM(int j){
  lcd_puts(":");

  lcd_putch((j/10)+'0');
  lcd_putch((j%10)+'0');

}

// affichage des secondes sur l'afficheur LCD


void horlogeaffichS(int d){
  lcd_puts(":");

  lcd_putch((d/10)+'0');
  lcd_putch((d%10)+'0');
}

// clignotement de l'heure lors d'un réglage manuel à l'aide du bouton S3 associé à RB0

void blinkH(void){
    blinkHH=1;
    while((blinkHH==1)&&(S3==1)){

   // lcd_pos('2',5);
        lcd_puts("  ");
        horlogeaffichM(m);
        horlogeaffichS(s);
        lcd_home();
   // lcd_pos('2',5);
    __delay_us(10);
    //lcd_pos('2',5);

        horlogeaffichH(h);
        horlogeaffichM(m);
        horlogeaffichS(s);
        lcd_home();
   // lcd_pos('2',5);

    }
}

//  clignotement des minuites lors d'un réglage manuel à l'aide du bouton S3 associé à RB0

void blinkM(void){
    blinkMM=1;
    while((blinkMM==1)&&(S3==1)){

    //lcd_pos('2',5);

        horlogeaffichH(h);
        lcd_puts(":");
        lcd_puts("  ");
        horlogeaffichS(s);
        lcd_home();
   // lcd_pos('2',5);
    __delay_us(100);
  //  lcd_pos('2',5);

        horlogeaffichH(h);
        horlogeaffichM(m);
        horlogeaffichS(s);
        lcd_home();
  //  lcd_pos('2',5);

    }
}



// fonction principal de l'horloge

void horloge(void){

    int n1=0; // variable pour déterminer le passage d'un réglage des heures à un réglage des minutes
    if(S2==0){ // appui sur S2 pendant 2 secondes active le réglage manuel
       __delay_ms(2000);

        if((S2==0)){
            n1 = 1;
            blinkH();
            __delay_ms(100);
            if((S3==0)){  // clignotement de l'heure tant que S3 n'est pas appuié
                blinkHH = 0;
                while(S2==1){
                    incrH=0;
                    if((S3==0)&&(incrH==0)){
                        h++;
                        __delay_ms(400);
                    }
                        horlogeaffichH(h);
                        horlogeaffichM(m);
                        horlogeaffichS(s);
                        lcd_home();
                    int s1 = s + 2; // variable qui calcule les 2 secondes avant de modifier manuellement
                    if((S3==0)&&(s >= s1)){
                        incrH==1;
                        while((S3==0)&&(S2==1)){ // incrementation manuelle de l'heure
                            h++;
                             horlogeaffichH(h);
                             horlogeaffichM(m);
                             horlogeaffichS(s);
                             lcd_home();
                            __delay_ms(200);
                         }
                    }
                }

            }

                __delay_ms(50);
                if((S2==0)&&(S3==1)&&(n1==1)){ // réglage manuelle des minutes
                    n1++;
                    blinkM();
                    if((S3==0)&&(S2==1)){ // clignotement de l'heure tant que S3 n'est pas appuié
                     blinkMM = 0;
                     while(S2==1){
                        incrM=0;
                        if((S3==0)&&(incrM==0)){ // incrémentation manuelle des minutes
                            m++;
                            __delay_ms(400);
                        }
                            horlogeaffichH(h);
                            horlogeaffichM(m);
                            horlogeaffichS(s);
                            lcd_home();
                        int s2 = s + 2;
                        if((S3==0)&&(s >= s2)){
                            incrH==1;
                            while((S3==0)&&(S2==1)){
                                h++;
                                 horlogeaffichH(h);
                                 horlogeaffichM(m);
                                 horlogeaffichS(s);
                                 lcd_home();
                                __delay_ms(200);
                             }
                        }
                    }
                         }
                     }

     if((S2==0)&&(S3==1)&&(n1==2)){ // retour à l'état initial
            horlogeaffichH(h);
            horlogeaffichM(m);
            horlogeaffichS(s);
            lcd_home();
            n1 = 0;
            }
    }
    }
    if((S2==1)&&(S3==1)&&(n1==0)){ // commande incorrecte donc aucune possibilité de réglage manuelle
            horlogeaffichH(h);
            horlogeaffichM(m);
            horlogeaffichS(s);




            }

}
