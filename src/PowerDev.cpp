/*
 * PowerDev.cpp
 *
 *  Created on: 2017年7月4日
 *      Author: Romeli
 */

#include "PowerDev.h"
#include "HC595.h"

#define MotorFre 45000;

HC595Class HC595PowerDev = HC595Class(GPIOE, GPIO_Pin_11, GPIO_Pin_14,
GPIO_Pin_12, GPIO_Pin_13);

volatile uint32_t PowerDev::Status = 0x00000000;
bool PowerDev::Busy;

void PowerDev::Init() {
	HC595PowerDev.Init();
	HC595PowerDev.Write(Status, 24);
	GPIOInit();
	TIMInit();
}

void PowerDev::RefreshData() {
	HC595PowerDev.Write(Status, 24);
}

void PowerDev::Valve(uint32_t status) {
	Busy = true;
	Status &= MotorMask;
	Status = (status & ValveCh_Mask);
	RefreshData();
	Busy = false;
}

void PowerDev::Motor(int16_t speed) {
	Busy = true;
	if (speed > 0) {
		Status |= MotorMask;
	} else {
		speed = -speed;
		Status &= (~MotorMask);
	}
	TIM3->CCR4 = speed;
	RefreshData();
	Busy = false;
}

void PowerDev::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

//	GPIO_ResetBits(GPIOC, GPIO_Pin_9);
}

void PowerDev::TIMInit() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	TIM_DeInit(TIM3);
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInitStructure.TIM_Period = 30000;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能
	TIM_OCInitStructure.TIM_Pulse = 0; //脉冲宽度
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //高电平有效
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //低电平
	TIM_OC4Init(TIM3, &TIM_OCInitStructure); //初始化

	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_Cmd(TIM3, ENABLE);
}
