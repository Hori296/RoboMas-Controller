/*
 * adc.h
 *
 *  Created on: 2020/06/17
 *      Author: TAKUMA
 */

#ifndef SKEN_LIBRARY_ADC_H_
#define SKEN_LIBRARY_ADC_H_

#include "io_name.h"
#include "sken_library/gpio.h"

class Adc
{
public:
	Adc(void);
	void init(AdcSelect adc_select);
	void addChannel(Pin pin,uint32_t sampling_time = ADC_SAMPLETIME_28CYCLES);
	void startAdc(void);
	int getValue(int rank);
private:
	AdcSelect adc_select_;
	int rank_;
	uint16_t adc_value_[16];
	ADC_HandleTypeDef adc_handle_;
	uint32_t choseChannel(Pin pin);
};


#endif /* SKEN_LIBRARY_ADC_H_ */
