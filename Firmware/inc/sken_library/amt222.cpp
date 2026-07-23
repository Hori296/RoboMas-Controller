/*
 * SPI.cpp
 *
 *  Created on: 2025/12/09
 *      Author: p1ing
 */

#include "amt222.h"

void Amt222::init(Pin cs_pin,SpiNumber spi_number)
{
	__HAL_RCC_SPI1_CLK_ENABLE();

	gpio_init_spi_.Mode = GPIO_MODE_AF_PP;
	gpio_init_spi_.Pull = GPIO_NOPULL;
	gpio_init_spi_.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	switch(spi_number){
	case SPI_1:
		spi.Instance           = SPI1;
		gpio_init_spi_.Alternate = GPIO_AF5_SPI1;
		gpio_init_spi_.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
		HAL_GPIO_Init(GPIOA, &gpio_init_spi_);
		break;
	case SPI_2:
		spi.Instance           = SPI2;
		gpio_init_spi_.Alternate = GPIO_AF5_SPI2;
		gpio_init_spi_.Pin = GPIO_PIN_7 | GPIO_PIN_2 | GPIO_PIN_3;
		HAL_GPIO_Init(GPIOC, &gpio_init_spi_);
		break;
	case SPI_3:
		spi.Instance           = SPI3;
		gpio_init_spi_.Alternate = GPIO_AF6_SPI3;
		gpio_init_spi_.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
		HAL_GPIO_Init(GPIOC, &gpio_init_spi_);
		break;
	}

	spi.Init.Mode              = SPI_MODE_MASTER;
	spi.Init.Direction         = SPI_DIRECTION_2LINES;
	spi.Init.DataSize          = SPI_DATASIZE_8BIT;
	spi.Init.CLKPolarity       = SPI_POLARITY_LOW;
	spi.Init.CLKPhase          = SPI_PHASE_1EDGE;
	spi.Init.NSS               = SPI_NSS_SOFT;
	spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	spi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	spi.Init.TIMode            = SPI_TIMODE_DISABLE;
	spi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	spi.Init.CRCPolynomial     = 10;

	HAL_SPI_Init(&spi);

	cs.init(cs_pin,OUTPUT);
}

void Amt222::set_zero()
{
	cs.write(LOW);
	HAL_SPI_Transmit(&spi, &cmd[0], 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&spi, &cmd[1], 1, HAL_MAX_DELAY);
	cs.write(HIGH);
}

float Amt222::read()
{
	cs.write(LOW);
    HAL_SPI_TransmitReceive(&spi, &cmd[0], &rx_data[0], 1, HAL_MAX_DELAY);
    HAL_SPI_TransmitReceive(&spi, &cmd[0], &rx_data[1], 1, HAL_MAX_DELAY);
    cs.write(HIGH);

    return (((rx_data[0] << 8 | rx_data[1]) & 0x3FFF) / 16384.0f) * 360.0f;
}


