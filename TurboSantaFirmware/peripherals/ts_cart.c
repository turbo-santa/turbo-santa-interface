/*
 * ts_cart.c
 *
 * Created: 17 Jan 2015 1:31:16 PM
 *  Author: Andrew
 */ 

#include "ts_cart.h"

void write_to_address(uint16_t address) {
	(address & 0x1) ? set_pin_high(TS_CART_ADDR0_CONTROLLER,  TS_CART_ADDR0_PIO)  : set_pin_low(TS_CART_ADDR0_CONTROLLER,  TS_CART_ADDR0_PIO);
	(address & 0x2) ? set_pin_high(TS_CART_ADDR1_CONTROLLER,  TS_CART_ADDR1_PIO)  : set_pin_low(TS_CART_ADDR1_CONTROLLER,  TS_CART_ADDR1_PIO);
	(address & 0x4) ? set_pin_high(TS_CART_ADDR2_CONTROLLER,  TS_CART_ADDR2_PIO)  : set_pin_low(TS_CART_ADDR2_CONTROLLER,  TS_CART_ADDR2_PIO);
	(address & 0x8) ? set_pin_high(TS_CART_ADDR3_CONTROLLER,  TS_CART_ADDR3_PIO)  : set_pin_low(TS_CART_ADDR3_CONTROLLER,  TS_CART_ADDR3_PIO);
	(address & 0x10) ? set_pin_high(TS_CART_ADDR4_CONTROLLER,  TS_CART_ADDR4_PIO)  : set_pin_low(TS_CART_ADDR4_CONTROLLER,  TS_CART_ADDR4_PIO);
	(address & 0x20) ? set_pin_high(TS_CART_ADDR5_CONTROLLER,  TS_CART_ADDR5_PIO)  : set_pin_low(TS_CART_ADDR5_CONTROLLER,  TS_CART_ADDR5_PIO);
	(address & 0x40) ? set_pin_high(TS_CART_ADDR6_CONTROLLER,  TS_CART_ADDR6_PIO)  : set_pin_low(TS_CART_ADDR6_CONTROLLER,  TS_CART_ADDR6_PIO);
	(address & 0x80) ? set_pin_high(TS_CART_ADDR7_CONTROLLER,  TS_CART_ADDR7_PIO)  : set_pin_low(TS_CART_ADDR7_CONTROLLER,  TS_CART_ADDR7_PIO);
	(address & 0x100) ? set_pin_high(TS_CART_ADDR8_CONTROLLER,  TS_CART_ADDR8_PIO)  : set_pin_low(TS_CART_ADDR8_CONTROLLER,  TS_CART_ADDR8_PIO);
	(address & 0x200) ? set_pin_high(TS_CART_ADDR9_CONTROLLER,  TS_CART_ADDR9_PIO)  : set_pin_low(TS_CART_ADDR9_CONTROLLER,  TS_CART_ADDR9_PIO);
	(address & 0x400) ? set_pin_high(TS_CART_ADDR10_CONTROLLER, TS_CART_ADDR10_PIO) : set_pin_low(TS_CART_ADDR10_CONTROLLER, TS_CART_ADDR10_PIO);
	(address & 0x800) ? set_pin_high(TS_CART_ADDR11_CONTROLLER, TS_CART_ADDR11_PIO) : set_pin_low(TS_CART_ADDR11_CONTROLLER, TS_CART_ADDR11_PIO);
	(address & 0x1000) ? set_pin_high(TS_CART_ADDR12_CONTROLLER, TS_CART_ADDR12_PIO) : set_pin_low(TS_CART_ADDR12_CONTROLLER, TS_CART_ADDR12_PIO);
	(address & 0x2000) ? set_pin_high(TS_CART_ADDR13_CONTROLLER, TS_CART_ADDR13_PIO) : set_pin_low(TS_CART_ADDR13_CONTROLLER, TS_CART_ADDR13_PIO);
	(address & 0x4000) ? set_pin_high(TS_CART_ADDR14_CONTROLLER, TS_CART_ADDR14_PIO) : set_pin_low(TS_CART_ADDR14_CONTROLLER, TS_CART_ADDR14_PIO);
	(address & 0x8000) ? set_pin_high(TS_CART_ADDR15_CONTROLLER, TS_CART_ADDR15_PIO) : set_pin_low(TS_CART_ADDR15_CONTROLLER, TS_CART_ADDR15_PIO);
}

void write_to_data(uint8_t data) {
	(data & 0x1) ? set_pin_high(TS_CART_DATA0_CONTROLLER, TS_CART_DATA0_PIO) : set_pin_low(TS_CART_DATA0_CONTROLLER, TS_CART_DATA0_PIO);
	(data & 0x2) ? set_pin_high(TS_CART_DATA1_CONTROLLER, TS_CART_DATA1_PIO) : set_pin_low(TS_CART_DATA1_CONTROLLER, TS_CART_DATA1_PIO);
	(data & 0x4) ? set_pin_high(TS_CART_DATA2_CONTROLLER, TS_CART_DATA2_PIO) : set_pin_low(TS_CART_DATA2_CONTROLLER, TS_CART_DATA2_PIO);
	(data & 0x8) ? set_pin_high(TS_CART_DATA3_CONTROLLER, TS_CART_DATA3_PIO) : set_pin_low(TS_CART_DATA3_CONTROLLER, TS_CART_DATA3_PIO);
	(data & 0x10) ? set_pin_high(TS_CART_DATA4_CONTROLLER, TS_CART_DATA4_PIO) : set_pin_low(TS_CART_DATA4_CONTROLLER, TS_CART_DATA4_PIO);
	(data & 0x20) ? set_pin_high(TS_CART_DATA5_CONTROLLER, TS_CART_DATA5_PIO) : set_pin_low(TS_CART_DATA5_CONTROLLER, TS_CART_DATA5_PIO);
	(data & 0x40) ? set_pin_high(TS_CART_DATA6_CONTROLLER, TS_CART_DATA6_PIO) : set_pin_low(TS_CART_DATA6_CONTROLLER, TS_CART_DATA6_PIO);
	(data & 0x80) ? set_pin_high(TS_CART_DATA7_CONTROLLER, TS_CART_DATA7_PIO) : set_pin_low(TS_CART_DATA7_CONTROLLER, TS_CART_DATA7_PIO);
}

uint8_t read_from_data(void) {
	uint8_t data = 0;
	
	if (read_pin_level(TS_CART_DATA0_CONTROLLER, TS_CART_DATA0_PIO)) {
		data |= 0x01;
	}
	if (read_pin_level(TS_CART_DATA1_CONTROLLER, TS_CART_DATA1_PIO)) {
		data |= 0x02;
	}
	if (read_pin_level(TS_CART_DATA2_CONTROLLER, TS_CART_DATA2_PIO)) {
		data |= 0x04;
	}
	if (read_pin_level(TS_CART_DATA3_CONTROLLER, TS_CART_DATA3_PIO)) {
		data |= 0x08;
	}
	if (read_pin_level(TS_CART_DATA4_CONTROLLER, TS_CART_DATA4_PIO)) {
		data |= 0x10;
	}
	if (read_pin_level(TS_CART_DATA5_CONTROLLER, TS_CART_DATA5_PIO)) {
		data |= 0x20;
	}
	if (read_pin_level(TS_CART_DATA6_CONTROLLER, TS_CART_DATA6_PIO)) {
		data |= 0x40;
	}
	if (read_pin_level(TS_CART_DATA7_CONTROLLER, TS_CART_DATA7_PIO)) {
		data |= 0x80;
	}
	
	return data;
}