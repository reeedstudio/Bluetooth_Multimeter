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
    int volX[8];
    float volY[8];
    float volY_n[8];
    
    for(int i = 0; i<8; i++)
    {
        EEPM.read(EEPADDRVOLX+2*i, &volX[i], 2);
        EEPM.read(EEPADDRVOLY+4*i, &volY[i], 4);
        EEPM.read(EEPADDRVOLY_N+4*i, &volY_n[i], 4);
    }

    for(int i = 0; i<4; i++)
    {
        lsline(2, &volX[2*i], &volY[2*i], volAdjustAB[i]);
        lsline(2, &volX[2*i], &volY_n[2*i], volAdjustAB_n[i]);
    }
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
** Function name: volAdjust
** Descriptions:  vol test adjust
*********************************************************************************************************/
void AdjustManage::volAdjust(unsigned char sign, unsigned char ch, float *dta)
{

    unsigned char adchvNum[4] = {1, 3, 0, 2};
    if(NSIGN == sign)     // adjust
    {
        *dta = (*dta - volAdjustAB_n[adchvNum[ch]][0])/volAdjustAB_n[adchvNum[ch]][1];
    }
    else
    {
        *dta = (*dta - volAdjustAB[adchvNum[ch]][0])/volAdjustAB[adchvNum[ch]][1];
    }

}

/*********************************************************************************************************
** Function name: ampAdjust
** Descriptions:  amp test adjust
*********************************************************************************************************/
void AdjustManage::ampAdjust(unsigned char sign, unsigned char ch, float *dta)
{
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
