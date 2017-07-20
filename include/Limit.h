/*
 * Limit.h
 *
 *  Created on: 2017年7月3日
 *      Author: Romeli
 */

#ifndef LIMIT_H_
#define LIMIT_H_

#include "cmsis_device.h"

class Limit {
public:
	static uint8_t Data;
	static void Init();
	static void RefreshData();
private:
	static void GPIOInit();
};


#endif /* LIMIT_H_ */
