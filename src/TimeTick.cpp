/*
 * TimeTick.cpp
 *
 *  Created on: 2017��1��11��
 *      Author: Romeli
 */

#include "TimeTick.h"

bool TimeTick::ThreadStart = false;

void TimeTick::Init(uint16_t ms) {
	TIMInit(ms);
	NVICInit();
}

void TimeTick::TIMInit(uint16_t ms) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock >> 1 / 1000;
	TIM_TimeBaseStructure.TIM_Period = ms * 2;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
}

void TimeTick::NVICInit() {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM2, ENABLE);
}

void __attribute__((weak)) TimeTickISR() {

}

extern "C" void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TimeTickISR();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
