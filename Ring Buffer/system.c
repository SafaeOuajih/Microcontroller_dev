///////////////////////////////////////////////////////////////////////////////////////////////////
///  @file		System.c
///
///  @brief		initialise tout le syst�me
///
///  @date		15/04/2014
///  @author	        Ouajih Safae
///////////////////////////////////////////////////////////////////////////////////////////////////



#include "System.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// HARDWARE CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
// Config hardware boot (mask are only AND defined)

#if defined(__dsPIC33EP512MU810__) || defined(__dsPIC33EP512MU814__)
_FGS(GWRP_OFF & GSS_OFF & GSSK_OFF);
_FOSCSEL(FNOSC_FRC & IESO_ON);
_FOSC(POSCMD_HS & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSECME);
_FWDT(FWDTEN_OFF & WINDIS_OFF & PLLKEN_ON);
_FPOR(FPWRT_PWR1 & BOREN_OFF);
_FICD(ICS_PGD1 & RSTPRI_PF & JTAGEN_OFF);
_FAS(AWRP_OFF & APL_OFF & APLK_OFF);

#else
#error processeur non support�
#endif



///////////////////////////////////////////////////////////////////////////////////////////////////
// RAM allocation
///////////////////////////////////////////////////////////////////////////////////////////////////
/// structure contenant toutes les variables du syst�me
System sys							__attribute__((__near__));

static uint64 SIZE = 64;
static uint8 buffer1[SIZE];
static uint64 head1 = 0; // where we write  %p
static uint64 tail1 = 0; // where we read  %p

static uint64 SIZE2 = 64;
static NMEA buffer2[SIZE2];
static uint64 head2 = 0; // where we write %p
static uint64 tail2 = 0; // where we read %p


///////////////////////////////////////////////////////////////////////////////////////////////////
// SYSTEM boot
///////////////////////////////////////////////////////////////////////////////////////////////////

int8 systemPowerUp() {
    // switch frequency => quartz 16Mhz
    PLLFBD = 15 - 2;
	CLKDIVbits.PLLPOST = 0;
    CLKDIVbits.PLLPRE = 0;
    OSCTUN = 0;
    RCONbits.SWDTEN = 0;
	__builtin_write_OSCCONH(0x03); // Initiate Clock Switch to Primary Oscillator with PLL (XTPLL, HSPLL, ECPLL)
	__builtin_write_OSCCONL(0x01); // Start clock switching while (OSCCONbits.COSC != 0b001);
    while (OSCCONbits.COSC != 0x03); // Wait for Clock switch to occur
	while (OSCCONbits.LOCK != 1);

    // Watchdog disabled
    RCONbits.SWDTEN = 0;
    //enable multi level interrupt
    INTCON1bits.NSTDIS = 0;
    // disable analog pin
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    ANSELG = 0;

    return RETURN_SUCCESS;
}//powerUpSequence



///////////////////////////////////////////////////////////////////////////////////////////////////
// SYSTEM initialisation
///////////////////////////////////////////////////////////////////////////////////////////////////

int8 systemInit() {

    return RETURN_SUCCESS;
}//systemInit

///////////////////////////////////////////////////////////////////////////////////////////////////
// SYSTEM starting
///////////////////////////////////////////////////////////////////////////////////////////////////

uint16 cpt;
int8 systemStart() {
	cpt = 0;
	return RETURN_SUCCESS;
}//systemStart

///////////////////////////////////////////////////////////////////////////////////////////////////
// SYSTEM main loop
///////////////////////////////////////////////////////////////////////////////////////////////////
int8 systemLoop() {
	if (!cpt)
	{
		Time time = getTime();
	}
	cpt++;
	return RETURN_SUCCESS;
}//systemLoop


///////////////////////////////////////////////////////////////////////////////////////////////////
// SYSTEM shutdown
///////////////////////////////////////////////////////////////////////////////////////////////////

int8 systemShutDown() {
    return RETURN_SUCCESS;
}//systemShutDown



////////////////////////////////////////////////////////////////////////////////////////////////////
//ring buffer1
////////////////////////////////////////////////////////////////////////////////////////////////////

uint8 is_empty(){                 // return 1 if buffer is empty
  if((head1 - tail1))
    return RETURN_SUCCESS;
  else
    return RETURN_PROGRESS;
}

uint8 is_full(){                    // return 1 if buffer is full
  if((head1 - tail1)==SIZE)
    return RETURN_PROGRESS;
  else
    return RETURN_SUCCESS;
}

uint8 push_buf1(uint8 data) {       // add data to buffer return 1 if ok
  uint8 result = !is_full();
  if (result)
    buffer1[head1++ & (SIZE-1)] = data;
  return result;
}

uint8 pull_buf1(uint8 *data) {      // pull data from buffer return 1 if ok
  uint8 result = !is_empty();
  if (result)
    *data = buffer1[tail1++ & (SIZE-1)];
  return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//ring buffer2
////////////////////////////////////////////////////////////////////////////////////////////////////




uint8 is_empty2(){                  // return 1 if buffer is empty
  if((head2 - tail2))
    return RETURN_SUCCESS;
  else
    return RETURN_SUCCESS;
}

uint8 is_full2(){                     // return 1 if buffer is full
  if((head2 - tail2)==SIZE2)
    return RETURN_SUCCESS;
  else
    return RETURN_SUCCESS;
}

uint8 push_buf2(NMEA data) {            // add data to buffer return 1 if ok
  uint8 result = !is_full2();
  if (result)
    buffer2[head2++ & (SIZE2-1)] = data;
  return result;
}

uint8 pull_buf2(NMEA *data) {           // pull data from buffer, return 1 if ok
  uint8 result = !is_empty2();
  if (result)
    *data = buffer2[tail2++ & (SIZE2-1)];
  return result;
}
