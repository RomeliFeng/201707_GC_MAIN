/*
 * LED.cpp
 *
 *  Created on: 2017年7月10日
 *      Author: Romeli
 */

#include "LED.h"

#define RED_PIN GPIO_Pin_6
#define BLUE_PIN GPIO_Pin_2
#define GREEN_PIN GPIO_Pin_3

#define RED_SET (GPIOD->BSRR = RED_PIN)
#define RED_RESET (GPIOD->BRR = RED_PIN)
#define BLUE_SET (GPIOD->BSRR = BLUE_PIN)
#define BLUE_RESET (GPIOD->BRR = BLUE_PIN)
#define GREEN_SET (GPIOD->BSRR = GREEN_PIN)
#define GREEN_RESET (GPIOD->BRR = GREEN_PIN)

void LED::Init() {
	GPIOInit();
}

void LED::Turn(Color_Typedef color) {
	switch (color) {
	case Color_None:
		RED_SET;
		BLUE_SET;
		GREEN_SET;
		break;
	case Color_Red:
		RED_RESET;
		BLUE_SET;
		GREEN_SET;
		break;
	case Color_Blue:
		RED_SET;
		BLUE_RESET;
		GREEN_SET;
		break;
	case Color_Green:
		RED_SET;
		BLUE_SET;
		GREEN_RESET;
		break;
	default:
		break;
	}
}

void LED::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = RED_PIN | BLUE_PIN | GREEN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	RED_SET;
	BLUE_SET;
	GREEN_SET;
}
