/*
 * Protocol.cpp
 *
 *  Created on: 2017年7月5日
 *      Author: Romeli
 */

#include "Protocol.h"
#include "LED.h"

#define ADD_OFF 2
#define CMD_OFF 3
#define LEN_OFF 4
#define LEN2_OFF 5
#define DATA_OFF 6

uint8_t Protocol::DevAdd = 1;
P_Buf_Typedef Protocol::P_Rcv;
P_Buf_Typedef Protocol::P_Run;
P_Buf_Typedef Protocol::P_Run2;

void U_USART3_Event() {
	P_Buf_Typedef databuf;
	LED::Turn(Color_Blue);
	PA_Typedef result = Protocol::Analysis(&databuf);
	if (result == PA_Ok) {
		Protocol::P_Rcv.pc = (PC_Typedef) databuf.data[CMD_OFF];
		Protocol::P_Rcv.len = databuf.data[LEN_OFF];
		for (uint8_t i = DATA_OFF; i < Protocol::P_Rcv.len + DATA_OFF; ++i) {
			Protocol::P_Rcv.data[i - DATA_OFF] = databuf.data[i];
		}
		Protocol::P_Rcv.flag = true;
		Protocol::Send(PC_Post_Get, databuf.data[LEN_OFF], Protocol::P_Rcv.pc,
				Protocol::P_Rcv.data);
	} else {
		U_USART3.println("error");
	}
	U_USART3.clear();
}

void Protocol::Init() {
	Protocol::P_Rcv.flag = false;
	Protocol::P_Run.flag = false;
	Protocol::P_Run2.flag = false;
}

_PA_Typedef Protocol::Analysis(P_Buf_Typedef* databuf) {
	WordtoByte_Typedef datalen;

	databuf->len = U_USART3.available();
	if (databuf->len > 64) {
		return PA_FrameError;
	}
	U_USART3.read(databuf->data, databuf->len);
	datalen.byte[0] = databuf->data[LEN_OFF];
	datalen.byte[1] = databuf->data[LEN2_OFF];
	if (databuf->data[0] != '\r' || databuf->data[1] != '\n') {
		return PA_FrameError;
	}
	if (databuf->len != DATA_OFF + datalen.word + 1) { //起始位+地址+命令+长度位+数据+校验
		return PA_FrameError;
	}

	if (databuf->data[ADD_OFF] != DevAdd) {
		return PA_AddError;
	}
	uint8_t sum = 0;
	for (uint8_t i = ADD_OFF; i < DATA_OFF + datalen.word; ++i) {
		sum += databuf->data[i];
	}
	if (sum != (databuf->data[DATA_OFF + datalen.word])) {
		return PA_CheckSumError;
	}
	return PA_Ok;
}

void Protocol::Send(PC_Typedef com, uint16_t datalen, uint8_t com_get,
		uint8_t* data) {
	DataBuf_Typedef sendbuf;
	uint16_t index = 0;
	uint8_t sum = 0;

	datalen += 1;	//收到的指令字节
	sendbuf.data[index++] = '\r';
	sendbuf.data[index++] = '\n'; //填充起始字节
	sendbuf.data[index++] = DevAdd; //填充从机地址 
	sendbuf.data[index++] = com; //填充指令字节
	sendbuf.data[index++] = (uint8_t) datalen;	//填充数据长度字节（指令字节+数据长度）
	sendbuf.data[index++] = (uint8_t) (datalen >> 8);	//填充数据长度字节（指令字节+数据长度）
	sum = DevAdd + com + (uint8_t) datalen + (uint8_t) (datalen >> 8);//先累加地址字节、指令字节和数据长度字节

	for (uint16_t i = 0; i < datalen; ++i) {	//循环累加数据位，并填充发送缓冲
		if (i == 0) {
			sendbuf.data[index++] = com_get;	//填充收到的指令
			sum += com_get;	//累加校验
			continue;
		}
		sendbuf.data[index++] = *(data + i - 1); //减去收到指令字节的偏移
		sum += sendbuf.data[index - 1];
	}
	sendbuf.data[index++] = sum;	//sum
	//起始字节2+地址字节1+指令字节1+数据长度字节2+数据长度+校验
	sendbuf.len = (2 + 1 + 1 + 2 + datalen + 1);
	U_USART3.print(sendbuf.data, sendbuf.len);
}

void Protocol::Send(Salve_Typedef salve, PC_Typedef com, uint16_t datalen,
		uint8_t* data) {
	DataBuf_Typedef sendbuf;
	uint16_t index = 0;
	uint8_t sum = 0;

	sendbuf.data[index++] = com; //填充指令字节
	sendbuf.data[index++] = (uint8_t) datalen;	//填充数据长度字节（指令字节+数据长度）
	sendbuf.data[index++] = (uint8_t) (datalen >> 8);	//填充数据长度字节（指令字节+数据长度）
	sum = com + (uint8_t) datalen + (uint8_t) (datalen >> 8);	//指令字节和数据长度字节
	for (uint16_t i = 0; i < datalen; ++i) {	//循环累加数据位，并填充发送缓冲
		sendbuf.data[index++] = *(data + i);
		sum += sendbuf.data[index - 1];
	}
	sendbuf.data[index++] = sum;	//sum
	//指令字节1+数据长度字节2+数据长度+校验
	sendbuf.len = (1 + 2 + datalen + 1);

	SPIBUS::Select(salve, ENABLE);
	U_SPI2::SendSync(sendbuf.data, sendbuf.len);
	SPIBUS::Select(salve, DISABLE);
}
