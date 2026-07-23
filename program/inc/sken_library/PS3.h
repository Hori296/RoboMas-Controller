/*
 * PS3.h
 *
 *  Created on: 2021/12/17
 *      Author: hotaka
 */

#ifndef PS3_H_
#define PS3_H_

#include "sken_library/include.h"

struct PS3_data{
	int Up,Down,Right,Left,Triangle,Cross,Circle,Square,L1,L2,R1,R2,Start,Select;
	int LxPad,LyPad,RxPad,RyPad;
	uint8_t rx_data[8];
};

class PS3 {
	Uart serial;
	uint8_t rx_data_[8];
public:
	PS3();
	void StartRecive(Pin tx_pin = A9,Pin rx_pin = A10,UartNumber uart_num = SERIAL1);
	bool Getdata(PS3_data* ps3_data);
};

#endif /* PS3_H_ */
