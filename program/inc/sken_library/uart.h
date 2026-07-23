/*
 * uart.h
 *
 *  Created on: 2019/09/03
 *      Author: TakumaNakao
 */

#ifndef UART_H_
#define UART_H_

#include "io_name.h"

class Uart
{
public:
	void init(Pin tx_pin,Pin rx_pin,UartNumber uart_num,int baudrate,uint32_t parity = UART_PARITY_NONE);
	void startDmaRead(uint8_t* data_p,int data_size);
	uint8_t read(unsigned int dead_time);
	void write(uint8_t* data_p,int data_size,int dead_time = 100);
	bool checkTimeOut(void);
private:
	GPIO_TypeDef* tx_pin_group_;
	GPIO_TypeDef* rx_pin_group_;
	uint32_t tx_pin_number_;
	uint32_t rx_pin_number_;
	GPIO_InitTypeDef gpio_init_uart_;
	UART_HandleTypeDef uart_handle_;
	DMA_HandleTypeDef dma_handle_;
	bool time_out_;
};

#endif /* UART_H_ */
