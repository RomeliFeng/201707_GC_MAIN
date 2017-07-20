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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock / 1000;
	TIM_TimeBaseStructure.TIM_Period = ms;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
}

void TimeTick::NVICInit() {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM5, ENABLE);
}

void __attribute__((weak)) TimeTickISR() {

}

extern "C" void TIM5_IRQHandler(void) {
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) {
		TimeTickISR();
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	}
}
