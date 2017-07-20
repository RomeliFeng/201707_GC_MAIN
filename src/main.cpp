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

#include "Limit.h"
#include "ExLimit.h"
#include "EEPROM.h"

#include "PowerDev.h"

#include "TimeTick.h"

#include "Protect.h"
#include "Protocol.h"
#include "Function.h"

#include "U_SPI2.h"

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
	}
}

void PeriphInit() {
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	LED::Init();
	LED::Turn(Color_Blue);

	Delay_Init(1000);
	U_USART3.begin(1024000);
	Protocol::Init();

	SM1::Init();
	SM2::Init();
	OE2::Init();
	OE4::Init();

	ExADC::Init();
	Limit::Init();
	ExLimit::Init();

	PowerDev::Init();

	TimeTick::Init(5);
}

void TimeTickISR() {
	static uint16_t count = 0;
	LED::Turn(Color_Blue);
	count++;
	Limit::RefreshData();
	Protect::SM();
	if (count >= 20) {
		ExLimit::RefreshData();
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
