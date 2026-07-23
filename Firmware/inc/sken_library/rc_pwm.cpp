/*
 * rc_pwm.cpp
 *
 *  Created on: 2019/10/21
 *      Author: TakumaNakao
 */

#include "rc_pwm.h"

void RcPwm::init(Pin pin,TimerNumber tim_num,TimerChannel tim_ch)
{
	signal.init(pin,PWM_OUTPUT,tim_num,tim_ch,50);
}

void RcPwm::turn(double percentage)
{
	signal.write(5 + 5 * (percentage / 100));
}
