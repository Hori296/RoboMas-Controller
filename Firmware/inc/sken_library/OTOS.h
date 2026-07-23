/*
 * OTOS.h
 *
 *  Created on: 2024/06/21
 *      Author: hhhot
 */

#ifndef SKEN_LIBRARY_OTOS_H_
#define SKEN_LIBRARY_OTOS_H_

#include "include.h"

class OTOS {
	I2C i2c_;
	float coefficient_[9] =
	{0.0003,0.0003,0.0055,0.00015,0.00015,0.061,0.0048,0.0048,5.5};
	uint8_t data_[2];
	int16_t Data_;
	float odom_[18],past_odom_[9],pos_x_,pos_y_;
	float drift_error_,drift_deg_,deg_error_,now_deg_,past_deg_;
	int interrupt_time_,count_=0;
public:
	OTOS();
	void init(Pin scl,Pin sda,I2CSelect i2c,int interrupt_time = 1);
	void get_odom(float* odom,int odom_num);
	void set_odom(float x,float y,float deg);
	void interrupt(float* odom);
	void offset_odom(float x,float y,float deg);
	void raw_data(uint8_t* data,int odom_num);
	void reset();
};

#endif /* SKEN_LIBRARY_OTOS_H_ */
