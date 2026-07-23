/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"
#include <cmath>

#define COMMAND_TIMEOUT_MS 500
#define m2006_P 36
#define m3508_P 19

enum Motor_Type{
    motor_none = 0,
	m2006 = 1,
	m3508 = 2
};

enum Control_Type{
	speed = 1,
	position = 2
};
			
Gpio LED[3];
long unsigned int time;

CanData can1_data,can2_data;
uint8_t debug_data[8],RoboMas_data[8];

Pid speed_pid[2][4],position_pid[2][4];

uint8_t motor_type[4],control_type[4],motor_EN[4],angle_reset[4];
uint32_t last_command_time[4];
int motor_ratio[4];
double motor_out[4],motor_rps[4],motor_rad[4],target_rps[4],target_rad[4];

void RoboMas_turn()
{
    static uint16_t previous_rot[4];
    static int32_t total_rot[4];
    static bool initialized[4];

    static double motor_rad_offset[4];
    static uint8_t previous_angle_reset[4];

    if (can2_data.rx_stdid < 0x201 ||
        can2_data.rx_stdid > 0x204) {
        return;
    }

    int i = can2_data.rx_stdid - 0x201;

    if (motor_ratio[i] <= 0) {
        return;
    }

    uint16_t current_rot =
        ((uint16_t)can2_data.rx_data[0] << 8) |
        can2_data.rx_data[1];

    int16_t motor_rpm =
        (int16_t)(
            ((uint16_t)can2_data.rx_data[2] << 8) |
            can2_data.rx_data[3]
        );

    motor_rps[i] =
        (double)motor_rpm /
        60.0 /
        (double)motor_ratio[i];

    if (!initialized[i]) {
        previous_rot[i] = current_rot;
        total_rot[i] = 0;
        initialized[i] = true;
    } else {
        int32_t difference =
            (int32_t)current_rot -
            (int32_t)previous_rot[i];

        if (difference > 4096) {
            difference -= 8192;
        } else if (difference < -4096) {
            difference += 8192;
        }

        total_rot[i] += difference;
        previous_rot[i] = current_rot;
    }

    double raw_motor_rad =
        (double)total_rot[i] *
        2.0 * M_PI /
        8192.0 /
        (double)motor_ratio[i];

    if (angle_reset[i] == 1 &&
    	previous_angle_reset[i] == 0) {

        motor_rad_offset[i] = raw_motor_rad;

        position_pid[0][i].reset();
        position_pid[1][i].reset();
    }


    previous_angle_reset[i] = angle_reset[i];

    motor_rad[i] = raw_motor_rad - motor_rad_offset[i];
}

void rcv_command()
{
    static uint8_t motor_EN_[4];

    for (int i = 0; i < 4; i++) {
        if (can1_data.rx_stdid != (0x221 + i)) {
            continue;
        }

        uint8_t received_motor_type =
            can1_data.rx_data[0];

        uint8_t received_control_type =
            can1_data.rx_data[1];

        uint8_t received_enable =
            can1_data.rx_data[4];

        uint8_t received_angle_reset =
            can1_data.rx_data[5];

        if (received_motor_type != m2006 &&
            received_motor_type != m3508) {
            return;
        }

        if (received_control_type != speed &&
            received_control_type != position) {
            return;
        }

        if (received_enable > 1) {
            return;
        }

        int16_t target_raw =
            (int16_t)(
                ((uint16_t)can1_data.rx_data[2] << 8) |
                can1_data.rx_data[3]
            );

        motor_type[i] = received_motor_type;
        control_type[i] = received_control_type;
        motor_EN[i] = received_enable;

        angle_reset[i] =
            received_angle_reset != 0 ? 1 : 0;

        if (motor_EN[i] == 1 &&
            motor_EN_[i] == 0) {

            speed_pid[0][i].reset();
            position_pid[0][i].reset();
            speed_pid[1][i].reset();
            position_pid[1][i].reset();
        }

        motor_EN_[i] = motor_EN[i];

        if (motor_type[i] == m2006) {
            motor_ratio[i] = m2006_P;
        } else {
            motor_ratio[i] = m3508_P;
        }

        double received_target =
            (double)target_raw / 1000.0;

        if (control_type[i] == speed) {
            target_rps[i] = received_target;
        } else {
            target_rad[i] = received_target;
        }

        last_command_time[i] =
            sken_system.millis();
    }
}

void make_debug_data()
{
    for (int i = 0; i < 4; i++) {
        double error = 0.0;

        if (control_type[i] == speed) {
            error = target_rps[i] - motor_rps[i];
        }
        else if (control_type[i] == position) {
            error = target_rad[i] - motor_rad[i];
        }

        double scaled_error = error * 1000.0;

        if (scaled_error > 32767.0) {
            scaled_error = 32767.0;
        }
        else if (scaled_error < -32768.0) {
            scaled_error = -32768.0;
        }

        int16_t error_raw =
            (int16_t)scaled_error;

        debug_data[i * 2] =
            (uint8_t)((error_raw >> 8) & 0xFF);

        debug_data[i * 2 + 1] =
            (uint8_t)(error_raw & 0xFF);
    }
}

void transmit_debug()
{
    static uint32_t previous_time = 0;

    uint32_t now = sken_system.millis();

    if (now - previous_time < 10) {
        return;
    }

    previous_time = now;

    make_debug_data();

    sken_system.canTransmit(
        CAN_1,
        0x230,
        debug_data,
        8,
        0,
        stdid
    );
}

void write_LED(int num)
{
	switch (num)
	{
	case 0:
		for (int i=0;i<3;i++) LED[i].write(LOW);
		break;

	case 1:
		if (motor_EN[0] == 0){			//bright red
			for (int i=0;i<2;i++) LED[i*2].write(LOW);
			LED[1].write(HIGH);
		}else if (motor_EN[0] == 1){	//brgith red,blue,green
			if (motor_type[0] == m2006 && control_type[0] == speed){			//Green
				LED[0].write(HIGH);
				LED[1].write(LOW);
				LED[2].write(LOW);
			}else if (motor_type[0] == m2006 && control_type[0] == position){	//Yellow
				LED[0].write(HIGH);
				LED[1].write(HIGH);
				LED[1].write(HIGH);
			}else if (motor_type[0] == m3508 && control_type[0] == speed){		//Blue
				LED[0].write(LOW);
				LED[1].write(LOW);
				LED[2].write(HIGH);
			}else if (motor_type[0] == m3508 && control_type[0] == position){	//SkyBlue
				LED[0].write(HIGH);
				LED[1].write(LOW);
				LED[2].write(HIGH);
			}
		}
		break;

	case 2:
		if (motor_EN[1] == 0){			//bright red
			for (int i=0;i<2;i++) LED[i*2].write(LOW);
			LED[1].write(HIGH);
		}else if (motor_EN[1] == 1){	//brgith blue,green
			if (motor_type[1] == m2006 && control_type[1] == speed){			//Green
				LED[0].write(HIGH);
				LED[1].write(LOW);
				LED[2].write(LOW);
			}else if (motor_type[1] == m2006 && control_type[1] == position){	//Yellow
				LED[0].write(HIGH);
				LED[1].write(HIGH);
				LED[1].write(HIGH);
			}else if (motor_type[1] == m3508 && control_type[1] == speed){		//Blue
				LED[0].write(LOW);
				LED[1].write(LOW);
				LED[2].write(HIGH);
			}else if (motor_type[1] == m3508 && control_type[1] == position){	//SkyBlue
				LED[0].write(HIGH);
				LED[1].write(LOW);
				LED[2].write(HIGH);
			}
		}
		break;

	case 3:
		if (motor_EN[2] == 0){			//bright red
			for (int i=0;i<2;i++) LED[i*2].write(LOW);
			LED[1].write(HIGH);
		}else if (motor_EN[2] == 1){	//brgith blue,green
			if (motor_type[2] == m2006 && control_type[2] == speed){			//Green
				LED[0].write(HIGH);
				LED[1].write(LOW);
				LED[2].write(LOW);
			}else if (motor_type[2] == m2006 && control_type[2] == position){	//Yellow
				LED[0].write(HIGH);
				LED[1].write(HIGH);
				LED[1].write(HIGH);
			}else if (motor_type[2] == m3508 && control_type[2] == speed){		//Blue
				LED[0].write(LOW);
				LED[1].write(LOW);
				LED[2].write(HIGH);
			}else if (motor_type[2] == m3508 && control_type[2] == position){	//SkyBlue
				LED[0].write(HIGH);
				LED[1].write(LOW);
				LED[2].write(HIGH);
			}
		}
		break;
	case 4:
		if (motor_EN[3] == 0){			//bright red
			for (int i=0;i<2;i++) LED[i*2].write(LOW);
			LED[1].write(HIGH);
		}else if (motor_EN[3] == 1){	//brgith blue,green
			if (motor_type[3] == m2006 && control_type[3] == speed){			//Green
				LED[0].write(HIGH);
				LED[1].write(LOW);
				LED[2].write(LOW);
			}else if (motor_type[3] == m2006 && control_type[3] == position){	//Yellow
				LED[0].write(HIGH);
				LED[1].write(HIGH);
				LED[1].write(HIGH);
			}else if (motor_type[3] == m3508 && control_type[3] == speed){		//Blue
				LED[0].write(LOW);
				LED[1].write(LOW);
				LED[2].write(HIGH);
			}else if (motor_type[3] == m3508 && control_type[3] == position){	//SkyBlue
				LED[0].write(HIGH);
				LED[1].write(LOW);
				LED[2].write(HIGH);
			}
		}
		break;
	}
}

void check_command_timeout()
{
    uint32_t now = sken_system.millis();

    for (int i = 0; i < 4; i++) {
        if (motor_EN[i] == 1 &&
            (uint32_t)(now - last_command_time[i]) >
            COMMAND_TIMEOUT_MS) {

            motor_EN[i] = 0;

            motor_out[i] = 0.0;
            target_rps[i] = 0.0;

            speed_pid[0][i].reset();
            position_pid[0][i].reset();
            speed_pid[1][i].reset();
            position_pid[1][i].reset();
        }
    }
}

double limitValue(
    double value,
    double min_value,
    double max_value
)
{
    if (value > max_value) {
        return max_value;
    }

    if (value < min_value) {
        return min_value;
    }

    return value;
}

void interrupt()
{
    for (int i = 0; i < 4; i++) {
        double calculated_output = 0.0;

        if (motor_EN[i] == 1 &&
            motor_type[i] != motor_none) {

            int type_index = motor_type[i] - 1;

            if (control_type[i] == speed) {
                calculated_output =
                    speed_pid[type_index][i].control(
                        target_rps[i],
                        motor_rps[i],
                        1
                    );
            }
            else if (control_type[i] == position) {
                double target_speed =
                    position_pid[type_index][i].control(
                        target_rad[i],
                        motor_rad[i],
                        1
                    );

                double max_rps = 0.0;

                if (motor_type[i] == m2006) {
                    max_rps = 2.0;
                }
                else if (motor_type[i] == m3508) {
                    max_rps = 6.7;
                }

                target_speed =
                    limitValue(
                        target_speed,
                        -max_rps,
                        max_rps
                    );

                calculated_output =
                    speed_pid[type_index][i].control(
                        target_speed,
                        motor_rps[i],
                        1
                    );
            }
        }

        double output_limit = 0.0;

        if (motor_type[i] == m2006) {
            output_limit = 10000.0;
        }
        else if (motor_type[i] == m3508) {
            output_limit = 16000.0;
        }

        calculated_output =
            limitValue(
                calculated_output,
                -output_limit,
                output_limit
            );

        motor_out[i] = calculated_output;

        int16_t command =
            (int16_t)motor_out[i];

        RoboMas_data[i * 2] =
            (uint8_t)((command >> 8) & 0xFF);

        RoboMas_data[i * 2 + 1] =
            (uint8_t)(command & 0xFF);
    }

    sken_system.canTransmit(
        CAN_2,
        0x200,
        RoboMas_data,
        8,
        0,
        stdid
    );
}

int main(void)
{
	sken_system.init();

	LED[0].init(A0,OUTPUT); //Green
	LED[1].init(A1,OUTPUT); //Red
	LED[2].init(A2,OUTPUT); //Blue

	sken_system.startCanCommunicate(A12,A11,CAN_1);
	sken_system.startCanCommunicate(B13,B12,CAN_2);
	sken_system.addCanRceiveInterruptFunc(CAN_1,&can1_data);
	sken_system.addCanRceiveInterruptFunc(CAN_2,&can2_data);

	for (int i = 0; i < 4; i++) {
		speed_pid[0][i].setGain(3000,1000,1,1); 		//timeconstant = 1 rps_Gain for m2006
		position_pid[0][i].setGain(3,0.1,0,1); 	//timeconstant = 1 rps_Gain for m2006
	    speed_pid[1][i].setGain(3000, 1000, 10, 1); 	//timeconstant = 1 rps_Gain for m3508
	    position_pid[1][i].setGain(2, 0.1, 0, 1); 		//timeconstant = 1 radian_Gain for m3508
	}

	sken_system.addTimerInterruptFunc(interrupt,0,1);
	while(true){
		time = sken_system.millis()/500;
		if(time%9 == 0){
			write_LED(1);
		}else if(time%9 == 2){
			write_LED(2);
		}else if(time%9 == 4){
			write_LED(3);
		}else if(time%9 == 6){
			write_LED(4);
		}else{
			write_LED(0);
		}

		rcv_command();
		RoboMas_turn();
	    check_command_timeout();
		transmit_debug();
	}
}
