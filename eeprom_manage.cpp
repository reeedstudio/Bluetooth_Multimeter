/*
  eeprom_manage.cpp
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
#include <EEPROM.h>
#include "eeprom_manage.h"
#include "BTMGlobalDfs.h"

/*********************************************************************************************************
** Function name: init
** Descriptions:  init 
*********************************************************************************************************/
unsigned char eeprom_manage::init()
{

	for(int i=0; i<256; i++)
	EEPROM.write(i, 0xff);
	return 0;
	
}

/*********************************************************************************************************
** Function name: e2prom_set
** Descriptions:  write a byte to eeprom
*********************************************************************************************************/
unsigned char eeprom_manage::e2prom_set(int addr,unsigned char dat)
{
	if(addr>=256 || addr<0)
		return 0;
    EEPROM.write(addr, dat);
	return 1;
}

/*********************************************************************************************************
** Function name: e2prom_get
** Descriptions:  get a byte from eeprom
*********************************************************************************************************/
unsigned char eeprom_manage::e2prom_get(int addr)
{
	char dat;
	if(addr>=256 || addr<0)
		return 0;
	return EEPROM.read(addr);
}

/*********************************************************************************************************
** Function name: e2prom_write
** Descriptions:  write a float(int ...) to eeprom
*********************************************************************************************************/
unsigned char eeprom_manage::e2prom_write(int addr,void *buffer,int len)
{
	int sum = 0;
	unsigned char *ch = (unsigned char*)buffer;
	for(int i=0; i<len; i++)
	{
		sum += e2prom_set(addr+i,ch[i]);
	}
	return sum;
}

/*********************************************************************************************************
** Function name: e2prom_read
** Descriptions:  read a float(int...) from eeprom
*********************************************************************************************************/
unsigned char eeprom_manage::e2prom_read(int addr,void *buffer,int len)
{
	unsigned char *ch;
	if(addr>=256 || addr<0)
		return 0;
	if(addr+len>=256)
		len = 256-addr;
	ch = (unsigned char*)buffer;
	for(int i=0;i<len;i++)
		ch[i] = e2prom_get(addr+i);
	return len;
}

eeprom_manage  EEPROMMANAGE;
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
