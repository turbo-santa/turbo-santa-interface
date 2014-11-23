/*
 * ts_pio1.h
 *
 * Created: 23 Nov 2014 6:27:29 PM
 *  Author: Andrew
 */ 



#ifndef TS_PIO1_H_
#define TS_PIO1_H_

#include "sam.h"

#define PERIPHERAL_A	 0
#define PERIPHERAL_B	 1
#define PERIPHERAL_C	 2
#define PERIPHERAL_D	 3
#define PERIPHERAL_SYS	 4
#define PERIPHERAL_GPIO	 5
#define PERIPHERAL_EXTRA 6

#define INITIAL_STATE_LOW  0
#define INITIAL_STATE_HIGH 1

void configure_pin_as_pio_output(Pio* controller, uint32_t pin, uint8_t initial_state);
void set_pin_high(Pio* controller, uint32_t pin);
void set_pin_low(Pio* controller, uint32_t pin);



#endif /* TS_PIO1_H_ */