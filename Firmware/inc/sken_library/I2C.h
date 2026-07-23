/*
 * I2C.h
 *
 *  Created on: 2022/08/14
 *      Author: hotaka
 */

#ifndef I2C_H_
#define I2C_H_

#include "sken_library/include.h"

class I2C {
	GPIO_InitTypeDef i2c_gpio_init_;
	I2C_HandleTypeDef i2c_handle_;
	GPIO_TypeDef* pin_group_;
	uint16_t scl_pin_number_,sda_pin_number_;
	I2C_TypeDef* i2c_num_;
	uint8_t i2c_alternate;
public:
	I2C();
	void init(Pin scl,Pin sda,I2CSelect i2c,int clockspeed);
	void write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout = 100);
	void read(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout = 100);
};

#endif /* I2C_H_ */
