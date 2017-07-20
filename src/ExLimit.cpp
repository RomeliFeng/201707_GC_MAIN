/*
 * ExLimit.cpp
 *
 *  Created on: 2017年7月3日
 *      Author: Romeli
 */

#include <ExLimit.h>
#include "HC165.h"

HC165Class HC165Limit = HC165Class(GPIOB, GPIO_Pin_5, GPIO_Pin_4, GPIO_Pin_3,
GPIO_Pin_9);

TwoWordtoByte_Typedef ExLimit::Data;
bool ExLimit::Busy = false;

void ExLimit::Init() {
	HC165Limit.Init();
	RefreshData();
}

void ExLimit::RefreshData() {
	Busy = true;
	Data.twoword = ~HC165Limit.Read(24);
	Busy = false;
}
