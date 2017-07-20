/*
 * ExADC.h
 *
 *  Created on: 2017年7月3日
 *      Author: Romeli
 */

#ifndef EXADC_H_
#define EXADC_H_

#include "cmsis_device.h"
#include "Typedef.h"

class ExADC {
public:
	static WordtoByte_Typedef Data[8];
	static void Init();
	static void RefreshData();
private:
	static void GPIOInit();
};

#endif /* EXADC_H_ */
