/*
 * debugger.cpp
 *
 *  Created on: 2019/10/14
 *      Author: TakumaNakao
 */

#include "debugger.h"

void Debugger::init(Pin tx_pin,Pin rx_pin,UartNumber uart_num)
{
	serial.init(tx_pin,rx_pin,uart_num,9600);
}

void Debugger::sendDebugData(float data1,float data2)
{
	uint8_t send_data[14];
	ConvertIntFloat cif[2];
	cif[0].float_val = data1;
	cif[1].float_val = data2;
	send_data[0] = 100;
	send_data[13] = 200;
	for(int i = 0; i < 4; i++){
		send_data[i + 1] = (cif[0].int_val >> (i * 8)) & 0xFF;
		send_data[i + 5] = (cif[1].int_val >> (i * 8)) & 0xFF;
		send_data[i + 9] = (sken_system.millis() >> (i * 8)) & 0xFF;
	}
	serial.write(send_data,14);
}
