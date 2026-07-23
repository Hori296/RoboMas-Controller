/*
 * Motor.h
 *
 *  Created on: 2021/06/23
 *      Author: hotaka
 */

/*
 * モーターを制御するクラス
*/

#ifndef MOTOR_H_
#define MOTOR_H_

#include "sken_library/Gpio.h"

enum MtrName{
	ShibaDriver,
	KuroDriver
};

enum MtrPin{
	Apin,Bpin
};

class Motor {
public:
	Motor();
	void init(Pin pin1,Pin pinP,Pin pin2,TimerNumber timer,TimerChannel ch);
	void init(MtrPin mtr_pin,Pin pin,TimerNumber timer,TimerChannel ch);
	void write(int val);
	void stop();
private:
	Gpio motor[3];
	MtrName mtr_name_;
};

#endif /* MOTOR_H_ */
