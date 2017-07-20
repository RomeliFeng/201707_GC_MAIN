/*
 * ExADC.cpp
 *
 *  Created on: 2017年7月3日
 *      Author: Romeli
 */

#include <ExADC.h>

#define DOUTB_PIN GPIO_Pin_5
#define DOUTA_PIN GPIO_Pin_4
#define CONVST_PIN GPIO_Pin_3
#define SCLK_PIN GPIO_Pin_2
#define CS_PIN GPIO_Pin_1
#define BUSY_PIN GPIO_Pin_0

#define CONVST_SET (GPIOE->BSRR = CONVST_PIN)
#define CONVST_RESET (GPIOE->BRR = CONVST_PIN)
#define SCLK_SET (GPIOE->BSRR = SCLK_PIN)
#define SCLK_RESET (GPIOE->BRR = SCLK_PIN)
#define CS_SET (GPIOE->BSRR = CS_PIN)
#define CS_RESET (GPIOE->BRR = CS_PIN)

#define DOUTA_READ (GPIOE->IDR & DOUTA_PIN)
#define DOUTB_READ (GPIOE->IDR & DOUTB_PIN)
#define BUSY_READ  (GPIOE->IDR & BUSY_PIN)

WordtoByte_Typedef ExADC::Data[8];

void ExADC::Init() {
	GPIOInit();
	CS_SET;
	CONVST_RESET;
	SCLK_SET;

	RefreshData();
}

void ExADC::RefreshData() {
	//高电平脉冲，开始转换信号
	CONVST_SET;
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	CONVST_RESET;
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();

	//等待转换结束
	while (BUSY_READ != 0)
		;

	SCLK_SET;
	__NOP();
	__NOP();
	__NOP();
	CS_RESET;
	__NOP();
	__NOP();
	__NOP();

	for (uint8_t i = 0; i < 1; i++) {
		for (uint16_t j = 0x8000; j != 0; j >>= 1) {
			SCLK_RESET;
			__NOP();
			if (DOUTA_READ != 0) {
				Data[i].word |= j;
			} else {
				Data[i].word &= ~(j);
			}
			if (DOUTB_READ != 0) {
				Data[i + 4].word |= j;
			} else {
				Data[i + 4].word &= ~(j);
			}
			SCLK_SET;
			__NOP();
			__NOP();
			__NOP();
			__NOP();
		}
	}
	CS_SET;
}

void ExADC::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = CONVST_PIN | SCLK_PIN | CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = DOUTA_PIN | DOUTB_PIN | BUSY_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
