///////////////////////////////////////////////////////////////////////////////////////////////////
///  @file		System.h
///
///  @brief		initialise tout le syst�me
///
///  @date		15/04/2014
///  @author	N/A

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEM_H
#define _SYSTEM_H


#include <Os/Os.h>
#include "Os/Pic/Time/Time.h"
typedef struct
{
}Device;



typedef struct
{

}SystemStatus;


typedef struct
{
}Os;


typedef struct
{
	Os		os;
	Device		dev;
	SystemStatus	status;
}System;


int8 systemPowerUp();
int8 systemInit();
int8 systemStart();
int8 systemLoop();
int8 systemShutDown();


uint8 is_full();
uint8 is_empty();
uint8 push_buf1(uint8 data);
uint8 pull_buf1(uint8 *data) ;



uint8 is_full2();
uint8 is_empty2();
uint8 push_buf2(NMEA data);
uint8 pull_buf2(NMEA *data) ;

#endif
