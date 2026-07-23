/*
 * rc_pwm.h
 *
 *  Created on: 2019/10/21
 *      Author: TakumaNakao
 */

#ifndef RC_PWM_H_
#define RC_PWM_H_

#include "sken_library/gpio.h"

class RcPwm{
public:
	void init(Pin pin,TimerNumber tim_num,TimerChannel tim_ch);
	void turn(double percentage);
private:
	Gpio signal;
};



#endif /* RC_PWM_H_ */
