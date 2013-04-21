/*
  SmartMultimeter.ino
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

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
#include "SmartMultimeter_dfs.h"

#define TESTWITHOUTBT   0       // test without bluetooth

bool getBtDta               = false;
unsigned char recvDtaLen    = 0;
unsigned char flagSleep     = 0;

/*********************************************************************************************************
** Function name: dispBtDta
** Descriptions:  display get data
*********************************************************************************************************/
void dispBtDta()
{  
#if __DEBUG_I2C
    unsigned int tmp = 0;
    tmp  =  SmartVom.dtaSendBt[DTASENDBTN1];
    tmp  =  tmp<<8;
    tmp  |= SmartVom.dtaSendBt[DTASENDBTN2];
    float n = (float)tmp;
    n += (float)SmartVom.dtaSendBt[DTASENDBTF]/100;
    
    if(SmartVom.dtaSendBt[DTASENDBTUNIT] > 10)
    {
        __print("-");
    }
    putFloat(n);
    
    char s[][5] = {" mV", " V", " mA", " A", " R", " KR", " MR"};
    
    __println(s[SmartVom.dtaSendBt[DTASENDBTUNIT]%0x80-1]);
#endif    
}

/*********************************************************************************************************
** Function name: checkGoodDta
** Descriptions:  check if bluetooth get good data
*********************************************************************************************************/
bool checkGoodDta(unsigned char *dta)
{

    if(recvDtaLen == 7 && dta[0] == DATASTART1 && dta[1] == DATASTART2 &&dta[2] == 1 &&dta[4] == 0 && dta[5] == DATAEND1 && dta[6] == DATAEND2)
    {
        return 1;
    }
    return 0;

}

/*********************************************************************************************************
** Function name: setup
** Descriptions:  setup
*********************************************************************************************************/
void setup()
{
#if __DEBUG_I2C
    I2C_Uart_Init();
    delay(10);
    PRINTLN("I2C INIT OVER!!");
#endif
    SmartVom.init();
    blueTooth_Init();
    PRINTLN("BLUETOOTH CONNECTED!!");
    recvDtaLen = 0;
    PRINTLN("ALL INIT OVER!!");

}

/*********************************************************************************************************
** Function name: loop
** Descriptions:  loop
*********************************************************************************************************/
void loop()
{

    if(getBtDta) {
        if(checkGoodDta(SmartVom.dtaRevBt))
        {
            SmartVom.genAVR();
            dispBtDta();
            blueToothSend(11, SmartVom.dtaSendBt);
          
            digitalWrite(13, HIGH);
            delay(10);
            digitalWrite(13, LOW);
        }
        recvDtaLen = 0;
        getBtDta   = false;
    }

    delay(1);
    
}

/*********************************************************************************************************
** Function name: serialEvent
** Descriptions:  setial Event, get Serial data
*********************************************************************************************************/
void serialEvent() 
{
    while (Serial.available()) {
        // get the new byte:
        SmartVom.dtaRevBt[recvDtaLen++] = (unsigned char)Serial.read();

        if(SmartVom.dtaRevBt[recvDtaLen-1] == DATAEND2 && SmartVom.dtaRevBt[recvDtaLen-2] == DATAEND1) {
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
  END FILE
*********************************************************************************************************/
