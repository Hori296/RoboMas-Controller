/*
 * IM920.h
 *
 *  Created on: 2023/03/09
 *      Author: kanri
 */

#ifndef IM920_H_
#define IM920_H_

#include "include.h"
#include "stdio.h"

enum IM920Bytes{
	Bytes8 = 1,
	Bytes16,
	Bytes24,
	Bytes32,
};

class IM920{
	Uart serial_;
	bool read_flag_ = false;
	int rx_size_,data_size_;
	uint16_t *rx_id_;
	uint8_t *return_data_;
	uint8_t rx_data_[108];
	uint8_t get_data_[108];
	uint8_t TXDA_[6] = {'T','X','D','A',' '};
	uint8_t TXDU_[6] = {'T','X','D','U',' '};
	char TxChar_[16];
	uint8_t TxData_[16];
	uint8_t finish_char_[2] = {0xA,0xD};
public:
	IM920();
	void init(Pin tx_pin,Pin rx_pin,UartNumber uart_num);
	void read(uint16_t* rx_id,uint8_t* rx_data,IM920Bytes bytes);
	void write(uint8_t* tx_data,IM920Bytes bytes,int node_num = 0x0000);
	void DebugData(uint8_t* data,int data_size);
};

#endif /* IM920_H_ */
