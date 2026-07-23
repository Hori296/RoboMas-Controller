/*
 * PS4.h
 *
 *  Created on: 2023/05/21
 *      Author: p1ing
 */

#ifndef PS4_H_
#define PS4_H_

#include "sken_library/include.h"

struct PS4_data{
	int Up,Down,Right,Left,Triangle,Cross,Circle,Square,L1,L2,R1,R2,Share,Options,Ps,Pad;
	int LxPad,LyPad,RxPad,RyPad;
	uint8_t rx_data[8];
};

class PS4 {
	Uart serial;
	uint8_t rx_data_[8];
public:
	PS4();
	void StartRecive(Pin tx_pin,Pin rx_pin,UartNumber uart_num);
	bool Getdata(PS4_data* ps4_data);
};

#endif /* PS4_H_ */
