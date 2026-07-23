/*
 * OTOS.cpp
 *
 *  Created on: 2024/06/21
 *      Author: hhhot
 */

#include "OTOS.h"
#include "math.h"

OTOS::OTOS() {}

void OTOS::init(Pin scl,Pin sda,I2CSelect i2c,int interrupt_time){
	i2c_.init(scl,sda,i2c,40000);
	interrupt_time_ = interrupt_time;
	reset();
}

void OTOS::get_odom(float* odom,int odom_num){
	for(int i=0; i<odom_num; i++){
		i2c_.read(0x17,0x20+(i*2)  ,1,(uint8_t*)&data_[0],1);
		i2c_.read(0x17,0x20+(i*2)+1,1,(uint8_t*)&data_[1],1);
		Data_ = data_[1]<<8 | data_[0];
		odom_[i] = Data_*coefficient_[i%9];
		odom[i] = odom_[i];
	}
}

void OTOS::set_odom(float x,float y,float deg){
	int16_t Data[3];
	Data[0] = x/coefficient_[0];
	Data[1] = y/coefficient_[1];
	Data[2] = deg/coefficient_[2];
	for(int i=0; i<3; i++){
		data_[0] =  Data[i]     & 0xFF;
		data_[1] = (Data[i]>>8) & 0xFF;
		i2c_.write(0x17,0x20+(i*2)  ,1,(uint8_t*)&data_[0],1);
		i2c_.write(0x17,0x20+(i*2)+1,1,(uint8_t*)&data_[1],1);
	}
	drift_error_ = 0;
	drift_deg_ = 0;
	pos_x_ = x;
	pos_y_ = y;
}

void OTOS::interrupt(float *odom){
	get_odom(odom_,18);
	//ドリフト削除
	drift_error_ += drift_deg_*(interrupt_time_*0.001);
	odom[2] = odom_[2]-drift_error_;
	odom[5] = odom_[5];
	odom[8] = odom_[8];

	//ドリフト値を0.1秒ごとに更新
	count_++;
	if(count_>=1000/interrupt_time_){
		count_ = 0;
		now_deg_ = odom_[2];
		deg_error_ = now_deg_ - past_deg_;
		past_deg_ = now_deg_;
		if(abs(odom_[5])<0.1 && abs(deg_error_)<0.5) drift_deg_ = deg_error_;
	}

	float dx,dy;
	//位置のドリフト成分削除
	dx =  (odom_[0]-past_odom_[0])*cos(odom_[2]*(2*PI/360.0)) + (odom_[1]-past_odom_[1])*sin(odom_[2]*(2*PI/360.0));
	dy = -(odom_[0]-past_odom_[0])*sin(odom_[2]*(2*PI/360.0)) + (odom_[1]-past_odom_[1])*cos(odom_[2]*(2*PI/360.0));
	pos_x_ += dx*cos(odom[2]*(2*PI/360.0))-dy*sin(odom[2]*(2*PI/360.0));
	pos_y_ += dx*sin(odom[2]*(2*PI/360.0))+dy*cos(odom[2]*(2*PI/360.0));
	odom[0] = pos_x_;
	odom[1] = pos_y_;

	//速度のドリフト成分削除
	dx =  odom_[3]*cos(odom_[2]*(2*PI/360.0)) + odom_[4]*sin(odom_[2]*(2*PI/360.0));
	dy = -odom_[3]*sin(odom_[2]*(2*PI/360.0)) + odom_[4]*cos(odom_[2]*(2*PI/360.0));
	odom[3] = dx*cos(odom[2]*(2*PI/360.0))-dy*sin(odom[2]*(2*PI/360.0));
	odom[4] = dx*sin(odom[2]*(2*PI/360.0))+dy*cos(odom[2]*(2*PI/360.0));

	//加速度のドリフト成分削除
	dx =  odom_[6]*cos(odom_[2]*(2*PI/360.0)) + odom_[7]*sin(odom_[2]*(2*PI/360.0));
	dy = -odom_[6]*sin(odom_[2]*(2*PI/360.0)) + odom_[7]*cos(odom_[2]*(2*PI/360.0));
	odom[6] = dx*cos(odom[2]*(2*PI/360.0))-dy*sin(odom[2]*(2*PI/360.0));
	odom[7] = dx*sin(odom[2]*(2*PI/360.0))+dy*cos(odom[2]*(2*PI/360.0));

	for(int i=0; i<9; i++) odom[i+9] = odom_[i+9];
	for(int i=0; i<9; i++) past_odom_[i] = odom_[i];
}

void OTOS::offset_odom(float x,float y,float deg){
	int16_t Data[3];
	Data[0] = x/coefficient_[0];
	Data[1] = y/coefficient_[1];
	Data[2] = deg/coefficient_[2];
	for(int i=0; i<3; i++){
		data_[0] =  Data[i]     & 0xFF;
		data_[1] = (Data[i]>>8) & 0xFF;
		i2c_.write(0x17,0x10+(i*2)  ,1,(uint8_t*)&data_[0],1);
		i2c_.write(0x17,0x10+(i*2)+1,1,(uint8_t*)&data_[1],1);
	}
}

void OTOS::raw_data(uint8_t* data,int odom_num){
	for(int i=0; i<odom_num*2; i++){
		i2c_.read(0x17,0x20+i,1,(uint8_t*)&data[i],1);
	}
}

void OTOS::reset(){
	uint8_t data=1;
	i2c_.write(0x17,0x07,1,(uint8_t*)&data,1);
}
