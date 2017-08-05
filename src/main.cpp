#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#include "LED.h"

#include "Delay.h"
#include "U_USART3.h"

#include "SM1.h"
#include "SM2.h"

#include "OE2.h"
#include "OE4.h"

#include "ExADC.h"
#include "U_ADC1.h"
#include "U_DAC.h"

#include "Limit.h"
#include "ExLimit.h"
#include "EEPROM.h"

#include "PowerDev.h"

#include "TimeTick.h"

#include "Protect.h"
#include "Protocol.h"
#include "Function.h"

#include "U_SPI2.h"
#include "SPIBUS.h"

#include "PID.h"

#define SPFUN1

void PeriphInit();

int main(int argc, char* argv[]) {
	PeriphInit();
	while (1) {
		if (Protocol::P_Rcv.flag) { //新的指令到达
			LED::Turn(Color_Blue);
			Protocol::P_Run.pc = Protocol::P_Rcv.pc;
			for (uint8_t i = 0; i < Protocol::P_Rcv.len; ++i) {
				Protocol::P_Run.data[i] = Protocol::P_Rcv.data[i];
			}
			Protocol::P_Rcv.flag = false;
			Protocol::P_Run.flag = true;

			Function::Enter(&Protocol::P_Run);

		}
		LED::Turn(Color_Green);
		U_ADC1::RefreshData();
	}
}

void PeriphInit() {
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//Init Time Base
	Delay_Init(100);

	LED::Init();
	LED::Turn(Color_Blue);

	//Init Comunication
	U_USART3.begin(1024000);
	SPIBUS::Init();
	U_SPI2::Init(SPI2_Speed_9M);

	Protocol::Init();

	//Init Periph
	SM1::Init();
	SM2::Init();
	OE2::Init();
	OE4::Init();

	U_ADC1::Init();
	ExADC::Init();
	Limit::Init();
	ExLimit::Init();

	U_DAC::Init();
	U_DAC::RefreshData((uint16_t)2047);
	Function::PID.SetLimits(-2047, 2047);

	PowerDev::Init();

	TimeTick::Init(1);
}

void TimeTickISR() {
	static uint16_t count = 0;
//	static float lastOut;
//	static float k = 0.7;

	LED::Turn(Color_Blue);
	count++;
	Limit::RefreshData();
	Protect::SM();

	if (Function::PIDEnable) {
		Function::PIDParam.Input = (U_ADC1::Data.word) - 2047;
		Function::PID.Compute();

		U_DAC::RefreshData((uint16_t) (Function::PIDParam.Output + 2047));
//		U_DAC::RefreshData(
//				(uint16_t) (((Function::PIDParam.out + 2047) * (1.0 - k))
//						+ (lastOut * k)));
//		lastOut = Function::PIDParam.out + 2047;
	}
	if (count >= 100) {
		ExLimit::RefreshData();
#ifdef SPFUN1
		if ((ExLimit::Data.byte[0] & 0x04) != 0) {
			PowerDev::Status |= ValveCh_0;
		} else {
			PowerDev::Status &= (~ValveCh_0);
		}
#endif
		PowerDev::RefreshData();
		if (TimeTick::ThreadStart && Protocol::P_Rcv.flag) {
			if ((Protocol::P_Rcv.pc & PC_Mask) != PC_AutoControl_Mask) {
				Protocol::P_Run2.pc = Protocol::P_Rcv.pc;
				for (uint8_t i = 0; i < Protocol::P_Rcv.len; ++i) {
					Protocol::P_Run2.data[i] = Protocol::P_Rcv.data[i];
				}
				Protocol::P_Rcv.flag = false;
				Protocol::P_Run2.flag = true;

				Function::Enter(&Protocol::P_Run2);
			}
		}
		count = 0;
	}
}

#pragma GCC diagnostic pop
