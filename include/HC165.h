/*
 * HC165.h
 *
 *  Created on: 2017��1��6��
 *      Author: Romeli
 */

#ifndef HC165_H_
#define HC165_H_

#include "cmsis_device.h"

class HC165Class {
public:
	HC165Class(GPIO_TypeDef* GPIOx, uint16_t PL, uint16_t CP, uint16_t CE,
			uint16_t DAT);
	void Init();
	uint32_t Read(uint8_t len);
private:
	void GPIOInit();
	inline void Delay();
	GPIO_TypeDef* GPIOPort;
	uint16_t PL_PIN;
	uint16_t CP_PIN;
	uint16_t CE_PIN;
	uint16_t DAT_PIN;
};

#endif /* HC165_H_ */
