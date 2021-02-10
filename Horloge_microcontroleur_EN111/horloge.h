/*
 * File:   horloge.h
 * Author: souajih
 *
 * Created on 28 mai 2019, 14:42
 */


#include <xc.h>
#define _XTAL_FREQ 4000000


# ifndef HORLOGE_H
# define HORLOGE_H 
void blinkH(void);
void blinkM(void);
void horlogeaffichH(int i);
void horlogeaffichM(int j);
void horlogeaffichS(int d);
void horlogeinit(int h0, int m0, int s0);
void horloge(void);
#endif