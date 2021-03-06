/*
 PAN3101.cpp - Part of optical mouse sensor library for Arduino
 Copyright (c) 2008 Martijn The.  All right reserved.
 http://www.martijnthe.nl/
 
 Based on sketches by Beno�t Rousseau.
 
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

/******************************************************************************
 * Includes
 ******************************************************************************/

#include "PAN3101.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

#define Delta_Y				0x02
#define Delta_X				0x03
#define Motion_Status		0x16

#define Mask_Motion			0x80
#define Mask_DYOVF			0x10
#define Mask_DXOVF			0x08

#define Operation_Mode1		0x00
#define Operation_Mode2		0x1A

/******************************************************************************
 * Constructors
 ******************************************************************************/


PAN3101::PAN3101(uint8_t sclkPin, uint8_t sdioPin) : OptiMouse::OptiMouse(sclkPin, sdioPin)
{

}

/******************************************************************************
 * User API
 ******************************************************************************/

void PAN3101::updateStatus(void)
{
	_status = readRegister(Motion_Status);
}

signed char PAN3101::dx(void)
{
	return (signed char) readRegister(Delta_X);
}

signed char PAN3101::dy(void)
{
	return (signed char) readRegister(Delta_Y);
}

uint8_t PAN3101::motion() const
{
	return (uint8_t) (_status & Mask_Motion) == Mask_Motion;
}

uint8_t PAN3101::dxOverflow() const
{
	return (uint8_t) (_status & Mask_DXOVF) == Mask_DXOVF;
}
uint8_t PAN3101::dyOverflow() const
{
	return (uint8_t) (_status & Mask_DYOVF) == Mask_DYOVF;
}

void PAN3101::force_awake_mode(bool b)
{
	uint8_t config = readRegister(Operation_Mode1);
	uint8_t mask = (1 << 0);
	if(b) {
		config |= mask;
	} else {
		config &= ~mask;
	}
	writeRegister(Operation_Mode1, config);
}

void PAN3101::toggle_awake_sleep(bool b)
{
	uint8_t config = readRegister(Operation_Mode2);
	uint8_t maska = (1 << 0);
	uint8_t masks = (1 << 1);
	if(b) {
		config |= maska;
	} else {
		config |= masks;
	}
	writeRegister(Operation_Mode2, config);
}

void PAN3101::toggle_led_shutter(bool b)
{
	uint8_t config = readRegister(Operation_Mode2);
	uint8_t mask = (1 << 3);
	if(b) {
		config |= mask;
	} else {
		config &= ~mask;
	}
	writeRegister(Operation_Mode2, config);
}

void PAN3101::toggle_power_down(bool b)
{
	uint8_t config = readRegister(Operation_Mode1);
	uint8_t mask = (1 << 6);
	if(b) {
		config |= mask;
	} else {
		config &= ~mask;
	}
	writeRegister(Operation_Mode1, config);
}

// Private Methods /////////////////////////////////////////////////////////////

