#include "serial.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "stm32f303xc.h"

// We store the pointers to the GPIO and USART that are used
//  for a specific serial port. To add another serial port
//  you need to select the appropriate values.
struct _SerialPort {
	USART_TypeDef *UART;
	GPIO_TypeDef *GPIO;
	volatile uint32_t MaskAPB2ENR;	// mask to enable RCC APB2 bus registers
	volatile uint32_t MaskAPB1ENR;	// mask to enable RCC APB1 bus registers
	volatile uint32_t MaskAHBENR;	// mask to enable RCC AHB bus registers
	volatile uint32_t SerialPinModeValue;
	volatile uint32_t SerialPinSpeedValue;
	volatile uint32_t SerialPinAlternatePinValueLow;
	volatile uint32_t SerialPinAlternatePinValueHigh;
	void (*completion_function)(uint32_t);
};

// instantiate the serial port parameters
//   note: the complexity is hidden in the c file
SerialPort USART1_PORT = {USART1,
		GPIOC,
		RCC_APB2ENR_USART1EN, // bit to enable for APB2 bus
		0x00,	// bit to enable for APB1 bus
		RCC_AHBENR_GPIOCEN, // bit to enable for AHB bus
		0xA00,
		0xF00,
		0x770000,  // for USART1 PC10 and 11, this is in the AFR low register
		0x00, // no change to the high alternate function register
		0x00 // default function pointer is NULL
		};

void finished_transmission(uint32_t bytes_sent) {
	// This function will be called after a transmission is complete

	volatile uint32_t test = 0;
	// make a very simple delay
	for (volatile uint32_t i = 0; i < 0x8ffff; i++) {
		// waste time !
	}
}

// InitialiseSerial - Initialise the serial port
// Input: baudRate is from an enumerated set
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t)) {

	serial_port->completion_function = completion_function;

	// enable clock power, system configuration clock and GPIOC
	// common to all UARTs
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// enable the GPIO which is on the AHB bus
	RCC->AHBENR |= serial_port->MaskAHBENR;

	// set pin mode to alternate function for the specific GPIO pins
	serial_port->GPIO->MODER = serial_port->SerialPinModeValue;

	// enable high speed clock for specific GPIO pins
	serial_port->GPIO->OSPEEDR = serial_port->SerialPinSpeedValue;

	// set alternate function to enable USART to external pins
	serial_port->GPIO->AFR[0] |= serial_port->SerialPinAlternatePinValueLow;
	serial_port->GPIO->AFR[1] |= serial_port->SerialPinAlternatePinValueHigh;

	// enable the device based on the bits defined in the serial port definition
	RCC->APB1ENR |= serial_port->MaskAPB1ENR;
	RCC->APB2ENR |= serial_port->MaskAPB2ENR;

	// Get a pointer to the 16 bits of the BRR register that we want to change
	uint16_t *baud_rate_config = (uint16_t*)&serial_port->UART->BRR; // only 16 bits used!

	// Baud rate calculation from datasheet
	switch(baudRate) {
	case BAUD_9600:
		// NEED TO FIX THIS !
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_19200:
		// NEED TO FIX THIS !
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_38400:
		// NEED TO FIX THIS !
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_57600:
		// NEED TO FIX THIS !
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	case BAUD_115200:
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	}


	// enable serial port for tx and rx
	serial_port->UART->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

//////////////////////////////////////////////////////////////////////////////////

uint8_t outStr[OUT_BUFFER_SIZE] = {'\0'}; //initialise the array which stores the string/s to be sent via serial
int offset = 0;
SerialPort *out_serial = NULL;
uint8_t strNum = 0; //num of strings waiting to be sent

void SerialOut_IRQ() {
	if(out_serial == NULL) {
		return;
	}

	if(outStr[offset]) { //check there is a character to send (not '\0')
		out_serial->UART->TDR = outStr[offset]; //send character to serial output register
		offset++;
	}

	else { //character to send is '\0' - end of string
		offset = 0;
		strNum = 0; //no more strings waiting to be sent
		disable_tx_interrupt(); //disable the tx interrupt now that the string has been fully sent
	}
}


void SerialOut(uint8_t *data, SerialPort *serial_port) {
	if(strNum) {
		strcat(outStr, data); //if there is a string waiting to be sent, concatenate the new string onto the end of outStr
	}

	else {
		out_serial = serial_port; //define the serial port to output to
		strcpy(outStr, data); //copy data into the outStr array
		enable_tx_interrupt(); //enable the transmit empty interrupt
	}
	strNum++; //add one to the strNum to represent one more string waiting to send
}


void ClearInput(SerialPort *serial_port) {
	serial_port->UART->ICR = (USART_ICR_FECF | USART_ICR_ORECF); //clear FE and ORE bits

    if((serial_port->UART->ISR & USART_ISR_FE)||(serial_port->UART->ISR & USART_ISR_ORE)) {
    	ClearInput(serial_port); //if FE or ORE bits are still on, clear them again. Be careful of infinite looping here.
    }
}

void FullClear(SerialPort *serial_port) {
	ClearInput(serial_port);
	serial_port->UART->RQR |= USART_RQR_RXFRQ; //clear RXNE bit
}

int i = 0;
void SerialInput(uint8_t data[2][BUFFER_SIZE], SerialPort *serial_port) {
	if(!i && (serial_port->UART->ISR & USART_ISR_FE)||(serial_port->UART->ISR & USART_ISR_ORE)) {
    	ClearInput(serial_port); //reset ORE and FE bits
    }

	if(i==BUFFER_SIZE-3) { //too many letters donny
		data[0][i] = '\r';
	}

	else {
		data[0][i] = serial_port->UART->RDR; //read character from receive register
	}

	if(data[0][i] == '\r') { // putty sends \r when you press enter
		data[0][i+1] = '\n';
		data[0][i+2] = '\0'; //null terminate string
		if(received != NULL) {
			received(data[0], (i+3)); //callback function
		}
		FullClear(serial_port); //reset ORE, FE, and RXNE bits
		i = 0;
	}
	else {
		i++;
		ClearInput(serial_port);
	}
}

void DoubleBuffer(uint8_t data[BUFFER_SIZE], int NumChar) {
	for(int j=0; j<NumChar; j++) {
		inpStr[1][j] = data[j]; //transfer Kernel to User buffer
	}

	memset(data, 0, sizeof(data)); //clear Kernel
	memset(inpStr[0], 0, sizeof(inpStr[0])); //clear Kernel

	if(demonstrate != NULL) {
		demonstrate(inpStr[1], NumChar); //if there is a demonstrate function defined, run it now
	}
}

//example code to demonstrate input and NumChar callback function
/////////////////////////////////////////////////////////////////////////
void upper(char str[BUFFER_SIZE]) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

void EchoInput(uint8_t data[BUFFER_SIZE], int NumChar) {
	uint8_t copy[BUFFER_SIZE] = {0};
	strcpy(copy, data);

	upper(copy);

	char OutLen[30] = "Number of characters = ";
	char NumCharStr[3];
	sprintf(NumCharStr, "%d", (NumChar-3));
	strcat(OutLen, NumCharStr);
	strcat(OutLen, "\r\n");

	SerialOut(copy, &USART1_PORT);
	SerialOut(OutLen, &USART1_PORT);
}
//////////////////////////////////////////////////////////////////////////

void USART1_EXTI25_IRQHandler(void) {
	if (inpDetect != 0x00 && ((&USART1_PORT)->UART->ISR & USART_ISR_RXNE)) {
		inpDetect(inpStr, &USART1_PORT); //interrupt is for serial input
	}

	if (((&USART1_PORT)->UART->ISR & USART_ISR_TXE)&&((&USART1_PORT)->UART->CR1 & USART_CR1_TXEIE)) {
		SerialOut_IRQ(); //interrupt is for serial output
	}
}

void enable_USART_interrupt(void) {
	__disable_irq(); //disable interrupts while changes are made
	USART1->CR1 |= USART_CR1_RXNEIE;

	NVIC_SetPriority(USART1_IRQn, 1);
	NVIC_EnableIRQ(USART1_IRQn);

	__enable_irq(); //reenable interrupts
}

void enable_tx_interrupt(void) {
	__disable_irq();
	USART1->CR1 |= USART_CR1_TXEIE; //enable interrupt for TXE bit on
	__enable_irq();
}

void disable_tx_interrupt(void) {
	__disable_irq();
	USART1->CR1 &= ~(USART_CR1_TXEIE);
	__enable_irq();
}

