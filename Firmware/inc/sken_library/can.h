/*
 * can.h
 *
 *  Created on: 2019/09/07
 *      Author: TakumaNakao
 */

#ifndef CAN_H_
#define CAN_H_

#include "stm32f4xx.h"
#include "sken_library/gpio.h"
#include "sken_library/io_name.h"

typedef struct {
	uint32_t err;
    uint32_t esr;
    uint32_t msr;
    uint32_t tsr;
    uint32_t rf0r;
    uint32_t rf1r;

    uint8_t lec;
    uint8_t tec;
    uint8_t rec;
    uint8_t ewgf;
    uint8_t epvf;
    uint8_t boff;
    uint8_t tx_free;
    uint8_t fifo0_pending;
    uint8_t fifo1_pending;

    uint32_t tx_count;
    uint32_t tx_fail_count;
    uint32_t rx_count;
    uint32_t last_tx_id;
    uint32_t last_rx_id;
} CanDebug;

class Can
{
public:
	Can(void);
	bool init(Pin tx_pin,Pin rx_pin,CanSelect can_select);
	inline CAN_HandleTypeDef* getCanHandle(void);
	inline void receiveInterruptCallback(void);
	HAL_StatusTypeDef transmit(uint32_t id, uint8_t* data_p, int data_size, int dead_time, IDmode id_mode);
	void addRceiveInterruptFunc(CanData* can_data);
	void deleteRceiveInterruptFunc();
	CanRxMsgTypeDef rx_receive();
	void get_can_debug(CanDebug *dbg);
private:
	GPIO_TypeDef* tx_pin_group_;
	GPIO_TypeDef* rx_pin_group_;
	uint32_t tx_pin_number_;
	uint32_t rx_pin_number_;
	bool can_start_flag_;
	GPIO_InitTypeDef gpio_init_can_;
	CAN_HandleTypeDef can_handle_;
	CAN_FilterConfTypeDef filter_config_;
	CanRxMsgTypeDef can_rx_;
	CanTxMsgTypeDef can_tx_;
	CanData* can_data_;
	bool can_interrupt_flag_;
	bool can_select_;
};

extern Can can_1;
extern Can can_2;

#endif /* CAN_H_ */
