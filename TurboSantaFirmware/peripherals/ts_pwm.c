/*
 * ts_pwm.c
 *
 * Created: 17 Jan 2015 2:00:44 AM
 *  Author: Andrew
 */ 

#include "ts_pwm.h"


void set_pwm_off(uint8_t channel) {
	if (channel >= PWMCH_NUM_NUMBER) {
		return;
	}
	
	PWM->PWM_CH_NUM[channel].PWM_CDTY = PWM_DUTY_CYCLE_OFF;
}

void set_pwm_25(uint8_t channel) {
	if (channel >= PWMCH_NUM_NUMBER) {
		return;
	}
	
	PWM->PWM_CH_NUM[channel].PWM_CDTY = PWM_DUTY_CYCLE_25;
}

void set_pwm_50(uint8_t channel) {
	if (channel >= PWMCH_NUM_NUMBER) {
		return;
	}
	
	PWM->PWM_CH_NUM[channel].PWM_CDTY = PWM_DUTY_CYCLE_50;
}

void set_pwm_75(uint8_t channel) {
	if (channel >= PWMCH_NUM_NUMBER) {
		return;
	}
	
	PWM->PWM_CH_NUM[channel].PWM_CDTY = PWM_DUTY_CYCLE_75;
}

void set_pwm_on(uint8_t channel) {
	if (channel >= PWMCH_NUM_NUMBER) {
		return;
	}
	
	PWM->PWM_CH_NUM[channel].PWM_CDTY = PWM_DUTY_CYCLE_ON;
}