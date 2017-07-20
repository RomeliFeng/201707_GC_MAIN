/*
 * Protect.cpp
 *
 *  Created on: 2017年7月10日
 *      Author: Romeli
 */

#include "Protect.h"

void Protect::SM() {
	if (SM1::Busy) {
		switch (SM1::CurDIR) {
		case SM_DIR_Forward:
			if ((Limit::Data & SM1::ForwardLimit) != 0) {
				SM1::Stop();
			}
			break;
		case SM_DIR_Backward:
			if ((Limit::Data & SM1::BackwardLimit) != 0) {
				SM1::Stop();
			}
			break;
		default:
			break;
		}
	}

	if (SM2::Busy) {
		switch (SM2::CurDIR) {
		case SM_DIR_Forward:
			if ((Limit::Data & SM2::ForwardLimit) != 0) {
				SM2::Stop();
			}
			break;
		case SM_DIR_Backward:
			if ((Limit::Data & SM2::BackwardLimit) != 0) {
				SM2::Stop();
			}
			break;
		default:
			break;
		}
	}
}
