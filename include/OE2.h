/*
 * OE2.h
 *
 *  Created on: 2017年6月30日
 *      Author: Romeli
 */

#ifndef OE2_H_
#define OE2_H_

#include "cmsis_device.h"

class OE2 {
public:
	static int16_t ExCNT;
	static void Init();
	static int32_t GetPos();
	static void SetPos(int32_t pos);
private:
	static void GPIOInit();
	static void TIMInit();
	static void NVICInit();
};

#endif /* OE2_H_ */
