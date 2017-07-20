/*
 * EEPROM.h
 *
 *  Created on: 2017年7月17日
 *      Author: Romeli
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#define EEPROMADD 0xae

#include "cmsis_device.h"

class EEPROM {
public:
	static void Init();
	static void Write(uint16_t add, uint8_t* data, uint8_t num);
	static inline void Write(uint16_t add, uint8_t data) {
		Write(add, &data, 1);
	}
	static void Read(uint16_t add, uint8_t *data, uint8_t num);
	static inline uint8_t Read(uint16_t add) {
		uint8_t tmp;
		Read(add, &tmp, 1);
		return tmp;
	}
};

#endif /* EEPROM_H_ */
