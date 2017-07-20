/*
 * LED.h
 *
 *  Created on: 2017年7月10日
 *      Author: Romeli
 */

#ifndef LED_H_
#define LED_H_

#include "cmsis_device.h"

typedef enum _Color_Typedef {
	Color_None, Color_Red, Color_Blue, Color_Green
} Color_Typedef;

class LED {
public:
	static void Init();
	static void Turn(Color_Typedef color);
private:
	static void GPIOInit();
};

#endif /* LED_H_ */
