/*
 * OE2.cpp
 *
 *  Created on: 2017年6月30日
 *      Author: Romeli
 */

#include <OE2.h>

#define TIM_OE TIM1
#define TIM_OE_IRQn TIM1_UP_IRQn
#define TIM_OE_RCC RCC_APB2Periph_TIM1

#define PA_PIN GPIO_Pin_6
#define PB_PIN GPIO_Pin_7
#define PI_PIN GPIO_Pin_8

int16_t OE2::ExCNT = 0;

void OE2::Init() {
	GPIOInit();
	TIMInit();
	NVICInit();

	TIM_Cmd(TIM_OE, ENABLE);
}

void OE2::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = PA_PIN | PB_PIN | PI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void OE2::TIMInit() {
	RCC_APB2PeriphClockCmd(TIM_OE_RCC, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	TIM_DeInit(TIM_OE);
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInitStructure.TIM_Period = 0xffff;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM_OE, &TIM_TimeBaseInitStructure);

	TIM_EncoderInterfaceConfig(TIM_OE, TIM_EncoderMode_TI12,
	TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);

	TIM_ICInitTypeDef TIM_ICInitStructure;

	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 1;
	TIM_ICInit(TIM_OE, &TIM_ICInitStructure);
	TIM_OE->CNT = 0;
}

int32_t OE2::GetPos() {
	return ((int32_t) ExCNT * 0x10000) + TIM_OE->CNT;
}

void OE2::SetPos(int32_t pos) {
	ExCNT = pos / 0x10000;
	TIM_OE->CNT = pos - (ExCNT * 0x10000);
}

void OE2::NVICInit() {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM_OE_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearITPendingBit(TIM_OE, TIM_IT_Update);
	TIM_ITConfig(TIM_OE, TIM_IT_Update, ENABLE);
}

extern "C" void TIM1_UP_IRQHandler(void) {
	if (TIM_OE->CNT <= 1024) {
		++OE2::ExCNT;
	} else {
		--OE2::ExCNT;
	}
	TIM_ClearITPendingBit(TIM_OE, TIM_IT_Update);
}
