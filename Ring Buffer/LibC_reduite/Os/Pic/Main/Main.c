///////////////////////////////////////////////////////////////////////////////////////////////////
///  @file		Main.c
///
///  @brief		G�re la boucle principale du main
///
///  @date		15/04/2014
///  @author	N/A
///  @copyright	SYSNAV
///////////////////////////////////////////////////////////////////////////////////////////////////



#include "Main.h"


int main()
{

// oscillator and specific boot
	systemPowerUp();

/////////////////////////////////
// INIT AND TEST
/////////////////////////////////

  init();      // timer 1 init
  uart_init(); // uart and timer 2 init
	Time test_time = getTime();  // add push lcd to verify !


  uint8 DataPush = 2;
  uint8 DataP = 0;
  uint8 *DataPull = &DataP;
  uint8 result = push_buf1(DataPush);
  if(result){
    uint8 result2 = pull_buf1(DataPull);
    // add push lcd to verify !
  }


  NMEA DataPush2 = 2;
  uint8 result3 = push_buf2(DataPush2);
  // pull is done by uart

/////////////////////////////////
// END
/////////////////////////////////

	// user init
		systemInit();

	// start devices
		systemStart();


	// main loop
	while(1)
	{
		systemLoop();
	}

	// shut down
	systemShutDown();

	return RETURN_ERROR;
}//main
