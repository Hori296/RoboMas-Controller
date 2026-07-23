/*
 * sken_mdd.cpp
 *
 *  Created on: 2021/02/01
 *      Author: TakumaNakao
 */

#include "sken_mdd.h"

void SkenMdd::init(Pin tx_pin,Pin rx_pin,UartNumber uart_num)
{
	serial.init(tx_pin,rx_pin,uart_num,115200);
	serial.startDmaRead(&receive_data,1);
	mode_ = UART_MODE;
}
void SkenMdd::init(MddStdid stdid,Pin can_tx,Pin can_rx,CanSelect can_select){
	stdid_ = stdid;
	can_select_ = can_select;
	sken_system.startCanCommunicate(can_tx,can_rx,can_select);
	sken_system.addCanRceiveInterruptFunc(can_select,&rx_can_data_);
	mode_ = CAN_MODE;
}

bool SkenMdd::tcp(uint8_t id,const float (&command_data)[4],unsigned int resend_time,unsigned int max_wait_time)
{
	if(mode_ == UART_MODE){
		return Uart_tcp(id,command_data,resend_time,max_wait_time);
	}else if(mode_ == CAN_MODE){
		return Can_tcp(id,command_data,resend_time,max_wait_time);
	}else{
		return false;
	}
}

void SkenMdd::udp(uint8_t id,const float (&command_data)[4])
{
	if(mode_ == UART_MODE){
		UartSendData(id,command_data);
	}else if(mode_ == CAN_MODE){
		CanSendData(id,command_data,0);
	}
}

void SkenMdd::UartSendData(uint8_t id,const float (&command_data)[4])
{
	uint8_t send_data[21] = {};
	send_data[0] = 0xA5;
	send_data[1] = 0xA5;
	send_data[2] = ++seq;
	send_data[3] = id;
	setFloatData(command_data,send_data);
	send_data[20] = CalcChecksum(send_data);
	serial.write(send_data,21);
}

void SkenMdd::CanSendData(uint8_t id,const float (&command_data)[4],int send_num)
{
	seq++;
	//闕ｳ雍具ｽｽ�ｿｽ2郢晁侭繝｣郢晏現竊馴ｨｾ螢ｹ�ｼ�騾｡�ｽｪ陷ｿ�ｽｷ邵ｲ竏ｽ�ｽｸ蛟ｶ�ｽｽ謳ｾ�ｽｼ謔ｶ繝ｳ郢晢ｿｽ郢晏現竊都eq
	for(int i=0; i<3; i++){
		send_data_[i][0] = (i+1)<<6 | (seq&0x3F);
	}
	send_data_[0][1] = id;
	//float郢晢ｿｽ郢晢ｽｼ郢ｧ�ｽｿ郢ｧ鄂ｮint8_t邵ｺ�ｽｫ隴ｬ�ｽｼ驍擾ｿｽ
	ConvertIntFloat cif;
	for(int i=0; i<4; i++){
		cif.float_val = command_data[i];
		if(i != 3){
			for(int j=0; j<4; j++){
				send_data_[i][3+j] = cif.uint8_val[j];
			}
		}else{
			send_data_[1][1] = cif.uint8_val[0];
			send_data_[1][2] = cif.uint8_val[1];
			send_data_[2][1] = cif.uint8_val[2];
			send_data_[2][2] = cif.uint8_val[3];
		}
	}
	//郢昶�壹♂郢晢ｿｽ郢ｧ�ｽｯ郢ｧ�ｽｵ郢晢ｿｽ
	for(int i=0; i<3; i++){
		uint8_t checksum = 0;
		for(int j=0; j<7; j++){
			checksum += send_data_[i][j];
		}
		send_data_[i][7] = checksum;
	}

	/*if(send_num){
		sken_system.canTransmit(can_select_,0x00,&send_data_[send_num-1][0],8,20);
	}else{
		for(int i=0; i<3; i++){
			sken_system.canTransmit(can_select_,0x00,&(send_data_[i][0]),8,20);
		}
	}*/
}

bool SkenMdd::Uart_tcp(uint8_t id,const float (&command_data)[4],unsigned int resend_time,unsigned int max_wait_time){
	unsigned int send_time = 0;
	unsigned int start_time = sken_system.millis();
	do {
		if ((sken_system.millis() - start_time) > max_wait_time) { //隴幢ｿｽ陞滂ｽｧ陟包ｿｽ邵ｺ�ｽ｡隴弱ｋ菫｣郢ｧ螳夲ｽｶ�ｿｽ邵ｺ蛹ｻ笳�郢ｧ迚呻ｽ､�ｽｱ隰ｨ蜉ｱ�ｽ帝恆譁絶�
			return false;
		}
		if ((sken_system.millis() - send_time) > resend_time) { //陷�蝓ｼ�ｿｽ竏ｽ�ｽｿ�ｽ｡隴弱ｋ菫｣郢ｧ螳夲ｽｶ�ｿｽ邵ｺ蛹ｻ笳�郢ｧ迚呻ｿｽ蝓ｼ�ｿｽ竏ｽ�ｽｿ�ｽ｡
			UartSendData(id,command_data);
			send_time = sken_system.millis();
		}
	} while (receive_data != seq); //鬨ｾ竏ｽ�ｽｿ�ｽ｡邵ｺ蜉ｱ笳�seq邵ｺ�ｽｨ陷ｿ蠍ｺ�ｽｿ�ｽ｡邵ｺ蜉ｱ笳�seq邵ｺ蠕｡�ｽｸ�ｿｽ髢ｾ�ｽｴ邵ｺ蜷ｶ�ｽ狗ｸｺ�ｽｾ邵ｺ�ｽｧ郢晢ｽｫ郢晢ｽｼ郢晢ｿｽ
	return true;
}

bool SkenMdd::Can_tcp(uint8_t id,const float (&command_data)[4],unsigned int resend_time,unsigned int max_wait_time){
	for(int i=1; i<=3; i++){
		unsigned int send_time = 0;
		unsigned int start_time = sken_system.millis();
		do {
			if ((sken_system.millis() - start_time) > max_wait_time) { //隴幢ｿｽ陞滂ｽｧ陟包ｿｽ邵ｺ�ｽ｡隴弱ｋ菫｣郢ｧ螳夲ｽｶ�ｿｽ邵ｺ蛹ｻ笳�郢ｧ迚呻ｽ､�ｽｱ隰ｨ蜉ｱ�ｽ帝恆譁絶�
				return false;
			}
			if ((sken_system.millis() - send_time) > resend_time) { //陷�蝓ｼ�ｿｽ竏ｽ�ｽｿ�ｽ｡隴弱ｋ菫｣郢ｧ螳夲ｽｶ�ｿｽ邵ｺ蛹ｻ笳�郢ｧ迚呻ｿｽ蝓ｼ�ｿｽ竏ｽ�ｽｿ�ｽ｡
				CanSendData(id,command_data,i);
				send_time = sken_system.millis();
			}
		} while (rx_can_data_.rx_stdid != stdid_ || rx_can_data_.rx_data[0] != ((i<<6)|seq)); //鬨ｾ竏ｽ�ｽｿ�ｽ｡邵ｺ蜉ｱ笳�seq邵ｺ�ｽｨ陷ｿ蠍ｺ�ｽｿ�ｽ｡邵ｺ蜉ｱ笳�seq邵ｺ蠕｡�ｽｸ�ｿｽ髢ｾ�ｽｴ邵ｺ蜷ｶ�ｽ狗ｸｺ�ｽｾ邵ｺ�ｽｧ郢晢ｽｫ郢晢ｽｼ郢晢ｿｽ
	}
	return true;
}

void SkenMdd::setFloatData(const float (&command_data)[4],uint8_t (&send_data)[21])
{
	ConvertIntFloat cif;
	for (int i = 0; i < 4; i++) {
		cif.float_val = command_data[i];
		for (int j = 0; j < 4; j++) {
			send_data[4 + (i * 4) + j] = cif.uint8_val[j];
		}
	}
}

uint8_t SkenMdd::CalcChecksum(const uint8_t (&send_data)[21])
{
	uint8_t checksum = 0;
	for (int i = 2; i < 20; i++) {
		checksum += send_data[i];
	}
	return checksum;
}

SendData SkenMdd::GetSendData(){
	SendData data;
	for(int i=0; i<3; i++){
		for(int j=0; j<8; j++){
			data.send_data[i][j] = send_data_[i][j];
		}
	}
	return data;
}
