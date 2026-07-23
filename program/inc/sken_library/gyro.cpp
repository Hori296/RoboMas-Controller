/*
 * gyro.cpp
 *
 *  Created on: 2019/09/14
 *      Author: TakumaNakao
 */

#include "gyro.h"

void Gyro::angleValueSet(void)
{
	if((roll_angle_sensor_value_ - roll_angle_offset_) > 180){
		gyro_data_.roll.angle = roll_angle_sensor_value_ - roll_angle_offset_ - 360;
	}
	else if((roll_angle_sensor_value_ - roll_angle_offset_) < -180){
		gyro_data_.roll.angle = roll_angle_sensor_value_ - roll_angle_offset_ + 360;
	}
	else{
		gyro_data_.roll.angle = roll_angle_sensor_value_ - roll_angle_offset_;
	}
	if((pitch_angle_sensor_value_ - pitch_angle_offset_) > 180){
		gyro_data_.pitch.angle = pitch_angle_sensor_value_ - pitch_angle_offset_ - 360;
	}
	else if((pitch_angle_sensor_value_ - pitch_angle_offset_) < -180){
		gyro_data_.pitch.angle = pitch_angle_sensor_value_ - pitch_angle_offset_ + 360;
	}
	else{
		gyro_data_.pitch.angle = pitch_angle_sensor_value_ - pitch_angle_offset_;
	}
	if((yaw_angle_sensor_value_ - yaw_angle_offset_) > 180){
		gyro_data_.yaw.angle = yaw_angle_sensor_value_ - yaw_angle_offset_ - 360;
	}
	else if((yaw_angle_sensor_value_ - yaw_angle_offset_) < -180){
		gyro_data_.yaw.angle = yaw_angle_sensor_value_ - yaw_angle_offset_ + 360;
	}
	else{
		gyro_data_.yaw.angle = yaw_angle_sensor_value_ - yaw_angle_offset_;
	}
}

void Gyro::init(Pin tx_pin,Pin rx_pin,UartNumber uart_num,int connect_dead_time)
{
	serial.init(tx_pin,rx_pin,uart_num,38400);
	serial.startDmaRead(receive_data_,52);
	connect_check_flag_ = false;
	dead_time_ = connect_dead_time;
}

bool Gyro::update(void)
{
	static int last_connect_time = 0;
	static uint8_t before_index[2] = {};
	uint8_t data[2][26];
	unsigned int check_sum[2] = {};
	for(int i = 0; i < 26; i++){
		if(receive_data_[i] == 0xA6 && receive_data_[i + 1] == 0xA6){
			for(int j = 0; j < 26; j++){
				data[0][j] = receive_data_[(j + i) % 52];
				if(j != 0 && j != 1 && j != 25){
					check_sum[0] += data[0][j];
				}
			}
			for(int j = 0; j < 26; j++){
				data[1][j] = receive_data_[(j + i + 26) % 52];
				if(j != 0 && j != 1 && j != 25){
					check_sum[1] += data[1][j];
				}
			}
			break;
		}
		if(i == 25){
			return false;
		}
	}
	if(data[0][2] != before_index[0] || data[1][2] != before_index[1]){
		last_connect_time = sken_system.millis();
		if((sken_system.millis() - last_connect_time) < dead_time_){
			connect_check_flag_ = true;
		}
		else{
			connect_check_flag_ = false;
		}
		before_index[0] = data[0][2];
		before_index[1] = data[1][2];
		for(int i = 0; i < 2; i++){
			if((check_sum[i] & 0xFF) == data[i][25]){
				roll_angle_sensor_value_ = (int16_t)(data[i][4] << 8 | data[i][3]) / 100.0;
				pitch_angle_sensor_value_ = (int16_t)(data[i][6] << 8 | data[i][5]) / 100.0;
				yaw_angle_sensor_value_ = (int16_t)(data[i][8] << 8 | data[i][7]) / 100.0;
				angleValueSet();
				gyro_data_.roll.rate = (int16_t)(data[i][10] << 8 | data[i][9]) / 100.0;
				gyro_data_.pitch.rate = (int16_t)(data[i][12] << 8 | data[i][11]) / 100.0;
				gyro_data_.yaw.rate = (int16_t)(data[i][14] << 8 | data[i][13]) / 100.0;
				gyro_data_.roll.acceleration = data[i][16] << 8 | data[i][15];
				gyro_data_.pitch.acceleration = data[i][18] << 8 | data[i][17];
				gyro_data_.yaw.acceleration = data[i][20] << 8 | data[i][19];
				break;
			}
			if(i == 1){
				return false;
			}
		}
	}
	return true;
}

bool Gyro::resetAngle(Coordinate coordinate)
{
	if(coordinate == ROLL){
		roll_angle_offset_ = roll_angle_sensor_value_;
	}
	else if(coordinate == PITCH){
		pitch_angle_offset_ = pitch_angle_sensor_value_;
	}
	else if(coordinate == YAW){
		yaw_angle_offset_ = yaw_angle_sensor_value_;
	}
	else{
		return false;
	}
	return true;
}

bool Gyro::getConnectFlag(void)
{
	return connect_check_flag_;
}

gyro_data_t Gyro::getData(void)
{
	return gyro_data_;
}
