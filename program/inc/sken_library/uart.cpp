/*
 * uart.cpp
 *
 *  Created on: 2019/09/07
 *      Author: TakumaNakao
 */

#include "uart.h"

void Uart::init(Pin tx_pin,Pin rx_pin,UartNumber uart_num,int baudrate,uint32_t parity)
{
	tx_pin_group_ = io_convert::PIN_GROUP[tx_pin / 16];
	rx_pin_group_ = io_convert::PIN_GROUP[rx_pin / 16];
	tx_pin_number_ = io_convert::PIN_NUM[tx_pin % 16];
	rx_pin_number_ = io_convert::PIN_NUM[rx_pin % 16];
	gpio_init_uart_.Mode = GPIO_MODE_AF_PP;
	gpio_init_uart_.Pull = GPIO_PULLUP;
	gpio_init_uart_.Speed = GPIO_SPEED_FAST;
	gpio_init_uart_.Alternate = io_convert::UART_AF[uart_num];

	uart_handle_.Instance = io_convert::UART_N[uart_num];
	uart_handle_.Init.WordLength = UART_WORDLENGTH_8B;
	uart_handle_.Init.StopBits = UART_STOPBITS_1;
	uart_handle_.Init.Parity = parity;
	uart_handle_.Init.Mode = UART_MODE_TX_RX;
	uart_handle_.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart_handle_.Init.OverSampling = UART_OVERSAMPLING_16;

	uart_handle_.Init.BaudRate = baudrate;
	gpio_init_uart_.Pin = tx_pin_number_;
	HAL_GPIO_Init(tx_pin_group_, &gpio_init_uart_);
	gpio_init_uart_.Pin = rx_pin_number_;
	HAL_GPIO_Init(rx_pin_group_, &gpio_init_uart_);
	HAL_UART_Init(&uart_handle_);
	__HAL_UART_ENABLE(&uart_handle_);
}

void Uart::startDmaRead(uint8_t* data_p,int data_size)
{
	if(uart_handle_.Instance == USART1){
		dma_handle_.Instance = DMA2_Stream2;
		dma_handle_.Init.Channel = DMA_CHANNEL_4;
	}
	else if(uart_handle_.Instance == USART2){
		dma_handle_.Instance = DMA1_Stream5;
		dma_handle_.Init.Channel = DMA_CHANNEL_4;
	}
	else if(uart_handle_.Instance == USART3){
			dma_handle_.Instance = DMA1_Stream1;
			dma_handle_.Init.Channel = DMA_CHANNEL_4;
	}
	else if(uart_handle_.Instance == UART4){
		dma_handle_.Instance = DMA1_Stream2;
		dma_handle_.Init.Channel = DMA_CHANNEL_4;
	}
	else if(uart_handle_.Instance == UART5){
		dma_handle_.Instance = DMA1_Stream0;
		dma_handle_.Init.Channel = DMA_CHANNEL_4;
	}
	dma_handle_.Init.Direction = DMA_PERIPH_TO_MEMORY;
	dma_handle_.Init.PeriphInc = DMA_PINC_DISABLE;
	dma_handle_.Init.MemInc = DMA_MINC_ENABLE;
	dma_handle_.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	dma_handle_.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	dma_handle_.Init.Mode = DMA_CIRCULAR;
	dma_handle_.Init.Priority = DMA_PRIORITY_LOW;
	dma_handle_.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	HAL_DMA_Init(&dma_handle_);
	__HAL_LINKDMA(&uart_handle_,hdmarx,dma_handle_);
	HAL_UART_Receive_DMA(&uart_handle_, data_p, data_size);
}

uint8_t Uart::read(unsigned int deadtime)
{
	uint8_t data = 0;
	if (HAL_UART_Receive(&uart_handle_, &data, 1, deadtime) == HAL_TIMEOUT) {
		HAL_UART_Init(&uart_handle_);
		__HAL_UART_CLEAR_PEFLAG(&uart_handle_);
		__HAL_UART_CLEAR_FEFLAG(&uart_handle_);
		__HAL_UART_CLEAR_NEFLAG(&uart_handle_);
		__HAL_UART_CLEAR_OREFLAG(&uart_handle_);
		__HAL_UART_CLEAR_IDLEFLAG(&uart_handle_);
		time_out_ = true;
		data = 0;
	}
	else {
		time_out_ = false;
	}
	return data;
}

void Uart::write(uint8_t* data_p,int data_size,int dead_time)
{
	HAL_UART_Transmit(&uart_handle_,data_p,data_size,dead_time);
}

bool Uart::checkTimeOut(void)
{
	return time_out_;
}
