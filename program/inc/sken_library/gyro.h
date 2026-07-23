/*
 * gyro.h
 *
 *  Created on: 2019/09/14
 *      Author: TakumaNakao
 */

#ifndef GYRO_H_
#define GYRO_H_

#include "system.h"
#include "uart.h"

enum Coordinate{
	ROLL,
	PITCH,
	YAW
};

struct gyro_data_t{
	struct{
		double angle;
		double rate;
		int16_t acceleration;
	}roll;
	struct{
		double angle;
		double rate;
		int16_t acceleration;
	}pitch;
	struct{
		double angle;
		double rate;
		int16_t acceleration;
	}yaw;
};

class Gyro
{
public:
	void init(Pin tx_pin,Pin rx_pin,UartNumber uart_num,int connect_dead_time = 1000);
	bool update(void);
	bool resetAngle(Coordinate coordinate);
	bool getConnectFlag(void);
	gyro_data_t getData(void);
private:
	Uart serial;
	uint8_t receive_data_[52];
	gyro_data_t gyro_data_;
	int dead_time_;
	bool connect_check_flag_;
	double roll_angle_sensor_value_;
	double pitch_angle_sensor_value_;
	double yaw_angle_sensor_value_;
	double roll_angle_offset_;
	double pitch_angle_offset_;
	double yaw_angle_offset_;
	void angleValueSet(void);
};



#endif /* GYRO_H_ */
