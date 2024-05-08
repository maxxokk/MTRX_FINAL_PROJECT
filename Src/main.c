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

//THESE FOUR LINES OF CODE MUST BE AT THE TOP OF THE MAIN FILE TO USE SERIAL IO
uint8_t inpStr[2][BUFFER_SIZE] = {0};
void (*inpDetect)(); //what to do when input is detected
void (*received)(); //what to do once string is terminated
void (*demonstrate)(); //how to show that the string has been input
//DO NOT REMOVE

//in PuTTY, select: Serial Input, COM<x> (find in device manager), Speed = 115200, Terminal->local echo = FORCE ON, Terminal->local line editing = FORCE OFF, Connection->serial->Flow control = None

int main(void)
{

}

