/*
 * gpio.h
 *
 *  Created on: 2019/09/03
 *      Author: TakumaNakao
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "io_name.h"

enum Mode{
	OUTPUT,
	INPUT,
	INPUT_PULLUP,
	INPUT_PULLDOWN,
	PWM_OUTPUT
};

enum PinState{
	LOW,
	HIGH
};

class Gpio 
{
public:
	void init(Pin pin,Mode pin_mode);
	void init(Pin pin,Mode pin_mode,TimerNumber tim_num,TimerChannel tim_ch,int frequency = 980);
	void write(PinState state);
	void write(double percentage);
	bool read(void);
private:
	const int PWM_PERIOD = 10000;
	GPIO_TypeDef* pin_group_;
	uint16_t pin_number_;
	Mode pin_mode_;
	uint32_t tim_channel_;
	GPIO_InitTypeDef gpio_init_io_;
	TIM_ClockConfigTypeDef pwm_clk_config_;
	TIM_MasterConfigTypeDef pwm_mas_config_;
	TIM_OC_InitTypeDef pwm_oc_init_;
	TIM_HandleTypeDef pwm_handle_;
};

#endif /* GPIO_H_ */
