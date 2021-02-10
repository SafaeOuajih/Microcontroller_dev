Test1 : Timing
  Use of timer1 // see Time.c and Time.h
  Validation : Use oscilloscope to check
              timing by adding a lcd or a led output  // see main.c

Test2 : Ring buffer
    1/ data type NMEA : direct push , pull via uart // timer2 __interrupt__ // see system.c and system.h
    2/ data type uint8 : direct push & pull // see system.c and system.h
    Validation :  1/ Direct check via uart :)
                  2/ Use lcd to check whats in buffer !
