#include "../../FLEX_RESISTOR/Inc/flex.h"

#include "stm32f303xc.h"
#include <stddef.h>



// enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;
}

// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_board() {

	// get a pointer to the second half word of the MODER register (for outputs pe8-15)
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;
}

uint16_t ReadADC(uint32_t channel) {

	ADC1->SQR1 = 0;
	ADC1->SQR1 |= channel << ADC_SQR1_SQ1_Pos; // set the request for channel specified

	// request the process to start
	ADC1->CR |= ADC_CR_ADSTART;

	// Wait for the end of conversion
	while((ADC1->ISR &= ADC_ISR_EOS) != ADC_ISR_EOS);

	return ADC1->DR;
}



void ContinuousReadSingleChannelADC() {




	//// very simple delay loop
void delay_ms(uint32_t ms) {
	   for (volatile uint32_t i = 0; i < ms * 8000; i++) {

	    }
	}


	setup_gpio_PA3_OUTPUT();
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
	uint8_t scale = 0;

    /* Loop forever */
	for(;;) {

		// Wait for the end of conversion
		while(!(ADC1->ISR &= ADC_ISR_EOC));

		value = ADC1->DR;

		// read the value from ADC1->DR,
		// full range is 12 bits (0xFFF maximum)
		// divide the scale into 8 even partitions (for 8 leds)
		scale = value / (0xfff / 8);

		if (scale > 7) {
			scale = 7;
		}


		*led_register = 1 << scale ;

		if(*led_register == 0x0004){
		    GPIOA->ODR |= (1 << 3);
		    start_pwm_2_buzzers(TIM1, 1,300, 90); // PA8
		    delay_ms(5);
		    start_pwm_2_buzzers(TIM1, 2,1000, 90); // PA9





		    }
		else{
			GPIOA->BRR = (1 << 3);
			//start_pwm(500,0);
			start_pwm_2_buzzers(TIM1, 1,500, 0);
			start_pwm_2_buzzers(TIM1, 2,500, 0);

				    }

	}

}



void setup_gpio_PA3_OUTPUT() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;  // Enable GPIOA clock
    GPIOA->MODER &= ~(0x3 << (3 * 2));  // Clear mode bits for PA3
    GPIOA->MODER |= (0x1 << (3 * 2));   // Set mode to General purpose output mode

    GPIOA->OTYPER &= ~(1 << 3);         // Set output type to push-pull
    GPIOA->OSPEEDR |= (0x3 << (3 * 2)); // Set output speed to high
    GPIOA->PUPDR &= ~(0x3 << (3 * 2));  // Ensure no pull-up or pull-down

}





////////////////////////////////////////////////////MAIN BOARD////////////////////////////////////////

#include <stdbool.h>

void setup_gpio_PA6_INPUT() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;  // Enable GPIOA clock

    // Configure PA6 as input
    GPIOA->MODER &= ~(0x3 << (6 * 2));  // Clear mode bits for PA6 to set it as input

    // No need to configure OTYPER for input mode
    // No need to configure OSPEEDR for input mode
    // Configure PUPDR as needed (e.g., no pull-up/pull-down, pull-up, or pull-down)
    GPIOA->PUPDR &= ~(0x3 << (6 * 2));  // Ensure no pull-up or pull-down
}

bool flex(void) {
    setup_gpio_PA6_INPUT();  // Ensure PA6 is configured as input

    // Check if PA6 input is high
    if (GPIOA->IDR & (1 << 6)) {
        return true;
    } else {
        return false;
    }
}

////////////////////BUZZZZZER_CONFIG/////////////////////////////////////////////////////////////////////



void setup_gpio_PA8_PA9(void) {
        // Enable GPIOA clock
        RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

        // Set PA8 and PA9 to alternate function mode
        GPIOA->MODER &= ~(0x3 << (8 * 2));  // Clear mode bits for PA8
        GPIOA->MODER |= (0x2 << (8 * 2));   // Set mode to alternate function for PA8

        GPIOA->MODER &= ~(0x3 << (9 * 2));  // Clear mode bits for PA9
        GPIOA->MODER |= (0x2 << (9 * 2));   // Set mode to alternate function for PA9

        // Set alternate function to TIM1_CH1 (AF6) for PA8 and TIM1_CH2 (AF6) for PA9
        GPIOA->AFR[1] &= ~(0xF << ((8 - 8) * 4)); // Clear alternate function bits for PA8
        GPIOA->AFR[1] |= (0x6 << ((8 - 8) * 4));  // Set alternate function to AF6 (TIM1_CH1)

        GPIOA->AFR[1] &= ~(0xF << ((9 - 8) * 4)); // Clear alternate function bits for PA9
        GPIOA->AFR[1] |= (0x6 << ((9 - 8) * 4));  // Set alternate function to AF6 (TIM1_CH2)
    }











/////////////////PWM for 2 Buzzers PA8 and PA9////////////////////////////

void start_pwm_2_buzzers(TIM_TypeDef *TIMx, uint32_t channel, uint32_t frequency, uint32_t duty_cycle) {
    uint32_t timer_clock = 8000000; // Assuming 8 MHz clock for simplicity

    // Calculate prescaler and auto-reload values for the desired frequency
    uint32_t prescaler = (timer_clock / (frequency * 1000)) - 1;
    uint32_t period = 1000; // Fixed period value for simplicity

    // Set the prescaler and auto-reload register values
    TIMx->PSC = prescaler;
    TIMx->ARR = period;

    // Calculate and set the compare value for the desired duty cycle
    if (channel == 1) {
        TIMx->CCR1 = (period * duty_cycle) / 100;
    } else if (channel == 2) {
        TIMx->CCR2 = (period * duty_cycle) / 100;
    }

    // Start the timer
    TIMx->CR1 |= TIM_CR1_CEN;
}


void setup_timer2_pwm(void) {
    // Enable TIM1 clock
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    // Configure TIM1 for PWM mode on CH1 (PA8) and CH2 (PA9)
    TIM1->PSC = 7999;  // Prescaler value (to get a suitable PWM frequency)
    TIM1->ARR = 1000;  // Auto-reload value (to set the PWM period)

    // Set output compare mode to PWM1 for CH1
    TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM1->CCMR1 |= (0x6 << TIM_CCMR1_OC1M_Pos);

    // Set output compare mode to PWM1 for CH2
    TIM1->CCMR1 &= ~TIM_CCMR1_OC2M;
    TIM1->CCMR1 |= (0x6 << TIM_CCMR1_OC2M_Pos);

    // Enable the output compare channels 1 and 2
    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    // Enable the main output
    TIM1->BDTR |= TIM_BDTR_MOE;

    // Start the timer
    TIM1->CR1 |= TIM_CR1_CEN;
}


