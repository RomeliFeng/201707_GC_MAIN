/*
 * HC595.cpp
 *
 *  Created on: 2016��12��24��
 *      Author: Romeli
 */

#include "HC595.h"

#define DS_SET GPIOPort->BSRR = DS_PIN
#define DS_RESET GPIOPort->BRR = DS_PIN
#define OE_SET GPIOPort->BSRR = OE_PIN
#define OE_RESET GPIOPort->BRR = OE_PIN
#define STCP_SET GPIOPort->BSRR = STCP_PIN
#define STCP_RESET GPIOPort->BRR = STCP_PIN
#define SHCP_SET GPIOPort->BSRR = SHCP_PIN
#define SHCP_RESET GPIOPort->BRR = SHCP_PIN

HC595Class::HC595Class(GPIO_TypeDef* GPIOx, uint16_t DS, uint16_t OE,
		uint16_t STCP, uint16_t SHCP) {
	GPIOPort = GPIOx;
	DS_PIN = DS;
	OE_PIN = OE;
	STCP_PIN = STCP;
	SHCP_PIN = SHCP;
}

void HC595Class::Init() {
	GPIOInit();
	Disable();
}

void HC595Class::GPIOInit() {
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
	GPIO_InitStructure.GPIO_Pin = DS_PIN | OE_PIN | STCP_PIN | SHCP_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOPort, &GPIO_InitStructure);
}

void HC595Class::Write(uint32_t data, uint8_t len) {
	STCP_RESET;	//����Ϊ�͵�ƽ �Ա���������ظ�������
	Delay();
	for (uint32_t mask = 1 << --len; mask != 0; mask >>= 1) {
		SHCP_RESET; ////����Ϊ�͵�ƽ �Ա������������λ����  Qn>>Q(n+1)
		Delay();
		if ((mask & data) != 0) {
			DS_SET;
		} else {
			DS_RESET;
		}
		Delay();
		SHCP_SET;
		Delay();
	}
	STCP_SET;
	Delay();
	Enable();
}

inline void HC595Class::Disable() {
	OE_SET;	//�������
}

inline void HC595Class::Enable() {
	OE_RESET; //ʹ�����
}

inline void HC595Class::Delay() {
	__NOP();
	__NOP();
	__NOP();
}
