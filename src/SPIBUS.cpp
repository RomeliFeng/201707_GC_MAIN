/*
 * SPIBUS.cpp
 *
 *  Created on: 2017年7月21日
 *      Author: Romeli
 */

#include "SPIBUS.h"
#include "Delay.h"

#define READY_PIN GPIO_Pin_12
#define READ_READY (GPIOB->IDR & READY_PIN)

void SPIBUS::Init() {
	GPIOInit();
}

void SPIBUS::Select(Salve_Typedef salve, FunctionalState newState) {
	GPIO_SetBits(GPIOD, Salve_CC);
	GPIO_SetBits(GPIOD, Salve_MC1);
	GPIO_SetBits(GPIOD, Salve_MC2);
	GPIO_SetBits(GPIOD, Salve_AC);
	GPIO_SetBits(GPIOE, Salve_FL);
	if (newState != DISABLE) {
		if (salve == Salve_FL) {
			GPIO_ResetBits(GPIOE, salve);
		} else {
			GPIO_ResetBits(GPIOD, salve);
		}
	}
}

bool SPIBUS::Ready() {
	volatile uint16_t last = millis();
	while (READ_READY != 0) {
		if (millis() - last > READYTIMELIMIT) {
			return false;
		}
	}
	return true;
}

void SPIBUS::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(
	RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = Salve_CC | Salve_MC1 | Salve_MC2 | Salve_AC;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Salve_FL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = READY_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOD, Salve_CC);
	GPIO_SetBits(GPIOD, Salve_MC1);
	GPIO_SetBits(GPIOD, Salve_MC2);
	GPIO_SetBits(GPIOD, Salve_AC);
	GPIO_SetBits(GPIOE, Salve_FL);
}
