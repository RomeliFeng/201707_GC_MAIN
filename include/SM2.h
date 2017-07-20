/*
 * SM2.h
 *
 *  Created on: 2017年6月29日
 *      Author: Romeli
 */

#ifndef SM2_H_
#define SM2_H_

#include <SM.h>
#include "cmsis_device.h"

class SM2 {
public:
	static uint32_t TgtStep;
	static uint32_t CurStep;
	static uint32_t TgtAcc;
	static uint16_t MaxSpeed;
	static SM_DIR_Typedef CurDIR;
	static bool SpeedAcc;
	static bool NoStep;
	static bool FullStep;
	static bool Busy;
	static uint8_t ForwardLimit;
	static uint8_t BackwardLimit;

	static void Init();
	static void Move(uint32_t step, SM_DIR_Typedef dir);
	static void Run(SM_DIR_Typedef dir);
	static void Stop();
	static void SetSpeed(uint16_t speed, uint32_t tgtAcc);
	static void Unlock();
private:
	static void SetDir(SM_DIR_Typedef dir);

	static void GPIOInit();
	static void TIMInit();
	static void NVICInit();
};

#endif /* SM2_H_ */
