/*
 * TimeTick.h
 *
 *  Created on: 2017��1��11��
 *      Author: Romeli
 */

#ifndef TIMETICK_H_
#define TIMETICK_H_

#include "cmsis_device.h"

class TimeTick {
public:
	static bool ThreadStart;

	static void Init(uint16_t ms);
private:
	static void TIMInit(uint16_t ms);
	static void NVICInit();
};

extern void TimeTickISR();

#endif /* TIMETICK_H_ */
