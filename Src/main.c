#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f303xc.h"
#include "initialize.h"
#include "serial.h"
#include "DigitalIO.h"
#include "integration.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define NUM_SINGLE 2 //number of single player game modes
#define NUM_2P 2 //number of 2 player game modes

//THESE FOUR LINES OF CODE MUST BE AT THE TOP OF THE MAIN FILE TO USE SERIAL IO
uint8_t inpStr[2][BUFFER_SIZE] = {0};
void (*inpDetect)(); //what to do when input is detected
void (*received)(); //what to do once string is terminated and received
void (*demonstrate)(); //how to show that the string has been input
//DO NOT REMOVE

//in PuTTY, select: Serial Input, COM<x> (find in device manager), Speed = 115200, Terminal->local echo = FORCE ON, Terminal->local line editing = FORCE OFF, Connection->serial->Flow control = None

int main(void)
{
	SerialInitialise(BAUD_115200, &USART1_PORT, &finished_transmission);
	enable_USART_interrupt();

	uint8_t input[2][BUFFER_SIZE];
	inpDetect = &SerialInput; //what to do when input is detected
	received = &DoubleBuffer; //what to do once string is terminated
	demonstrate = &gameLoop;

	for(;;){} //loop here, waiting for interrupt
}

