/*
 * PS3.cpp
 *
 *  Created on: 2022/01/08
 *      Author: hotaka
 */

#include "PS3.h"

PS3::PS3(){}

void PS3::StartRecive(Pin tx_pin,Pin rx_pin,UartNumber uart_num ){
	serial.init(tx_pin,rx_pin,uart_num,115200);
	serial.startDmaRead(rx_data_,8);
}

bool PS3::Getdata(PS3_data* ps3_data){
	for(int i=0 ; i < 8 ; i++){
		ps3_data->rx_data[i] = rx_data_[i];
	}
	if(rx_data_[0] == 0x80){
		ps3_data->Up 		= ((rx_data_[2]&0x03) == 0x01)? true : false;
		ps3_data->Down 		= ((rx_data_[2]&0x03) == 0x02)? true : false;
		ps3_data->Right 	= ((rx_data_[2]&0x0C) == 0x04)? true : false;
		ps3_data->Left 		= ((rx_data_[2]&0x0C) == 0x08)? true : false;
		ps3_data->Triangle 	= (rx_data_[2]&0x10)? true : false;
		ps3_data->Cross 	= (rx_data_[2]&0x20)? true : false;
		ps3_data->Circle 	= (rx_data_[2]&0x40)? true : false;
		ps3_data->Square 	= (rx_data_[1]&0x01)? true : false;
		ps3_data->L1 		= (rx_data_[1]&0x02)? true : false;
		ps3_data->L2 		= (rx_data_[1]&0x04)? true : false;
		ps3_data->R1 		= (rx_data_[1]&0x08)? true : false;
		ps3_data->R2 		= (rx_data_[1]&0x10)? true : false;
		ps3_data->Start 	= ((rx_data_[2]&0x03) == 0x03)? true : false;
		ps3_data->Select 	= ((rx_data_[2]&0x0C) == 0x0C)? true : false;
		ps3_data->LxPad 	= rx_data_[3]-64;
		ps3_data->LyPad 	= -1*(rx_data_[4]-64);
		ps3_data->RxPad 	= rx_data_[5]-64;
		ps3_data->RyPad 	= -1*(rx_data_[6]-64);
		return true;
	}else{
		ps3_data->Up 		= 0;
		ps3_data->Down 		= 0;
		ps3_data->Right 	= 0;
		ps3_data->Left 		= 0;
		ps3_data->Triangle 	= 0;
		ps3_data->Cross 	= 0;
		ps3_data->Circle 	= 0;
		ps3_data->Square 	= 0;
		ps3_data->L1 		= 0;
		ps3_data->L2 		= 0;
		ps3_data->R1 		= 0;
		ps3_data->R2 		= 0;
		ps3_data->Start 	= 0;
		ps3_data->Select 	= 0;
		ps3_data->LxPad 	= 0;
		ps3_data->LyPad 	= 0;
		ps3_data->RxPad 	= 0;
		ps3_data->RyPad 	= 0;
	}
	return false;
}
