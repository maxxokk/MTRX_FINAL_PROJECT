/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f303xc.h"


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


// enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;
}
void setup_gpio_PA4_OUTPUT() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;  // Enable GPIOA clock
    GPIOA->MODER &= ~(0x4 << (4 * 2));  // Clear mode bits for PA3
    GPIOA->MODER |= (0x1 << (4 * 2));   // Set mode to General purpose output mode

    GPIOA->OTYPER &= ~(1 << 4);         // Set output type to push-pull
    GPIOA->OSPEEDR |= (0x4 << (4 * 2)); // Set output speed to high
    GPIOA->PUPDR &= ~(0x4 << (4 * 2));  // Ensure no pull-up or pull-down

}
// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_board() {

	// get a pointer to the second half word of the MODER register (for outputs pe8-15)
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;

	setup_gpio_PA4_OUTPUT(); //Setup PA4 for output of conditioned ldr signal

}





void ContinuousReadSingleChannelADC() {

	// get a pointer to the location of the LEDs
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	// enable the clock for ADC1
	RCC->AHBENR |= RCC_AHBENR_ADC12EN;

	// set to synchronise the ADC with the clock
	ADC12_COMMON->CCR |= ADC12_CCR_CKMODE_0;

	// ADEN must be = 0 for configuration (is the default)
	ADC1->CR &= ~ADC_CR_ADVREGEN; // clear voltage regulator enable
	ADC1->CR |= ADC_CR_ADVREGEN_0; // set ADVREGEN TO 01
	ADC1->CR &= ~ADC_CR_ADCALDIF; // clear bit to enable Single-ended-input

	// calibrate the ADC (self calibration routine)
	ADC1->CR |= ADC_CR_ADCAL;
	while((ADC1->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL); // Waiting for the calibration to finish

	// select the channel, only one conversion so it goes in
	//  the first conversion slot (SQ1)
	//  and the L value should be 0 (L=0000 does one conversion only)
	ADC1->SQR1 = 0; // clear any existing channel requests
	ADC1->SQR1 |= 0x02 << ADC_SQR1_SQ1_Pos; // request channel 2
	ADC1->SQR1 |= 0x00 << ADC_SQR1_L_Pos; // set the number of channels to read

	// continuous mode
	ADC1->CFGR |= ADC_CFGR_CONT;

	// Enable ADC
	ADC1->CR |= ADC_CR_ADEN;

	while (ADC1->ISR == 0); //Wait the ADC to be ready.

	// request the process to start
	// only need once in continuous mode
	ADC1->CR |= ADC_CR_ADSTART;

	uint16_t value = 0;
//	uint8_t scale = 0;
	uint16_t trigger = 0;
    /* Loop forever */
	uint16_t high = 0;
	for(;;) {

		// Wait for the end of conversion
		while(!(ADC1->ISR &= ADC_ISR_EOC));

		value = ADC1->DR;
		if (trigger == 0) {
			trigger = value+200;
		}
		// read the value from ADC1->DR,

		if (high == 1 && value < trigger - 150) { // Helps to reduce flickering between high and low
			high = 0;
		}
		else if (high == 0 && value > trigger + 200) { // Helps to reduce flickering between high and low
			high = 1;
		}
		if (high == 1) { // Ball not in holder
			*led_register = 0b00000001;
			GPIOA->ODR |= GPIO_ODR_4;
//		    GPIOA->ODR |= (1 << 4);
			//yes, allowed to count bucket
		}
		else { // ball in holder
		*led_register = 0b00000000;
		GPIOA->ODR &= ~GPIO_ODR_4; // Set PA4 low
		//			//no, don't count bucket
		}

	}
}

int main(void)
{
	enable_clocks();
	initialise_board();

	ContinuousReadSingleChannelADC();

}



