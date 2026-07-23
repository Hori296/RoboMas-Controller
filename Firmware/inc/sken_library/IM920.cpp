/*
 * IM920.cpp
 *
 *  Created on: 2023/03/09
 *      Author: kanri
 */

#include "IM920.h"

IM920::IM920(){}

void IM920::init(Pin tx_pin,Pin rx_pin,UartNumber uart_num){
	serial_.init(tx_pin,rx_pin,uart_num,19200);
}

void IM920::read(uint16_t* rx_id,uint8_t* rx_data,IM920Bytes bytes){
	if(!read_flag_){
		rx_id_ = rx_id;
		return_data_ = rx_data;
		data_size_ = bytes*8;
		rx_size_ = 13+data_size_*3-1;
		serial_.startDmaRead(rx_data_,rx_size_);
		read_flag_ = true;
	}
	//受信データ並べ替え
	for(int i=0; i<rx_size_; i++){
		if(rx_data_[i] == 0xD && rx_data_[(i+1)%rx_size_] == 0xA && rx_data_[(i+2)%rx_size_] == 0x30 && rx_data_[(i+3)%rx_size_] == 0x30){
			for(int j=0; j<rx_size_-2; j++){
				get_data_[j] = rx_data_[((i+2)+j)%rx_size_];
			}
			break;
		}
	}
	//必要な情報を取り出し
	uint16_t id = 0;
	for(int i=0; i<4; i++){
		id |= ((get_data_[3+i]&0xF0) == 0x30)? (get_data_[3+i]&0x0F)<<((3-i)*4) : ((get_data_[3+i]&0x0F)+9)<<((3-i)*4);
	}
	*rx_id_ = id;
	for(int i=0; i<data_size_; i++){
		uint8_t data = 0;
		for(int j=0; j<2; j++){
			data |= ((get_data_[11+i*3+j]&0xF0) != 0x40)? (get_data_[11+i*3+j]&0x0F)<<((1-j)*4) : ((get_data_[11+i*3+j]&0x0F)+9)<<((1-j)*4);
		}
		return_data_[i] = data;
	}
}


void IM920::write(uint8_t* tx_data,IM920Bytes bytes,int node_num){
	if(node_num == 0x0000){
		serial_.write(TXDA_,5);
	}else{
		serial_.write(TXDU_,5);
		sprintf(TxChar_,"%04X",node_num);
		for(int i=0; i<4; i++){
			TxData_[i] = TxChar_[i];
		}
		serial_.write(TxData_,4);
		uint8_t comma = ',';
		serial_.write(&comma,1);
	}
	switch(bytes){
	case Bytes8:
		sprintf(TxChar_,"%02X%02X%02X%02X%02X%02X%02X%02X",tx_data[0],tx_data[1],tx_data[2],tx_data[3],tx_data[4],tx_data[5],tx_data[6],tx_data[7]);
		for(int i=0; i<16; i++){
			TxData_[i] = TxChar_[i];
		}
		serial_.write(TxData_,16);
		break;
	case Bytes16:
		for(int j=0; j<2; j++){
			sprintf(TxChar_,"%02X%02X%02X%02X%02X%02X%02X%02X",tx_data[0+j*8],tx_data[1+j*8],tx_data[2+j*8],tx_data[3+j*8],tx_data[4+j*8],tx_data[5+j*8],tx_data[6+j*8],tx_data[7+j*8]);
			for(int i=0; i<16; i++){
				TxData_[i] = TxChar_[i];
			}
			serial_.write(TxData_,16);
		}
		break;
	case Bytes24:
		for(int j=0; j<3; j++){
			sprintf(TxChar_,"%02X%02X%02X%02X%02X%02X%02X%02X",tx_data[0+j*8],tx_data[1+j*8],tx_data[2+j*8],tx_data[3+j*8],tx_data[4+j*8],tx_data[5+j*8],tx_data[6+j*8],tx_data[7+j*8]);
			for(int i=0; i<16; i++){
				TxData_[i] = TxChar_[i];
			}
			serial_.write(TxData_,16);
		}
		break;
	case Bytes32:
		for(int j=0; j<4; j++){
			sprintf(TxChar_,"%02X%02X%02X%02X%02X%02X%02X%02X",tx_data[0+j*8],tx_data[1+j*8],tx_data[2+j*8],tx_data[3+j*8],tx_data[4+j*8],tx_data[5+j*8],tx_data[6+j*8],tx_data[7+j*8]);
			for(int i=0; i<16; i++){
				TxData_[i] = TxChar_[i];
			}
			serial_.write(TxData_,16);
		}
		break;
	}
	serial_.write(finish_char_,2);
}


void IM920::DebugData(uint8_t* data,int data_size){
	for(int i=0; i<data_size; i++){
		data[i] = get_data_[i];
	}
}
