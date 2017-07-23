/*
 * SPIBUS.h
 *
 *  Created on: 2017年7月21日
 *      Author: Romeli
 */

#ifndef SPIBUS_H_
#define SPIBUS_H_

#include "cmsis_device.h"

#define READYTIMELIMIT 100

typedef enum _Salve_Typedef {
	Salve_FL = GPIO_Pin_6,
	Salve_CC = GPIO_Pin_8,
	Salve_MC1 = GPIO_Pin_9,
	Salve_MC2 = GPIO_Pin_10,
	Salve_AC = GPIO_Pin_11,
} Salve_Typedef;

class SPIBUS {
public:
	static void Init();
	static void Select(Salve_Typedef salve, FunctionalState newState);
	static bool Ready();
private:
	static void GPIOInit();
};

#endif /* SPIBUS_H_ */
