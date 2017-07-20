/*
 * PowerDev.h
 *
 *  Created on: 2017年7月4日
 *      Author: Romeli
 */

#ifndef POWERDEV_H_
#define POWERDEV_H_

#include "cmsis_device.h"

#define MotorMask 0x00800000

typedef enum _ValveCh_Typedef {
	ValveCh_0 = 0x00000001,
	ValveCh_1 = 0x00000002,
	ValveCh_2 = 0x00000004,
	ValveCh_3 = 0x00000008,
	ValveCh_4 = 0x00000010,
	ValveCh_5 = 0x00000020,
	ValveCh_6 = 0x00000040,
	ValveCh_7 = 0x00000080,
	ValveCh_8 = 0x00000100,
	ValveCh_9 = 0x00000200,
	ValveCh_10 = 0x00000400,
	ValveCh_11 = 0x00000800,
	ValveCh_12 = 0x00001000,
	ValveCh_13 = 0x00002000,
	ValveCh_14 = 0x00004000,
	ValveCh_15 = 0x00008000,
	ValveCh_16 = 0x00010000,
	ValveCh_17 = 0x00020000,
	ValveCh_18 = 0x00040000,
	ValveCh_19 = 0x00080000,
	ValveCh_20 = 0x00100000,
	ValveCh_21 = 0x00200000,
	ValveCh_22 = 0x00400000,
	ValveCh_Mask = 0x007fffff
} ValveCh_Typedef;

class PowerDev {
public:
	volatile static uint32_t Status;
	static bool Busy;

	static void Init();
	static void RefreshData();
	static void Valve(uint32_t status);
	static void Motor(int16_t speed);
private:
	static void GPIOInit();
	static void TIMInit();
};

#endif /* POWERDEV_H_ */
