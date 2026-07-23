/*
 * PS4.cpp
 *
 *  Created on: 2023/05/21
 *      Author: p1ing
 */

#include "PS4.h"

PS4::PS4(){}

void PS4::StartRecive(Pin tx_pin,Pin rx_pin,UartNumber uart_num){
	serial.init(tx_pin,rx_pin,uart_num,115200);
	serial.startDmaRead(rx_data_,8);
}

bool PS4::Getdata(PS4_data* ps4_data){
	for(int i=0;i<8;i++){
		ps4_data->rx_data[i] = rx_data_[i];
		if(ps4_data->rx_data[i] == 0xa5 && ps4_data->rx_data[(i+1)%8] == 0xa5){
			ps4_data->Right    = ((ps4_data->rx_data[(i+2)%8])&0x01) ? true:false;
			ps4_data->Down     = ((ps4_data->rx_data[(i+2)%8])&0x02) ? true:false;
			ps4_data->Left     = ((ps4_data->rx_data[(i+2)%8])&0x04) ? true:false;
			ps4_data->Up       = ((ps4_data->rx_data[(i+2)%8])&0x08) ? true:false;
			ps4_data->Square   = ((ps4_data->rx_data[(i+2)%8])&0x10) ? true:false;
			ps4_data->Cross    = ((ps4_data->rx_data[(i+2)%8])&0x20) ? true:false;
			ps4_data->Circle   = ((ps4_data->rx_data[(i+2)%8])&0x40) ? true:false;
			ps4_data->Triangle = ((ps4_data->rx_data[(i+2)%8])&0x80) ? true:false;
			ps4_data->L1       = ((ps4_data->rx_data[(i+3)%8])&0x01) ? true:false;
			ps4_data->R1       = ((ps4_data->rx_data[(i+3)%8])&0x02) ? true:false;
			ps4_data->L2       = ((ps4_data->rx_data[(i+3)%8])&0x04) ? true:false;
			ps4_data->R2       = ((ps4_data->rx_data[(i+3)%8])&0x08) ? true:false;
			ps4_data->Share    = ((ps4_data->rx_data[(i+3)%8])&0x10) ? true:false;
			ps4_data->Options  = ((ps4_data->rx_data[(i+3)%8])&0x20) ? true:false;
			ps4_data->Ps       = ((ps4_data->rx_data[(i+3)%8])&0x40) ? true:false;
			ps4_data->Pad      = ((ps4_data->rx_data[(i+3)%8])&0x80) ? true:false;
			ps4_data->LxPad    = ps4_data->rx_data[(i+4)%8] - 128;
			ps4_data->LyPad    = ps4_data->rx_data[(i+5)%8] - 128;
			ps4_data->RxPad    = ps4_data->rx_data[(i+6)%8] - 128;
			ps4_data->RyPad    = ps4_data->rx_data[(i+7)%8] - 128;
		}
	}
}
