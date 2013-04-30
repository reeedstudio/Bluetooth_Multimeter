/*
  SmartMultimeter.ino
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
#include <Wire.h>
#include <I2C_Uart.h>
#include <Sleep_n0m1.h>
#include <TimerOne.h>
#include <EEPROM.h>

#include "SmartMultimeter.h"
#include "BlueTooth.h"
#include "BTMGlobalDfs.h"
#include "eeprom_manage.h"
#include "AdjustManage.h"

#define TESTWITHOUTBT   0                       // test without bluetooth

bool getBtDta               = false;
unsigned char recvDtaLen    = 0;
unsigned char flagSleep     = 0;

/*********************************************************************************************************
** Function name: blueToothDtaProc
** Descriptions:  check if bluetooth get data and deal with it
*********************************************************************************************************/
bool blueToothDtaProc()
{

    if(!getBtDta) return 0;                     // if get data
    if(recvDtaLen != 7)return 0;                // check good data

    BTM.genAVR();
    blueToothSend(11, BTM.dtaSendBt);
    //digitalWrite(13, HIGH);
    //delay(10);
    //digitalWrite(13, LOW);
    recvDtaLen = 0;
    getBtDta   = false;
    return 1;
}

/*********************************************************************************************************
** Function name: i2cDtaProc
** Descriptions:  check if i2c get good data
*********************************************************************************************************/
bool i2cDtaProc()
{
    if(!EEPM.getDtaI2c)return 0;
    EEPM.getDtaI2c = 0;
    EEPM.dtaI2cLen = 0;
    return EEPM.putDta(EEPM.dtaI2c[2], EEPM.dtaI2c[3], &EEPM.dtaI2c[4]);
}

/*********************************************************************************************************
** Function name: setup
** Descriptions:  setup
*********************************************************************************************************/
void setup()
{
    BTM.init();
    BTMADJUST.init();
    blueTooth_Init();

    recvDtaLen = 0;
    Wire.begin(5);                          // i2c, for adjustment
    Wire.onReceive(receiveEvent);           // i2c data receive irq

}

/*********************************************************************************************************
** Function name: loop
** Descriptions:  loop
*********************************************************************************************************/
void loop()
{
    blueToothDtaProc();                     // bluetooth data
    i2cDtaProc();                           // for adjustment;
    delay(1);
}

/*********************************************************************************************************
** Function name: serialEvent
** Descriptions:  setial Event, get Serial data
*********************************************************************************************************/
void serialEvent()
{
    while (Serial.available()) 
    {
        BTM.dtaRevBt[recvDtaLen++] = (unsigned char)Serial.read();

        if(BTM.dtaRevBt[recvDtaLen-1] == DATAEND2 && BTM.dtaRevBt[recvDtaLen-2] == DATAEND1) {
            getBtDta = true;
        }

        if(recvDtaLen > 25)
        {
            recvDtaLen = 0;
            getBtDta = false;
        }
    }
}

/*********************************************************************************************************
** Function name: serialEvent
** Descriptions:  setial Event, get Serial data
*********************************************************************************************************/
void receiveEvent(int howMany)
{
    while(0 < Wire.available()) // loop through all but the last
    {
        EEPM.dtaI2c[EEPM.dtaI2cLen++] = (unsigned char)Wire.read();
        if(EEPM.dtaI2c[EEPM.dtaI2cLen-2] == END1 && EEPM.dtaI2c[EEPM.dtaI2cLen-1] == END2)
        {
            EEPM.getDtaI2c = 1;
        }
    }

}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/