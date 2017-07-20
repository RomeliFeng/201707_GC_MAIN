/*
 * HC165.cpp
 *
 *  Created on: 2017��1��6��
 *      Author: Romeli
 */

#include "HC165.h"

#define PL_SET GPIOPort->BSRR = PL_PIN
#define PL_RESET GPIOPort->BRR = PL_PIN
#define CP_SET GPIOPort->BSRR = CP_PIN
#define CP_RESET GPIOPort->BRR = CP_PIN
#define CE_SET GPIOPort->BSRR = CE_PIN
#define CE_RESET GPIOPort->BRR = CE_PIN
#define DAT_READ (GPIOPort->IDR & DAT_PIN)

HC165Class::HC165Class(GPIO_TypeDef* GPIOx, uint16_t PL, uint16_t CP,
		uint16_t CE, uint16_t DAT) {
	GPIOPort = GPIOx;
	PL_PIN = PL;
	CP_PIN = CP;
	CE_PIN = CE;
	DAT_PIN = DAT;
}

void HC165Class::Init() {
	GPIOInit();
}

uint32_t HC165Class::Read(uint8_t len) {
	uint32_t data = 0;
	PL_RESET;
	Delay();
	PL_SET; //
	CP_RESET; //
	CE_RESET; //
	Delay();
	uint32_t mask = (1 << (len - 1));
	if (DAT_READ != 0) {
		data |= mask;
	}
	mask >>= 1;
	for (; mask != 0; mask >>= 1) {
		CP_SET;
		Delay();
		if (DAT_READ != 0) {
			data |= mask;
		}
		CP_RESET;
		Delay();
	}
	CE_SET;
	Delay();
	return data;
}

void HC165Class::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;
	if (GPIOPort == GPIOA) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	} else if (GPIOPort == GPIOB) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	} else if (GPIOPort == GPIOC) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	} else if (GPIOPort == GPIOD) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	} else if (GPIOPort == GPIOE) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	} else if (GPIOPort == GPIOF) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	GPIO_InitStructure.GPIO_Pin = PL_PIN | CP_PIN | CE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOPort, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = DAT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOPort, &GPIO_InitStructure);
}

inline void HC165Class::Delay() {
	__NOP();
	__NOP();
	__NOP();
}

