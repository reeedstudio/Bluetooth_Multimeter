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
#include "AdjustManage.h"
/*********************************************************************************************************
** Function name: init
** Descriptions:  init
*********************************************************************************************************/
void AdjustManage::init()
{
    int xV[8] = {100, 1200, 1300, 3700, 3800, 13700, 14000, 30000};
    float ynV[8] = {106, 1210, 1220, 3660, 3480, 13360, 13480, 29480};
    float yV[8]  = { 88, 1180, 1380, 3800, 3900, 13800, 14100, 30100};
    
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
    float sumxy = 0;;

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
  END FILE
*********************************************************************************************************/
