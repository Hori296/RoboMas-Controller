/*
 * adc.cpp
 *
 *  Created on: 2020/06/17
 *      Author: TAKUMA
 */

#include "adc.h"

Adc::Adc(void) : rank_(0)
{}

void Adc::init(AdcSelect adc_select)
{
	adc_select_ = adc_select;
	if(adc_select_ == ADC_1){
		adc_handle_.Instance = ADC1;
	}
	else if(adc_select_ == ADC_2){
		adc_handle_.Instance = ADC2;
	}
	adc_handle_.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	adc_handle_.Init.Resolution = ADC_RESOLUTION_12B;
	adc_handle_.Init.ScanConvMode = ENABLE;
	adc_handle_.Init.ContinuousConvMode = ENABLE;
	adc_handle_.Init.DiscontinuousConvMode = DISABLE;
	adc_handle_.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	adc_handle_.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	adc_handle_.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc_handle_.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	adc_handle_.Init.DMAContinuousRequests = ENABLE;
}

void Adc::addChannel(Pin pin,uint32_t sampling_time)
{
	GPIO_InitTypeDef gpio_init;
	GPIO_TypeDef* pin_group = io_convert::PIN_GROUP[pin / 16];
	gpio_init.Pin = io_convert::PIN_NUM[pin % 16];
	gpio_init.Mode = GPIO_MODE_ANALOG;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(pin_group, &gpio_init);

	ADC_ChannelConfTypeDef config;
	config.Channel = choseChannel(pin);
	config.Rank = ++rank_;
	config.SamplingTime = sampling_time;
	config.Offset = 0;
    HAL_ADC_ConfigChannel(&adc_handle_, &config);
}

void Adc::startAdc(void)
{
	adc_handle_.Init.NbrOfConversion = rank_;
	HAL_ADC_Init(&adc_handle_);

	DMA_HandleTypeDef dma_handle;
	if(adc_select_ == ADC_1){
	    dma_handle.Instance = DMA2_Stream0;
		dma_handle.Init.Channel = DMA_CHANNEL_0;
	}
	else if(adc_select_ == ADC_2){
	    dma_handle.Instance = DMA2_Stream2;
		dma_handle.Init.Channel = DMA_CHANNEL_1;
	}
    dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
   	dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
   	dma_handle.Init.MemInc = DMA_MINC_ENABLE;
   	dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
   	dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
   	dma_handle.Init.Mode = DMA_CIRCULAR;
   	dma_handle.Init.Priority = DMA_PRIORITY_HIGH;
   	dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
   	HAL_DMA_Init(&dma_handle);
   	__HAL_LINKDMA(&adc_handle_,DMA_Handle,dma_handle);
    HAL_ADC_Start_DMA(&adc_handle_,(uint32_t*)adc_value_,rank_);
}

int Adc::getValue(int rank)
{
	if(0 < rank_ && rank_ < 16){
		return adc_value_[rank - 1];
	}
	else{
		return -1;
	}
}

uint32_t Adc::choseChannel(Pin pin)
{
	switch(pin){
	case A0:
		return ADC_CHANNEL_0;
	case A1:
		return ADC_CHANNEL_1;
	case A2:
		return ADC_CHANNEL_2;
	case A3:
		return ADC_CHANNEL_3;
	case A4:
		return ADC_CHANNEL_4;
	case A5:
		return ADC_CHANNEL_5;
	case A6:
		return ADC_CHANNEL_6;
	case A7:
		return ADC_CHANNEL_7;
	case B0:
		return ADC_CHANNEL_8;
	case B1:
		return ADC_CHANNEL_9;
	case C0:
		return ADC_CHANNEL_10;
	case C1:
		return ADC_CHANNEL_11;
	case C2:
		return ADC_CHANNEL_12;
	case C3:
		return ADC_CHANNEL_13;
	case C4:
		return ADC_CHANNEL_14;
	case C5:
		return ADC_CHANNEL_15;
	}
}
