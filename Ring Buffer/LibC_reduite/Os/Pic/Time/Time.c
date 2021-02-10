///////////////////////////////////////////////////////////////////////////////////////////////////
///  @file		Time.c
///
///  @brief		Gestion du temps
///
///  @date		15/04/2014
///  @author	N/A
///  @copyright	SYSNAV
///////////////////////////////////////////////////////////////////////////////////////////////////



#include "Time.h"

#define FCY 4000000 // (16 MHZ / 4)
_FOSC(CSW_FSCM_OFF & FRC); //16Mhz
static Time s =  0; // secondes

void init (void)
{
	s=0;
	TMR1 = 0x0000;
	PR1 = 0x0004;					// set Timer 1 period
	INTCON2bits.GIE = 1;  // enable interrupt
	IPC0bits.T1IP = 2;    // set Timer 1 interrupt priority
	IFS0bits.T1IF = 0;    // clear Timer 1 interrupt flag
	IEC0bits.T1IE = 1;    // enable Timer 1 interrupt
	T1CON = 0x8010; 			//timer mode , 8 prescaler //, 1cycle per instruction?
	T1CONbits.TON = 1;		// start Timer1
}



Time getTime()
{
	return s ;
};

void __attribute__((__interrupt__,no_auto_psv)) _T1Interrupt(void)
{
	s++; // +1 us
	TMR1 = 0x0000;
	// clear Timer1 interrupt
	IFS0bits.T1IF = 0;
	T2CONbits.TON = 1;                    // start timer2

}
