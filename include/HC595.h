/*
 * HC595.h
 *
 *  Created on: 2016��12��24��
 *      Author: Romeli
 *      PinMap:	|DS-----PC5|��������
 *      		|OE-----PE7|�͵�ƽ��Ч
 *      		|STCP---PC6|�洢�Ĵ�����ʱ������
 *      		|SHCP---PC7|��λ�Ĵ�����ʱ������
 */

#ifndef HC595_H_
#define HC595_H_

#include "stm32f10x.h"

class HC595Class {
public:
	HC595Class(GPIO_TypeDef* GPIOx, uint16_t DS, uint16_t OE, uint16_t STCP,
			uint16_t SHCP);
	void Init();
	void Write(uint32_t data, uint8_t len);
	inline void Disable();
	inline void Enable();

private:
	void GPIOInit();
	inline void Delay();
	GPIO_TypeDef* GPIOPort;
	uint16_t DS_PIN;
	uint16_t OE_PIN;
	uint16_t STCP_PIN;
	uint16_t SHCP_PIN;
};

#endif /* HC595_H_ */
