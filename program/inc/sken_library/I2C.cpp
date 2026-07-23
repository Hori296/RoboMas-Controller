/*
 * I2C.cpp
 *
 *  Created on: 2022/08/14
 *      Author: hotaka
 */

#include "I2C.h"

I2C::I2C() {}

void I2C::init(Pin scl,Pin sda,I2CSelect i2c,int clockspeed){
	i2c_num_ = io_convert::I2C_NUM[i2c];
	pin_group_ = io_convert::PIN_GROUP[scl / 16];
	scl_pin_number_ = io_convert::PIN_NUM[scl % 16];
	sda_pin_number_ = io_convert::PIN_NUM[sda % 16];
	i2c_alternate = io_convert::I2C_Alternate[i2c];

	i2c_gpio_init_.Pin = scl_pin_number_ | sda_pin_number_;
	i2c_gpio_init_.Mode = GPIO_MODE_AF_OD;
	i2c_gpio_init_.Alternate = i2c_alternate;
	i2c_gpio_init_.Pull = GPIO_PULLUP;
	i2c_gpio_init_.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(pin_group_,&i2c_gpio_init_);

	//sken_system.delayMillis(10);

	switch(i2c){
	case I2C_1:
		__HAL_RCC_I2C1_CLK_ENABLE();
		break;
	case I2C_2:
		__HAL_RCC_I2C2_CLK_ENABLE();
		break;
	case I2C_3:
		__HAL_RCC_I2C3_CLK_ENABLE();
		break;
	}

	i2c_handle_.Instance = i2c_num_;
	i2c_handle_.Init.ClockSpeed = clockspeed;
	i2c_handle_.Init.DutyCycle = I2C_DUTYCYCLE_2;
	i2c_handle_.Init.OwnAddress1 = 0x01;
	i2c_handle_.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	i2c_handle_.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c_handle_.Init.OwnAddress2 = 0;
	i2c_handle_.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c_handle_.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&i2c_handle_);
}

void I2C::write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout){
	HAL_I2C_Mem_Write(&i2c_handle_,DevAddress<<1,MemAddress,MemAddSize,pData,Size,Timeout);
}

void I2C::read(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout){
	HAL_I2C_Mem_Read(&i2c_handle_,DevAddress<<1,MemAddress,MemAddSize,pData,Size,Timeout);
}
