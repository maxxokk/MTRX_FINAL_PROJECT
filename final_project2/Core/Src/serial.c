#include "serial.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "stm32f303xc.h"


// NOTE: these are stored as pointers because they
//       are const values so we can't store them directly
//       in the struct
struct _SerialPort {
	USART_TypeDef *UART;
	GPIO_TypeDef *GPIO;
	volatile uint32_t *BaudRate;
	volatile uint32_t *ControlRegister1;
	volatile uint32_t *StatusRegister;
	volatile uint16_t *DataOutputRegister;
	volatile uint32_t *TimerEnableRegister;
	volatile uint32_t TimerEnableMask;
	volatile uint32_t SerialPortGPIO;
	volatile uint32_t *SerialPinModeRegister;
	volatile uint32_t SerialPinModeValue;
	volatile uint32_t *SerialPinSpeedRegister;
	volatile uint32_t SerialPinSpeedValue;
	volatile uint8_t *SerialPinAlternatePinRegister;
	volatile uint8_t SerialPinAlternatePinValue;
	void (*completion_function)(uint32_t);
};


// The different serial ports require different GPIO ports
enum {
	SERIAL_GPIO_A,
	SERIAL_GPIO_B,
	SERIAL_GPIO_C,
	SERIAL_GPIO_D,
	SERIAL_GPIO_E
};



// instantiate the serial port parameters
//   note: the complexity is hidden in the c file
SerialPort USART1_PORT = {USART1,
		GPIOC,
		&(USART1->BRR),
		&(USART1->CR1),
		&(USART1->ISR),
		&(USART1->TDR),
		&(RCC->APB2ENR),
		RCC_APB2ENR_USART1EN,
		SERIAL_GPIO_C,
		&(GPIOC->MODER),
		0xA00,
		&(GPIOC->OSPEEDR),
		0xF00,
		((uint8_t*)&(GPIOC->AFR[0])) + 2,
		0x77};


// InitialiseSerial - Initialise the serial port
// Input: baudRate is from an enumerated set
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t)) {

	serial_port->completion_function = completion_function;

	// enable clock power, system configuration clock and GPIOC
	// common to all UARTs
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	switch(serial_port->SerialPortGPIO) {
	case SERIAL_GPIO_C:
		RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
		break;
	default:
		break;
	}

	// set pin mode
	*(serial_port->SerialPinModeRegister) = serial_port->SerialPinModeValue;

	// enable high speed clock for GPIOC
	*(serial_port->SerialPinSpeedRegister) = serial_port->SerialPinSpeedValue;

	// set alternate function to enable USART to an external pin
	*(serial_port->SerialPinAlternatePinRegister) = serial_port->SerialPinAlternatePinValue;

	*(serial_port->TimerEnableRegister) |= serial_port->TimerEnableMask;

	uint16_t *baud_rate_config = ((uint16_t*)serial_port->BaudRate); // only 16 bits used!

	// Baud rate calculation from datasheet
	switch(baudRate){
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
		*baud_rate_config = 0x46 * 0x06;  // 115200 at 8MHz
		//*baud_rate_config = 0x46;
		break;
	}


	// enable serial port for tx and rx
	*(serial_port->ControlRegister1) |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}


void SerialOutputChar(uint8_t data, SerialPort *serial_port) {
	while((*(serial_port->StatusRegister) & USART_ISR_TXE) == 0){
	}

	*(serial_port->DataOutputRegister) = data;
}



void SerialOutputString(uint8_t *pt, SerialPort *serial_port) {

	uint32_t counter = 0;
	while(*pt) {
		SerialOutputChar(*pt, serial_port);
		counter++;
		pt++;
	}

	if (serial_port->completion_function != 0x00)
		serial_port->completion_function(counter);
}

uint8_t outStr[OUT_BUFFER_SIZE] = {'\0'}; //initialise the array which stores the string/s to be sent via serial
int offset = 0;
SerialPort *out_serial = NULL;
uint8_t strNum = 0; //num of strings waiting to be sent

void finished_transmission(uint32_t bytes_sent) {
	// This function will be called after a transmission is complete

	volatile uint32_t test = 0;
	// make a very simple delay
	for (volatile uint32_t i = 0; i < 0x8ffff; i++) {
		// waste time !
	}
}

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
		out_serial = serial_port; //define the serial port to output from
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

void USART1_IRQHandler(void) {
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

