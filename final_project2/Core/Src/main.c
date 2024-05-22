
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ptu_definitions.h"
#include "serial.h"
#include <stdio.h>
#include <string.h>
#include "math.h"
#include "stm32f3xx_hal.h"
#include <stdbool.h>
#include "lidar.h"
#include "buckets.h"


/* Private variables ---------------------------------------------------------*/


uint8_t inpStr[2][BUFFER_SIZE] = {0};
void (*inpDetect)(); //what to do when input is detected
void (*received)(); //what to do once string is terminated
void (*demonstrate)(); //how to show that the string has been input

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
    // Check if PA6 input is high
//    if (GPIOA->IDR & (1 << 6)) {
//        return true;
//    }
//    return false;
	return button();
}

int gamemode =1;

void gameModeFind(uint8_t data[BUFFER_SIZE], int size){
	gamemode = data[0] - 49;
}

void Init_PA4(){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // Probably isn't needed as clocks will already be initialised i assume
	GPIOA->MODER &= ~(GPIO_MODER_MODER4); // Clear PA4
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4); // Clear pupdr
}

bool LDR(void) {
    // Check if PA6 input is high
    if (GPIOA->IDR & (1 << 4)) {
        return true;
    }
    else {
        return false;
    }
}

//bool button(void){
//	if ((GPIOA->IDR & 0x01) == 1){
//		return true;
//	}
//	return false;
//}

int main(void)
{

	uint8_t string_to_send[64] = "INTT\n";
	uint8_t ready_string[64] = "R\n";

	enable_clocks();
	initialise_board();

	SerialInitialise(BAUD_115200, &USART1_PORT, &finished_transmission);
	enable_USART_interrupt();

	initLidar();

	setup_gpio_PA6_INPUT();

	Init_PA4();

	signed char gamemode = 0


//	inpDetect = &SerialInput; //what to do when input is detected
//	received = &DoubleBuffer; //what to do once string is terminated
//	demonstrate = &gameModeFind;

	signed char gamemode = 1;

	while(!button()){}
	while(button()){}

	SerialOut(string_to_send, &USART1_PORT);

	SerialOut(ready_string, &USART1_PORT);

//	uint8_t distance;
//
//    while(!button()){
//        distance = get_distance();
//        sprintf(string_to_send, "S%hu\n", distance);
//        SerialOut(string_to_send, &USART1_PORT);
//    }
//    SerialOut("D", &USART1_PORT);


//	setDist(gamePtr, playerNum);
//
//    while(LDR()){}
//
////    while(!endCondition(gamePtr, end_conds)){
//    int i = 0;
//    while(1){
//        while(!LDR()){}
//        //shotTaken(gamePtr);
//        while(LDR()){
//            if(button()){
//            	i++;
//            	sprintf(string_to_send, "%i,0,0,0,0,0,0,0,0,0,0,0,0\n", i);
//                SerialOut(string_to_send, &USART1_PORT);
//                while(button()){}
//                while(LDR()){}
//            }
//
//        }
//    }

	gameLoop(gamemode);
}

