/*
 * ts_cart.h
 *
 * Created: 17 Jan 2015 1:31:29 PM
 *  Author: Andrew
 */ 


#ifndef TS_CART_H_
#define TS_CART_H_

#include "sam.h"
#include "../board.h"

void write_to_address(uint16_t address);
void write_to_data(uint8_t data);
uint8_t read_from_data(void);

#endif /* TS_CART_H_ */