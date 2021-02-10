#include "uart.h"

#define FCY 4000000
#define BAUDRATE 9600
#define BRGVAL ((FP/BAUDRATE)/16)-1
#define DELAY_105uS asm volatile ("REPEAT, #4201"); Nop();  // 105uS delay
_FOSC(CSW_FSCM_OFF & FRC);
//// variables
NMEA *data ;
uint8 status = 0;


void uart_init()
{
TMR2 = 0x0000;
PR2 = 0x0004;					// set Timer 2 period
INTCON2bits.GIE = 1;  // enable interrupt
IPC0bits.T2IP = 2;    // set Timer 2 interrupt priority
IFS0bits.T2IF = 0;    // clear Timer 2 interrupt flag
IEC0bits.T2IE = 1;    // enable Timer 2 interrupt
T2CON = 0x4020; 			//timer mode , 64 prescaler //, 1cycle per instruction?
T2CONbits.TON = 1;		// start Timer2


RPOR4bits.RP8R = 3;        // Make Pin RP8 U1TX

AD1PCFGL = 0x03C0;        // Make analog pins digital

U1MODEbits.STSEL = 0;                    // 1-Stop bit
U1MODEbits.PDSEL = 0;                    // No Parity, 8-Data bits
U1MODEbits.ABAUD = 0;                    // Auto-Baud disabled
U1MODEbits.BRGH = 0;                     // Standard-Speed mode

U1BRG = BRGVAL;                           // Baud Rate setting for 9600
U1STAbits.UTXISEL1 = 0;
U1MODEbits.UARTEN = 1;                   // Enable UART
U1STAbits.UTXEN = 1;                     // Enable UART TX
/* Wait at least 105 microseconds (1/9600) before sending first char */
DELAY_105uS
U1TXREG = '0';                           // Transmit one character
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
      IFS0bits.T2IF = 0;    // clear interrupt flag
      status = pull_buf2(data);             // buffer not empty , we pull
      if(status){
        U1TXREG = *data;                    // Transmit character
      }
      TMR2=0;                               // clear timer2
      T2CONbits.TON = 1;                    // start timer2
}
