/*
  AdjustManage.cpp
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
#include <Arduino.h>
#include <EEPROM.h>

#include "AdjustManage.h"
#include "BTMGlobalDfs.h"
#include "eeprom_manage.h"

/*********************************************************************************************************
** Function name: init
** Descriptions:  init
*********************************************************************************************************/
void AdjustManage::init()
{
/*

    int xnI2A[4]    = {50, 230, 250, 2000};
    float ynI2A[4]  = {45, 236, 225, 2070};

    int xI2A[4]     = {50, 200, 250, 2000};
    float yI2A[4]   = {55, 212, 259, 2090};

    for(int i = 0; i<2; i++)
    {
        lsline(2, &xnI2A[2*i], &ynI2A[2*i], ampAdjustAB_n[i]);
        lsline(2, &xI2A[2*i],  &yI2A[2*i],  ampAdjustAB[i]);
    }
    for(int i = 0; i<4; i++)
    {
        lsline(2, &xV[2*i], &ynV[2*i], volAdjustAB_n[i]);
        lsline(2, &xV[2*i], &yV[2*i], volAdjustAB[i]);
    }
*/

    int     intX[2];
    long    longX[2];
    float   fY[2];

    ifAdjust = EEPROM.read(1);

    if(ifAdjust != 0x55)
    {
        return ;
    }

    // vol
    for(int i = 0; i<4; i++)
    {
        EEPM.read(EEPADDRVOLX+4*i, &intX[0], 2);
        EEPM.read(EEPADDRVOLX+4*i+2, &intX[1], 2);
        EEPM.read(EEPADDRVOLY+8*i, &fY[0], 4);
        EEPM.read(EEPADDRVOLY+8*i+4, &fY[1], 4);
        lsline(2, intX, fY, volAdjustAB[i]);
#if 0
        Serial.print("long X:");
        Serial.print(intX[0]);
        Serial.print('\t');
        Serial.println(intX[1]);
#endif
        EEPM.read(EEPADDRVOLY_N+8*i, &fY[0], 4);
        EEPM.read(EEPADDRVOLY_N+8*i+4, &fY[1], 4);
        lsline(2, intX, fY, volAdjustAB_n[i]);

    }
#if 0
    for(int i = 0; i<4; i++)
    {
        Serial.print(volAdjustAB[i][0]);
        Serial.print('\t');
        Serial.println(volAdjustAB[i][1]);
    }
#endif

    // ohm
#if OHMADJ
    for(int i = 0; i<4; i++)
    {
        EEPM.read(EEPADDROHMX+8*i, &longX[0], 4);
        EEPM.read(EEPADDROHMX+8*i+4, &longX[1], 4);
        EEPM.read(EEPADDROHMY+8*i, &fY[0], 4);
        EEPM.read(EEPADDROHMY+8*i+4, &fY[1], 4);
        lsline(2, intX, fY, ohmAdjustAB[i]);
    }
#endif
}

/*********************************************************************************************************
** Function name: lsline
** Descriptions:  The method of least squares linear fitting algorithm
*********************************************************************************************************/
void AdjustManage::lsline(int n, int *x, float *y, float a[2])
{

    float ave_x = 0;
    float ave_y = 0;
    float sumx2 = 0;
    float sumxy = 0;

    for(int i = 0; i<n; i++)
    {
        ave_x += x[i];
        ave_y += y[i];
    }

    ave_x = ave_x/n;
    ave_y = ave_y/n;

    for(int i = 0; i<n; i++)
    {
        sumx2 += (x[i] - ave_x) * (x[i] - ave_x);
        sumxy += (y[i] - ave_y) * (x[i] - ave_x);
    }

    a[1] = (float)sumxy/(float)sumx2;
    a[0] = ave_y - a[1]*ave_x;
}

/*********************************************************************************************************
** Function name: lsline
** Descriptions:  The method of least squares linear fitting algorithm
*********************************************************************************************************/
void AdjustManage::lsline(int n, long *x, float *y, float a[2])
{

    float ave_x = 0;
    float ave_y = 0;
    float sumx2 = 0;
    float sumxy = 0;

    for(int i = 0; i<n; i++)
    {
        ave_x += x[i];
        ave_y += y[i];
    }

    ave_x = ave_x/n;
    ave_y = ave_y/n;

    for(int i = 0; i<n; i++)
    {
        sumx2 += (x[i] - ave_x) * (x[i] - ave_x);
        sumxy += (y[i] - ave_y) * (x[i] - ave_x);
    }

    a[1] = (float)sumxy/(float)sumx2;
    a[0] = ave_y - a[1]*ave_x;
}

/*********************************************************************************************************
** Function name: volAdjust
** Descriptions:  vol test adjust
*********************************************************************************************************/
unsigned char AdjustManage::volAdjust(unsigned char sign, unsigned char ch, float *dta)
{
    IFADJUSTED;
    unsigned char adchvNum[4] = {1, 3, 0, 2};
    if(NSIGN == sign)     // adjust
    {
        *dta = (*dta - volAdjustAB_n[adchvNum[ch]][0])/volAdjustAB_n[adchvNum[ch]][1];
    }
    else
    {
        *dta = (*dta - volAdjustAB[adchvNum[ch]][0])/volAdjustAB[adchvNum[ch]][1];
    }

    return 1;

}

/*********************************************************************************************************
** Function name: ampAdjust
** Descriptions:  amp test adjust
*********************************************************************************************************/
unsigned char AdjustManage::ampAdjust(unsigned char sign, unsigned char ch, float *dta)
{
    IFADJUSTED;
/*
        if(A6 == pinAD)
        {
            switch(ch)
            {
                case ADCHA0:

                iGet = (iGet - ampAdjustAB[1][0])/ampAdjustAB[1][1];
                break;

                case ADCHA1:
                iGet = (iGet - ampAdjustAB[0][0])/ampAdjustAB[0][1];
                break;

                case ADCHA2:
                iGet = (iGet - ampAdjustAB[0][0])/ampAdjustAB[0][1];

                break;

                default:

                ;
            }
        }
        else if(A7 == pinAD)
        {
            switch(ch)
            {
                case ADCHA0:

                iGet = (iGet - ampAdjustAB_n[1][0])/ampAdjustAB_n[1][1];
                break;

                case ADCHA1:
                iGet = (iGet - ampAdjustAB_n[0][0])/ampAdjustAB_n[0][1];
                break;

                case ADCHA2:
                iGet = (iGet - ampAdjustAB_n[0][0])/ampAdjustAB_n[0][1];

                break;

                default:

                ;
            }
        }
*/
}


AdjustManage BTMADJUST;


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/