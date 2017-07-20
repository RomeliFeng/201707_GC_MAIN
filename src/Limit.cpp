/*
 * Limit.cpp
 *
 *  Created on: 2017年7月3日
 *      Author: Romeli
 */

#include <Limit.h>

//GPIOA
#define LIM0_PIN GPIO_Pin_5
#define LIM1_PIN GPIO_Pin_6
#define LIM2_PIN GPIO_Pin_7
#define LIM0_2MASK 0x00e0

//GPIOC
#define LIM3_PIN GPIO_Pin_4
#define LIM4_PIN GPIO_Pin_5
#define LIM3_4MASK 0x0030

//GPIOB
#define LIM5_PIN GPIO_Pin_0
#define LIM6_PIN GPIO_Pin_1
#define LIM7_PIN GPIO_Pin_2
#define Lim5_7MASK 0x0007

uint8_t Limit::Data = 0;

void Limit::Init() {
	GPIOInit();
	RefreshData();
}

void Limit::RefreshData() {
	Data = ~(((GPIOA->IDR & LIM0_2MASK) >> 5) | ((GPIOC->IDR & LIM3_4MASK) >> 1)
			| ((GPIOB->IDR & Lim5_7MASK) << 5));
}

void Limit::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(
	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LIM0_PIN | LIM1_PIN | LIM2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LIM3_PIN | LIM4_PIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LIM5_PIN | LIM6_PIN | LIM7_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
