/*
  SmartMultimeter.h
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:
  Hardware: Albert Miao
  Firmware: Loovee
  
  https://github.com/reeedstudio/Bluetooth_Multimeter

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

#ifndef _SMARTMULTIMETER_H_
#define _SMARTMULTIMETER_H_

class SmartMultimeter
{
public:
    unsigned char cmdType;                  // cmd type

public:
    unsigned char dtaRevBt[30] ;            // data read from bluetooth
    unsigned char dtaSendBt[11];            // data send to bluetooth

private:
    void setSwitch(unsigned char sw);       // switch adc read channel
    bool adcIsInv();                        // if adc value inverter
    void setR(unsigned char r);
    void genDtaBt(float dta, unsigned char rType, unsigned char unit);  // generate the dta to bluetooth
    inline int Partition(int a,int b, int *N);
    void quickSort(int a,int b, int *N);    // quick sort
    int readADC(int pinAD);
    int readADC(int pinAD, int ch);
    void genVol();                          // generate voltage value, and save to dtaSendBt
    void genAmp();                          // generate ampl  value, V
    void genRes();                          // generate resistance value
#if SLEEP_MODE
    void sleepMode();
#endif
    
public:
    void init();
    void genAVR();


};

extern SmartMultimeter BTM;

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/