/*
 * encoder.h
 *
 *  Created on: 2019/09/03
 *      Author: TakumaNakao
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include "io_name.h"

struct Encoder_data{
	int count;
	double rot,deg,distance,volcity,rps;
};

class Encoder{
public:
	void init(Pin a_pin,Pin b_pin,TimerNumber tim_num,double diameter = 100,int ppr = 8192,int period = 1);
	int read();
	void interrupt(Encoder_data* encoder_data);
	void reset(void);
private:
	GPIO_TypeDef* a_pin_group_;
	GPIO_TypeDef* b_pin_group_;
	TIM_TypeDef* encoder_tim_;
	GPIO_InitTypeDef gpio_init_encoder_a_;
	GPIO_InitTypeDef gpio_init_encoder_b_;
	TIM_HandleTypeDef encoder_handle_;
	TIM_Encoder_InitTypeDef encoder_init_;
	TIM_MasterConfigTypeDef encoder_mas_config_;
	int ppr_,diameter_,period_,limit;
	double before_rot_;
};

#endif /* ENCODER_H_ */
