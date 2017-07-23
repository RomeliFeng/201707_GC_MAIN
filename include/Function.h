/*
 * Function.h
 *
 *  Created on: 2017年7月5日
 *      Author: Romeli
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include "cmsis_device.h"
#include "Protocol.h"
#include "Typedef.h"

class Function {
public:
	static TwoWordtoByteSigned_Typedef ASBSWAAE_Pos[512];
	static WordtoByteSigned_Typedef ASBSWAAE_ADC[512];

	static void Enter(P_Buf_Typedef* p_buf);

	static void Inquire(P_Buf_Typedef* p_buf);
	static void Control(P_Buf_Typedef* p_buf);
	static void AutoControl(P_Buf_Typedef* p_buf);
	static void Setting(P_Buf_Typedef* p_buf);
	static void Special(P_Buf_Typedef* p_buf);

	static void Inquire_Limit();
	static void Inquire_ExLimit();
	static void Inquire_Valve();
	static void Inquire_Encoder(uint8_t no);
	static void Inquire_ADC(uint8_t no);
	static void Inquire_ExADC(uint8_t no);
	static void Inquire_Tigger();
	static void Inquire_Motor();
	static void Inquire_Special(uint16_t num);
	static void Inquire_DAC(uint8_t no);
	static void Inquire_Status(uint8_t no);

	static void Control_Valve(uint32_t status);
	static void Control_Motor(int16_t speed);
	static void Control_SM(uint8_t no, uint8_t status);
	static void Control_DAC(uint8_t no, uint16_t data);

	static void AutoControl_SM_By_Step(uint8_t no, int32_t step);
	static void AutoControl_SM_By_Limit(uint8_t no, uint8_t status,
			uint8_t limitNo);
	static bool AutoControl_SM_By_Limit_Judge(uint8_t limitNo);
	static void AutoControl_SM_By_Step_With_ADC_And_Encoder(uint8_t no,
			int32_t step, uint8_t encoderNo, uint8_t adcNo, uint16_t num);

	static void Setting_SM_Speed(uint8_t no, uint16_t speed, uint32_t tgtAcc);
	static void Setting_Valve_Default(uint32_t status);
	static void Setting_Encoder_Zero(uint8_t no);
	static void Setting_Protect_Limit(uint8_t no, uint8_t status,
			uint8_t limitNo);
	static void Setting_PIDParam(uint8_t no, DoubletoByte_Typedef p,
			DoubletoByte_Typedef i, DoubletoByte_Typedef d,
			DoubletoByte_Typedef set);
	static void Setting_PIDInput(uint8_t no, DoubletoByte_Typedef now);
	static void Setting_USART(uint8_t com);
	static void Setting_Address(uint8_t add);

	static void Special_Mask();
	static void Special_Reset();
	static void Special_Stop();
	static void Special_Continue();
	static void Special_Cacel();
private:
};

#endif /* FUNCTION_H_ */
