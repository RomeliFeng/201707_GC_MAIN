/*
 * EEPROM.cpp
 *
 *  Created on: 2017年7月17日
 *      Author: Romeli
 */

#include "EEPROM.h"
#include "U_I2C2.h"

void EEPROM::Init() {
	U_I2C2::Init();
}

void EEPROM::Write(uint16_t add, uint8_t* data, uint8_t num) {
	U_I2C2::SendAsync(EEPROMADD, (uint8_t) (add >> 8), (uint8_t) add, data,
			num);
}

void EEPROM::Read(uint16_t add, uint8_t* data, uint8_t num) {
	U_I2C2::ReceiveSync(EEPROMADD, (uint8_t) (add >> 8), (uint8_t) add, data,
			num);
}
