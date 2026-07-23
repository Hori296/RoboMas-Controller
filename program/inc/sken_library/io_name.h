/*
 * io_name.h
 *
 *  Created on: 2019/09/04
 *      Author: TakumaNakao
 */

#ifndef IO_NAME_H_
#define IO_NAME_H_

#include "stm32f4xx.h"

#define PI 3.1415926535

enum Pin{
	A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,
	B0,B1,B2,B3,B4,B5,B6,B7,B8,B9,B10,B11,B12,B13,B14,B15,
	C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12,C13,C14,C15,
	D0,D1,D2,D3,D4,D5,D6,D7,D8,D9,D10,D11,D12,D13,D14,D15
};

enum TimerNumber{
	TIMER1,
	TIMER2,
	TIMER3,
	TIMER4,
	TIMER5,
	TIMER8,
	TIMER9,
	TIMER10,
	TIMER11,
	TIMER12,
	TIMER13,
	TIMER14
};

enum TimerChannel{
	CH1,
	CH2,
	CH3,
	CH4
};

enum UartNumber{
	SERIAL1,
	SERIAL2,
	SERIAL3,
	SERIAL4,
	SERIAL5
};

enum CanSelect{
	CAN_1 = 1,
	CAN_2 = 2
};

enum AdcSelect{
	ADC_1 = 1,
	ADC_2 = 2
};

enum I2CSelect{
	I2C_1,
	I2C_2,
	I2C_3
};

enum IDmode{
	stdid,
	extid
};

struct CanData{
	uint32_t rx_stdid;
	uint32_t rx_extid;
	uint8_t rx_data[8];
	IDmode rx_idmode;
};

enum SpiNumber{
	SPI_1,
	SPI_2,
	SPI_3
};

namespace io_convert
{
	extern GPIO_TypeDef* PIN_GROUP[4];
	extern uint16_t PIN_NUM[16];
	extern uint8_t TIM_AF[12];
	extern TIM_TypeDef* TIM_N[12];
	extern uint32_t TIM_C[4];
	extern USART_TypeDef* UART_N[5];
	extern uint8_t UART_AF[5];
	extern I2C_TypeDef* I2C_NUM[3];
	extern uint8_t I2C_Alternate[5];
}

#endif /* IO_NAME_H_ */
