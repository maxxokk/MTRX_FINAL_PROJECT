#ifndef FLEX_H
#define FLEX_H

#include <stdint.h>

void enable_clocks();

void initialise_board();

uint16_t ReadADC(uint32_t channel) ;

void ContinuousReadSingleChannelADC();

void setup_gpio_pa3();

void on_adc_value_change();




////buzzers/////
void setup_timer2_pwm(void);

void start_pwm_2_buzzers(uint32_t frequency, uint32_t duty_cycle);

void setup_gpio_PA8_PA9(void);


#endif
