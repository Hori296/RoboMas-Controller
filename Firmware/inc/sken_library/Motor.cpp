/*
 * Motor.cpp
 *
 *  Created on: 2021/06/23
 *      Author: hotaka
 */

/*
 * モーターを制御するクラス
*/

#include "Motor.h"

Motor::Motor(){}

void Motor::init(Pin pin1,Pin pinP,Pin pin2,TimerNumber timer,TimerChannel ch){
	mtr_name_ = ShibaDriver;
	motor[0].init(pin1,OUTPUT);
	motor[1].init(pinP,PWM_OUTPUT,timer,ch,980);
	motor[2].init(pin2,OUTPUT);
}

void Motor::init(MtrPin mtr_pin,Pin pin,TimerNumber timer,TimerChannel ch){
	mtr_name_ = KuroDriver;
	motor[mtr_pin].init(pin,PWM_OUTPUT,timer,ch,50000);
}

void Motor::write(int val){
	if(mtr_name_ == ShibaDriver){
		if(val == 0){
			motor[0].write(LOW);
			motor[1].write(val);
			motor[2].write(LOW);
		}else if(val > 0){
			motor[0].write(HIGH);
			motor[1].write(val);
			motor[2].write(LOW);
		}else if(val < 0){
			motor[0].write(LOW);
			motor[1].write(-1*val);
			motor[2].write(HIGH);
		}
	}else if(mtr_name_ == KuroDriver){
		if(val == 0){
			motor[0].write(0);
			motor[1].write(0);
		}else if(val > 0){
			motor[0].write(val);
			motor[1].write(0);
		}else if(val < 0){
			motor[0].write(0);
			motor[1].write(-1*val);
		}else if(val > 99){
			motor[0].write(99);
			motor[1].write(0);
		}else if(val < -99){
			motor[0].write(0);
			motor[1].write(-1*val);
		}
	}
}

void Motor::stop(){
	if(mtr_name_ ==  ShibaDriver){
		motor[0].write(HIGH);
		motor[1].write(0);
		motor[2].write(HIGH);
	}else if(mtr_name_ == KuroDriver){
		motor[0].write(0);
		motor[1].write(0);
	}
}
