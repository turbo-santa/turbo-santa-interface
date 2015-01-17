/*
 * ts_pwm.h
 *
 * Created: 17 Jan 2015 2:01:10 AM
 *  Author: Andrew
 */ 


#ifndef TS_PWM_H_
#define TS_PWM_H_

#include "sam.h"

#define PWM_PERIOD 0xEA			 // At current settings it's 0xEA (aka 234) * (120MHz / 1024) = 8.53uS

#define PWM_DUTY_CYCLE_OFF  0xEA
#define PWM_DUTY_CYCLE_25   0xAE // 3/4 of 0xEA
#define PWM_DUTY_CYCLE_50   0x74 // 1/2 of 0xEA
#define PWM_DUTY_CYCLE_75   0x3A // 1/4 of 0xEA
#define PWM_DUTY_CYCLE_ON   0x00

void set_pwm_off(uint8_t channel);
void set_pwm_25(uint8_t channel);
void set_pwm_50(uint8_t channel);
void set_pwm_75(uint8_t channel);
void set_pwm_on(uint8_t channel);


#endif /* TS_PWM_H_ */