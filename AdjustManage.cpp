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

    int     intX[2];
    long    longX[2];
    float   fY[2];

    ifAdjust = EEPROM.read(1);

    if(ifAdjust != 0x55)
    {
        return ;
    }

    // vol
#if VOLADJ
    for(int i = 0; i<4; i++)
    {
        EEPM.read(EEPADDRVOLX+4*i, &intX[0], 2);
        EEPM.read(EEPADDRVOLX+4*i+2, &intX[1], 2);
        EEPM.read(EEPADDRVOLY+8*i, &fY[0], 4);
        EEPM.read(EEPADDRVOLY+8*i+4, &fY[1], 4);
        linearFit(2, intX, fY, volAdjustAB[i]);
        EEPM.read(EEPADDRVOLY_N+8*i, &fY[0], 4);
        EEPM.read(EEPADDRVOLY_N+8*i+4, &fY[1], 4);
        linearFit(2, intX, fY, volAdjustAB_n[i]);

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
        linearFit(2, intX, fY, ohmAdjustAB[i]);
    }
#endif

#if AMPADJ
    for(int i = 0; i<2; i++)
    {
        EEPM.read(EEPADDRAMPX+4*i, &intX[0], 2);
        EEPM.read(EEPADDRAMPX+4*i+2, &intX[1], 2);
        EEPM.read(EEPADDRAMPY+8*i, &fY[0], 4);
        EEPM.read(EEPADDRAMPY+8*i+4, &fY[1], 4);
        linearFit(2, intX, fY, ampAdjustAB[i]);
        EEPM.read(EEPADDRAMPY_N+8*i, &fY[0], 4);
        EEPM.read(EEPADDRAMPY_N+8*i+4, &fY[1], 4);
        linearFit(2, intX, fY, ampAdjustAB_n[i]);
    }
#endif

#if MAMPADJ
        EEPM.read(EEPADDRMAMPX, &intX[0], 2);
        EEPM.read(EEPADDRMAMPX+2, &intX[1], 2);
        EEPM.read(EEPADDRMAMPY, &fY[0], 4);
        EEPM.read(EEPADDRMAMPY+4, &fY[1], 4);
        linearFit(2, intX, fY, mAmpAdjustAB);
        EEPM.read(EEPADDRMAMPY_N, &fY[0], 4);
        EEPM.read(EEPADDRMAMPY_N+4, &fY[1], 4);
        linearFit(2, intX, fY, mAmpAdjustAB_n);
#endif
}

/*********************************************************************************************************
** Function name: lclearEEPROM
** Descriptions:  clear all eeprom
*********************************************************************************************************/
void AdjustManage::clearEEPROM()
{

    for(int i = 0; i<512; i++)
    {
        EEPROM.write(i, 0);
    }
}

/*********************************************************************************************************
** Function name: linearFit
** Descriptions:  The method of least squares linear fitting algorithm
*********************************************************************************************************/
void AdjustManage::linearFit(int n, int *x, float *y, float a[2])
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
** Function name: linearFit
** Descriptions:  The method of least squares linear fitting algorithm
*********************************************************************************************************/
void AdjustManage::linearFit(int n, long *x, float *y, float a[2])
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
    if(A6 == sign)
    {
        switch(ch)
        {
            case ADCHA0:

            *dta = (*dta - ampAdjustAB[1][0])/ampAdjustAB[1][1];
            break;

            case ADCHA1:
            *dta = (*dta - ampAdjustAB[0][0])/ampAdjustAB[0][1];
            break;

            case ADCHA2:
            *dta = (*dta - ampAdjustAB[0][0])/ampAdjustAB[0][1];

            break;

            default:

            ;
        }
    }
    else if(A7 == sign)
    {
        switch(ch)
        {
            case ADCHA0:

            *dta = (*dta - ampAdjustAB_n[1][0])/ampAdjustAB_n[1][1];
            break;

            case ADCHA1:
            *dta = (*dta - ampAdjustAB_n[0][0])/ampAdjustAB_n[0][1];
            break;

            case ADCHA2:
            *dta = (*dta - ampAdjustAB_n[0][0])/ampAdjustAB_n[0][1];

            break;

            default:

            ;
        }
    }

}

/*********************************************************************************************************
** Function name: ohmAdjust
** Descriptions:  ohm Adjust
*********************************************************************************************************/
unsigned char AdjustManage::ohmAdjust(unsigned char ch, float *dta)
{
    *dta = (*dta - ohmAdjustAB[ch][0])/volAdjustAB_n[ch][1];
    return 1;
}

/*********************************************************************************************************
** Function name: mampAdjust
** Descriptions:  mampAdjust
*********************************************************************************************************/
unsigned char AdjustManage::mampAdjust(unsigned char sign, float *dta)
{

    if(sign == A6)              // +
    {
        *dta = (*dta - mAmpAdjustAB[0])/mAmpAdjustAB[1];
    }
    else if(sign == A7)         // -
    {
        *dta = (*dta - mAmpAdjustAB_n[0])/mAmpAdjustAB_n[1];
    }
    else return 0;
    
    return 1;
}


AdjustManage BTMADJUST;


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/