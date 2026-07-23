/*
 * debugger.h
 *
 *  Created on: 2019/10/14
 *      Author: TakumaNakao
 */

#include "system.h"
#include "uart.h"

#ifndef RM_LIBRARY_DEBUGGER_H_
#define RM_LIBRARY_DEBUGGER_H_

class Debugger
{
public:
	void init(Pin tx_pin,Pin rx_pin,UartNumber uart_num);
	void sendDebugData(float data1,float data2);
private:
	Uart serial;
};

#endif /* RM_LIBRARY_DEBUGGER_H_ */
