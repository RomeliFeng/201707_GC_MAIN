/*
 * SM1.cpp
 *
 *  Created on: 2017年6月29日
 *      Author: Romeli
 */

#include <SM1.h>

#define TIM_ACC TIM7
#define TIM_ACC_IRQn TIM7_IRQn
#define TIM_ACC_RCC RCC_APB1Periph_TIM7
#define TIM_PUL TIM8
#define TIM_PUL_IRQn TIM8_UP_IRQn
#define TIM_PUL_RCC RCC_APB2Periph_TIM8

#define PUL_PIN GPIO_Pin_6
#define DIR_PIN GPIO_Pin_7
#define EN_PIN GPIO_Pin_8

#define DIR_SET GPIO_SetBits(GPIOC, DIR_PIN);
#define DIR_RESET GPIO_ResetBits(GPIOC, DIR_PIN);
#define EN_SET GPIO_SetBits(GPIOC, EN_PIN);
#define EN_RESET GPIO_ResetBits(GPIOC, EN_PIN);

uint32_t SM1::TgtStep = 10000;
uint32_t SM1::CurStep = 0;
uint32_t SM1::TgtAcc = 200000;
uint16_t SM1::MaxSpeed = 20000;
SM_DIR_Typedef SM1::CurDIR = SM_DIR_Forward;
bool SM1::SpeedAcc = true;
bool SM1::NoStep = false;
bool SM1::FullStep = false;
bool SM1::Busy = false;
TIM_TypeDef* SM1::TIM_Acc = TIM_ACC;
TIM_TypeDef* SM1::TIM_Pul = TIM_PUL;

uint8_t SM1::ForwardLimit = 0x01;
uint8_t SM1::BackwardLimit = 0x02;

void SM1::Init() {
	GPIOInit();
	TIMInit();
	NVICInit();
}

void SM1::Move(uint32_t step, SM_DIR_Typedef dir) {
	TIM_Cmd(TIM_ACC, DISABLE);
	TIM_Cmd(TIM_PUL, DISABLE);

	Limit::RefreshData();
	switch (dir) {
	case SM_DIR_Forward:
		if ((Limit::Data & ForwardLimit) != 0) {
			return;
		}
		break;
	case SM_DIR_Backward:
		if ((Limit::Data & BackwardLimit) != 0) {
			return;
		}
		break;
	default:
		break;
	}

	EN_RESET
	;
	//设置方向
	SetDir(dir);

	NoStep = false;
	CurStep = 0;
	TgtStep = step;
	Busy = true;

	if (SpeedAcc) {
		FullStep = false;

		//设置加速度定时器当前速度为200
		TIM_ACC->CNT = 200;

		//开始计算初始速度
		TIM_PUL->ARR = SystemCoreClock / TIM_ACC->CNT >> 3; //默认八分频
		TIM_PUL->CCR1 = TIM_PUL->ARR >> 1;

		//开始
		TIM_Cmd(TIM_ACC, ENABLE);
		TIM_Cmd(TIM_PUL, ENABLE);
	} else {
		FullStep = true;

		//设置速度；不加速
		TIM_ACC->CNT = MaxSpeed;

		//开始计算初始速度
		TIM_PUL->ARR = SystemCoreClock / TIM_ACC->CNT >> 3; //默认八分频
		TIM_PUL->CCR1 = TIM_PUL->ARR >> 1;

		//开始 无加速过程
//		TIM_Cmd(TIM_ACC, ENABLE);
		TIM_Cmd(TIM_PUL, ENABLE);
	}
}

void SM1::Run(SM_DIR_Typedef dir) {
	TIM_Cmd(TIM_ACC, DISABLE);
	TIM_Cmd(TIM_PUL, DISABLE);

	Limit::RefreshData();
	switch (dir) {
	case SM_DIR_Forward:
		if ((Limit::Data & ForwardLimit) != 0) {
			return;
		}
		break;
	case SM_DIR_Backward:
		if ((Limit::Data & BackwardLimit) != 0) {
			return;
		}
		break;
	default:
		break;
	}

	EN_RESET
	;
	//设置方向
	SetDir(dir);

	NoStep = true;
	Busy = true;

	if (SpeedAcc) {
		FullStep = false;

		//设置加速度定时器当前速度为200
		TIM_ACC->CNT = 200;

		//开始计算初始速度
		TIM_PUL->ARR = SystemCoreClock / TIM_ACC->CNT >> 3; //默认八分频
		TIM_PUL->CCR1 = TIM_PUL->ARR >> 1;

		//开始
		TIM_Cmd(TIM_ACC, ENABLE);
		TIM_Cmd(TIM_PUL, ENABLE);
	} else {
		FullStep = true;

		//设置速度；不加速
		TIM_ACC->CNT = MaxSpeed;

		//开始计算初始速度
		TIM_PUL->ARR = SystemCoreClock / TIM_ACC->CNT >> 3; //默认八分频
		TIM_PUL->CCR1 = TIM_PUL->ARR >> 1;

		//开始 无加速过程
//		TIM_Cmd(TIM_ACC, ENABLE);
		TIM_Cmd(TIM_PUL, ENABLE);
	}
}

void SM1::Stop() {
	TIM_Cmd(TIM_ACC, DISABLE);
	TIM_Cmd(TIM_PUL, DISABLE);
	Busy = false;
}

void SM1::SetSpeed(uint16_t speed, uint32_t tgtAcc) {
	if (tgtAcc < 1500) {
		SpeedAcc = false;
	} else {
		SpeedAcc = true;
		TgtAcc = tgtAcc;
	}
	MaxSpeed = speed < 200 ? 200 : speed;
	TIM_Cmd(TIM_ACC, DISABLE);
	TIM_ACC->PSC = SystemCoreClock / TgtAcc - 1;
	TIM_ACC->ARR = MaxSpeed;
	TIM_Cmd(TIM_ACC, ENABLE);
}

void SM1::Unlock() {
	EN_SET
	;
}

void SM1::SetDir(SM_DIR_Typedef dir) {
	CurDIR = dir;
	if (dir == SM_DIR_Forward) {
		DIR_SET
		;
	} else {
		DIR_RESET
		;
	}
}

void SM1::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = PUL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = DIR_PIN | EN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

//	GPIO_SetBits(GPIOC, EN_PIN);
}

void SM1::TIMInit() {
	RCC_APB2PeriphClockCmd(TIM_PUL_RCC, ENABLE);
	RCC_APB1PeriphClockCmd(TIM_ACC_RCC, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	TIM_DeInit(TIM_ACC);
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = SystemCoreClock / TgtAcc - 1; //根据加速度计算
	TIM_TimeBaseInitStructure.TIM_Period = MaxSpeed; //计数器的值为当前速度
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM_ACC, &TIM_TimeBaseInitStructure);

	//初始化脉冲发生定时器
	TIM_DeInit(TIM_PUL);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7;  //8分频 最低速度为 138
	TIM_TimeBaseInitStructure.TIM_Period = 36000; //通过当前加速度计算周期
	TIM_TimeBaseInit(TIM_PUL, &TIM_TimeBaseInitStructure);

	TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; //关闭互补输出
	TIM_OCInitStructure.TIM_Pulse = MaxSpeed / TIM_ACC->CNT >> 1; //脉冲宽度
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //高电平有效
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //低电平有效
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //低电平
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //低电平
	TIM_OC1Init(TIM_PUL, &TIM_OCInitStructure); //初始化
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
}

void SM1::NVICInit() {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM_ACC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM_PUL_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearITPendingBit(TIM_ACC, TIM_IT_Update);
	TIM_ClearITPendingBit(TIM_PUL, TIM_IT_Update);
	TIM_ITConfig(TIM_ACC, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM_PUL, TIM_IT_Update, ENABLE);
}

extern "C" {

void TIM7_IRQHandler(void) {
	TIM_ACC->CR1 &= (uint16_t) (~((uint16_t) TIM_CR1_CEN)); //关闭ACC累加
	TIM_ACC->CNT = SM1::MaxSpeed;
	TIM_ClearITPendingBit(TIM_ACC, TIM_IT_Update);
}

void TIM8_UP_IRQHandler(void) {
	SM1::CurStep++;
	if ((SM1::CurStep < SM1::TgtStep) || SM1::NoStep) { //预定步数未到，继续累加
		if (SM1::FullStep == false) {
			if (TIM_ACC->CNT == SM1::MaxSpeed) {
				SM1::FullStep = true;
			} else {
				TIM_PUL->ARR = SystemCoreClock / TIM_ACC->CNT >> 3;
				TIM_PUL->CCR1 = TIM_PUL->ARR >> 1;
			}
		} else {
			TIM_PUL->ARR = SystemCoreClock / SM1::MaxSpeed >> 3;
			TIM_PUL->CCR1 = TIM_PUL->ARR >> 1;
		}
	} else {
		TIM_PUL->CR1 &= (uint16_t) (~((uint16_t) TIM_CR1_CEN)); //到达指定步数，停止输出脉冲
		TIM_ACC->CR1 &= (uint16_t) (~((uint16_t) TIM_CR1_CEN)); //到达指定步数，停止计算加速度
		SM1::FullStep = false;
		SM1::Busy = false;
	}
	TIM_ClearITPendingBit(TIM_PUL, TIM_IT_Update);
}

}

