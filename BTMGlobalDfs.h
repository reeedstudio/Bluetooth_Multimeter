/*
  BTMGlobalDfs.h
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.
 
  Author: 
  Hardware: Albert Miao 
  Firmware: Loovee
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef __BTMGLOBALDFS_H__
#define __BTMGLOBALDFS_H__

#include <Arduino.h>

#define __DEBUG_I2C     0

#if __DEBUG_I2C
#define PRINT(X)        __print(X)
#define PRINTLN(X)      __println(X)
#else
#define PRINT(X)
#define PRINTLN(X)
#endif

#define SLEEP_MODE      0
#define FRESHRATE       300             // ms

#define VREF            (float)1.25     // adc voltage reference

#define MERGEMAX        20
#define AVETIME         200

#define DATASTART1          0x53
#define DATASTART2          0x00
#define DATAEND1            0x2f
#define DATAEND2            0x45
//pin define
#define PINADCINV       2       // 
#define PINSWS0         5       // 74hc4051 s0
#define PINSWS1         6       // 74hc4051 s1
#define PINSWS2         7       // 74hc4051 s2

#define PINR1           8
#define PINR2           9
#define PINR3           10
#define PINR4           11

//R of R
#define R1              1800
#define R2              18000
#define R3              180000
#define R4              1800000

// choos the chanel of ADC
#define ADCHV2          0           // 0.34     1.25-3.67
#define ADCHV4          1           // 0.03076  10-33
#define ADCHV1          2           // 1        0-1.25 
#define ADCHV3          3           // 0.125    3.67-10
#define ADCHA0          4           // x1
#define ADCHA1          5           // x10
#define ADCHA2          6           // x100
#define ADCHR           7

//
#define TIMESV1         1
#define TIMESV2         3
#define TIMESV3         8
#define TIMESV4         26


// bluetooth send 
#define DTASENDBTSTART1 0
#define DTASENDBTSTART2 1
#define DTASENDBTLEN    2
#define DTASENDBTTYPE   3
#define DTASENDBTN1     4
#define DTASENDBTN2     5
#define DTASENDBTF      6
#define DTASENDBTUNIT   7
#define DTASENDBTCRC    8
#define DTASENDBTEND1   9
#define DTASENDBTEND2   10

// bluetooth rev
#define DTAREVBTSTART1  0
#define DTAREVBTSTART2  1
#define DTAREVBTLEN     2
#define DTAREVBTTYPE    3
#define DTAREVBTCRC     4
#define DTAREVBTEND1    5
#define DTAREVBTEND2    6

// data start and end
#define DTASTART1       0x53
#define DTASTART2       0x00
#define DTASEND1        0x2F
#define DTASEND2        0x45

// data type android -> smart multimeter
#define GETV            0x01
#define GETA            0x02
#define GETMA           0x03
#define GETR            0x04

// data type smart multimeter -> android
#define RETV            0x81
#define RETA            0x82
#define RETMA           0x83
#define RETR            0x84

// unit return
#define UNITMV          0x01
#define UNITV           0x02
#define UNITMA          0x03
#define UNITA           0x04
#define UNITR           0x05
#define UNITKR          0x06
#define UNITMR          0x07

#define UNITNMV         0x81
#define UNITNV          0x82
#define UNITNMA         0x83
#define UNITNA          0x84

// button state
#define BUTTONB         0x01
#define BUTTONF         0x02
#define BUTTONBF        0x03

#define BUTTONBVAL      360
#define BUTTONFVAL      650
#define BUTTONBFVAL     250

// frame form i2c
#define START1          0x53
#define START2          0x19
#define END1            0x2f
#define END2            0x45

//
#define PSIGN           A6      // Positive sign
#define NSIGN           A7      // Negative sign

// value of adjust standard
#define VALVOL0         50  
#define VALVOL1         1200
#define VALVOL2         1300
#define VALVOL3         3600
#define VALVOL4         3700
#define VALVOL5         13400
#define VALVOL6         13500
#define VALVOL7         30000

//data in eeprom
#define EEPADDRVOLAB    20
#define EEPADDRVOLAB_N  EEPADDRVOLAB+32
#define EEPADDRVOLX     EEPADDRVOLAB_N+32
#define EEPADDRVOLY     EEPADDRVOLX+16
#define EEPADDRVOLY_N   EEPADDRVOLY+32




#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
