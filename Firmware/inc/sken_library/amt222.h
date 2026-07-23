/*
 * SPI.h
 *
 *  Created on: 2025/12/09
 *      Author: p1ing
 */

#ifndef SKEN_LIBRARY_AMT222_H_
#define SKEN_LIBRARY_AMT222_H_

#include "io_name.h"
#include "gpio.h"

class Amt222
{
public:
	void init(Pin cs_pin,SpiNumber spi_number);
	void set_zero();
	float read();
private:
	SPI_HandleTypeDef spi;
	GPIO_InitTypeDef gpio_init_spi_;
	Gpio cs;
	uint8_t cmd[2] = {0x00,0x70};
	uint8_t rx_data[2];
};



#endif /* SKEN_LIBRARY_AMT222_H_ */
