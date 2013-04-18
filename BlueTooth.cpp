/*
  Bluetooth.cpp
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
#include <Arduino.h>
#include "BlueTooth.h"

void blueTooth_Init()
{
    //Serial.begin(38400); //Set BluetoothBee BaudRate to default baud rate 38400
    Serial.begin(38400);
    delay(500);
    setupBlueToothConnection();
}

// SEND
void blueToothSend(INT8U len, INT8U *dataSend)
{
    // data head
    for(int i = 0; i<len; i++)
    {
        Serial.write(dataSend[i]);
    }
    
}

static void setupBlueToothConnection()
{
    sendBlueToothCommand("\r\n+STWMOD=0\r\n");
    CheckOK();
    sendBlueToothCommand("\r\n+STNA=BT MULTIMETER\r\n");
    CheckOK();
    sendBlueToothCommand("\r\n+STAUTO=0\r\n");
    CheckOK();
    sendBlueToothCommand("\r\n+STOAUT=1\r\n");
    CheckOK();
    sendBlueToothCommand("\r\n +STPIN=0000\r\n");
    checkBluetoothState('1');
    sendBlueToothCommand("\r\n+INQ=1\r\n");
    checkBluetoothState('4');
}

//Checks if the response "OK" is received
static void CheckOK()
{
    char a,b;
    while(1)
    {
        if(Serial.available())
        {
            a = Serial.read();
            if('O' == a)
            {
                // Wait for next character K. available() is required in some cases, as K is not immediately available.
                while(Serial.available())
                {
                    b = Serial.read();
                    break;
                }
                if('K' == b)
                {
                    break;
                }
            }
        }
        delay(1);
    }
}

static void sendBlueToothCommand(char command[])
{
    Serial.print(command);
}

static void checkBluetoothState(char state)
{
    unsigned char temp;
    while(1)  
    {
        while(!Serial.available());
        temp=Serial.read();
        if(temp=='+')  
        {
            while(temp!=state)  
            {
                if(Serial.available())
                temp=Serial.read();
            }
            if(temp==state)  
            {
                break;
            }
        }
        delay(1);
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
