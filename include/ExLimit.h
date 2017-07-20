/*
 * ExLimit.h
 *
 *  Created on: 2017骞�7鏈�3鏃�
 *      Author: Romeli
 */

#ifndef EXLIMIT_H_
#define EXLIMIT_H_

#include "cmsis_device.h"
#include "Typedef.h"

class ExLimit {
public:
	static TwoWordtoByte_Typedef Data;
	static bool Busy;

	static void Init();
	static void RefreshData();
private:

};

#endif /* EXLIMIT_H_ */
