#ifndef FLEX_H
#define FLEX_H

#include <stdint.h>

// Function declarations for enabling clocks, initializing the board, reading ADC values, 
// continuous ADC reading, GPIO setup for PA3, and handling ADC value changes

void enable_clocks();

void initialise_board();

uint16_t ReadADC(uint32_t channel);

void ContinuousReadSingleChannelADC();

void setup_gpio_pa3();

void on_adc_value_change();

// Function declarations for configuring and starting PWM for buzzers on PA8 and PA9

void setup_timer2_pwm(void);

void start_pwm_2_buzzers(uint32_t frequency, uint32_t duty_cycle);

void setup_gpio_PA8_PA9(void);

#endif
