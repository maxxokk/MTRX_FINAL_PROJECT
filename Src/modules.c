#include "modules.h"
#include "serial.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void exercise1(void){
    LED_Init();
    Button_Init();

    while (1) {
       // __WFI();  // Wait for interrupt, low power mode
    }
}

void exercise2(void){
	inpDetect = &SerialInput; //what to do when input is detected
	received = &DoubleBuffer; //what to do once string is terminated
	demonstrate = &EchoInput;

	SerialInitialise(BAUD_115200, &USART1_PORT, &finished_transmission);
	enable_USART_interrupt();

	SerialOut("Ready for input\r\n", &USART1_PORT);

	for(;;){} //loop here, waiting for interrupt
}

void exercise3(void){
	basic_function(5000);
	//one_shot_function(500 , &turn_on_LED);
	/* Loop forever */
	for(;;){
	};
}

void integration(void){
	inpDetect = &SerialInput; //what to do when input is detected
	received = &DoubleBuffer; //what to do once string is terminated
	demonstrate = &interpret;

	SerialInitialise(BAUD_115200, &USART1_PORT, &finished_transmission);
	enable_USART_interrupt();

	SerialOut("Ready for input\r\n", &USART1_PORT);

    /* Loop forever */
	for(;;);
}
