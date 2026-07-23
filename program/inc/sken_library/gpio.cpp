/*
 * gpio.cpp
 *
 *  Created on: 2019/09/07
 *      Author: TakumaNakao
 */

#include "gpio.h"

void Gpio::init(Pin pin,Mode mode)
{
	pin_group_ = io_convert::PIN_GROUP[pin / 16];
	pin_number_ = io_convert::PIN_NUM[pin % 16];
	pin_mode_ = mode;

	gpio_init_io_.Pin = pin_number_;

	switch (pin_mode_) {
	case OUTPUT:
		gpio_init_io_.Mode = GPIO_MODE_OUTPUT_PP;
		gpio_init_io_.Pull = GPIO_NOPULL;
		gpio_init_io_.Speed = GPIO_SPEED_FREQ_HIGH;
		break;
	case INPUT:
		gpio_init_io_.Mode = GPIO_MODE_INPUT;
		gpio_init_io_.Pull = GPIO_NOPULL;
		gpio_init_io_.Speed = GPIO_SPEED_FREQ_HIGH;
		break;
	case INPUT_PULLUP:
		gpio_init_io_.Mode = GPIO_MODE_INPUT;
		gpio_init_io_.Pull = GPIO_PULLUP;
		gpio_init_io_.Speed = GPIO_SPEED_FREQ_HIGH;
		break;
	case INPUT_PULLDOWN:
		gpio_init_io_.Mode = GPIO_MODE_INPUT;
		gpio_init_io_.Pull = GPIO_PULLDOWN;
		gpio_init_io_.Speed = GPIO_SPEED_FREQ_HIGH;
		break;
	}
	HAL_GPIO_Init(pin_group_, &gpio_init_io_);
}

void Gpio::init(Pin pin,Mode mode,TimerNumber tim_num,TimerChannel tim_ch,int frequency)
{
	pin_group_ = io_convert::PIN_GROUP[pin / 16];
	pin_number_ = io_convert::PIN_NUM[pin % 16];
	pin_mode_ = mode;

	gpio_init_io_.Pin = pin_number_;
	gpio_init_io_.Mode = GPIO_MODE_AF_PP;
	gpio_init_io_.Pull = GPIO_NOPULL;
	gpio_init_io_.Speed = GPIO_SPEED_FREQ_HIGH;
	pwm_clk_config_.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	pwm_mas_config_.MasterOutputTrigger = TIM_TRGO_RESET;
	pwm_mas_config_.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	pwm_oc_init_.OCMode = TIM_OCMODE_PWM1;
	pwm_oc_init_.Pulse = 0;
	pwm_oc_init_.OCPolarity = TIM_OCPOLARITY_HIGH;
	pwm_oc_init_.OCFastMode = TIM_OCFAST_DISABLE;

	pwm_handle_.Init.Prescaler = (double)16800 / frequency;
	pwm_handle_.Init.CounterMode = TIM_COUNTERMODE_UP;
	pwm_handle_.Init.Period = PWM_PERIOD;
	pwm_handle_.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	gpio_init_io_.Alternate = io_convert::TIM_AF[tim_num];
	pwm_handle_.Instance = io_convert::TIM_N[tim_num];

	tim_channel_ = io_convert::TIM_C[tim_ch];
	HAL_GPIO_Init(pin_group_, &gpio_init_io_);
	HAL_TIM_Base_Init(&pwm_handle_);
	HAL_TIM_ConfigClockSource(&pwm_handle_, &pwm_clk_config_);
	HAL_TIM_PWM_Init(&pwm_handle_);
	HAL_TIMEx_MasterConfigSynchronization(&pwm_handle_, &pwm_mas_config_);
	HAL_TIM_PWM_ConfigChannel(&pwm_handle_, &pwm_oc_init_, tim_channel_);
}


void Gpio::write(PinState state)
{
	if (state == HIGH) {
		HAL_GPIO_WritePin(pin_group_, pin_number_, GPIO_PIN_SET);
	}
	else if (state == LOW) {
		HAL_GPIO_WritePin(pin_group_, pin_number_, GPIO_PIN_RESET);
	}
}

void Gpio::write(double percentage)
{
	if (percentage > 100) {
		pwm_oc_init_.Pulse = PWM_PERIOD;
	}
	else {
		pwm_oc_init_.Pulse = percentage * PWM_PERIOD / 100;
	}
	HAL_TIM_PWM_ConfigChannel(&pwm_handle_, &pwm_oc_init_, tim_channel_);
	HAL_TIM_PWM_Start(&pwm_handle_, tim_channel_);
}

bool Gpio::read(void)
{
	return HAL_GPIO_ReadPin(pin_group_, pin_number_);
}
