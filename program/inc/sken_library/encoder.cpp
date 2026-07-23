/*
 * encoder.cpp
 *
 *  Created on: 2019/09/07
 *      Author: TakumaNakao
 */

#include "encoder.h"

void Encoder::init(Pin a_pin,Pin b_pin,TimerNumber tim_num,double diameter,int ppr,int period)
{
	a_pin_group_ = io_convert::PIN_GROUP[a_pin / 16];
	gpio_init_encoder_a_.Pin = io_convert::PIN_NUM[a_pin % 16];
	gpio_init_encoder_a_.Mode = GPIO_MODE_AF_PP;
	gpio_init_encoder_a_.Pull = GPIO_PULLUP;
	gpio_init_encoder_a_.Speed = GPIO_SPEED_HIGH;
	b_pin_group_ = io_convert::PIN_GROUP[b_pin / 16];
	gpio_init_encoder_b_.Pin = io_convert::PIN_NUM[b_pin % 16];
	gpio_init_encoder_b_.Mode = GPIO_MODE_AF_PP;
	gpio_init_encoder_b_.Pull = GPIO_PULLUP;
	gpio_init_encoder_b_.Speed = GPIO_SPEED_HIGH;

	gpio_init_encoder_a_.Alternate = io_convert::TIM_AF[tim_num];
	gpio_init_encoder_b_.Alternate = io_convert::TIM_AF[tim_num];
	encoder_tim_ = io_convert::TIM_N[tim_num];
	encoder_handle_.Instance = io_convert::TIM_N[tim_num];
	encoder_handle_.Init.Prescaler = 0;
	encoder_handle_.Init.CounterMode = TIM_COUNTERMODE_UP;
	encoder_handle_.Init.Period = 60000;
	encoder_handle_.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	encoder_init_.EncoderMode = TIM_ENCODERMODE_TI12;
	encoder_init_.IC1Polarity = TIM_ICPOLARITY_RISING;
	encoder_init_.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	encoder_init_.IC1Prescaler = TIM_ICPSC_DIV1;
	encoder_init_.IC1Filter = 0;
	encoder_init_.IC2Polarity = TIM_ICPOLARITY_RISING;
	encoder_init_.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	encoder_init_.IC2Prescaler = TIM_ICPSC_DIV1;
	encoder_init_.IC2Filter = 0;
	encoder_mas_config_.MasterOutputTrigger = TIM_TRGO_RESET;
	encoder_mas_config_.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	HAL_GPIO_Init(a_pin_group_, &gpio_init_encoder_a_);
	HAL_GPIO_Init(b_pin_group_, &gpio_init_encoder_b_);
	HAL_TIM_Encoder_Init(&encoder_handle_, &encoder_init_);
	HAL_TIMEx_MasterConfigSynchronization(&encoder_handle_, &encoder_mas_config_);
	HAL_TIM_Encoder_Start(&encoder_handle_, TIM_CHANNEL_ALL);
	encoder_tim_->CNT = 30000;

	diameter_ = diameter;
	ppr_ = ppr;
	period_ = period;
}

int Encoder::read(void)
{
	return (encoder_tim_->CNT) - 30000;
}

void Encoder::interrupt(Encoder_data* encoder_data){
	if(read() >= 20000||read() <= -20000){
		if(read() >= 20000){limit++;}else{limit--;}
		reset();
	}
	encoder_data->count = read()+limit*20000;
	encoder_data->rot = (encoder_data->count)/(double)ppr_;
	encoder_data->deg = encoder_data->rot*360.0;
	encoder_data->distance = encoder_data->rot*(PI*diameter_);

	encoder_data->rps = (double)(encoder_data->rot-before_rot_)/((double)period_*0.001);
	before_rot_ = encoder_data->rot;
	encoder_data->volcity = encoder_data->rps*PI*diameter_;
}

void Encoder::reset(void)
{
	encoder_tim_->CNT = 30000;
}
