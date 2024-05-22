#ifndef SERIAL_PORT_HEADER
#define SERIAL_PORT_HEADER

#define BUFFER_SIZE 64
#define OUT_BUFFER_SIZE 2048

#include <stdint.h>

// Defining the serial port struct, the definition is hidden in the
// c file as no one really needs to know this.
struct _SerialPort;
typedef struct _SerialPort SerialPort;


// make any number of instances of the serial port (they are extern because
//   they are fixed, unique values)
extern SerialPort USART1_PORT;


// The user might want to select the baud rate
enum {
  BAUD_9600,
  BAUD_19200,
  BAUD_38400,
  BAUD_57600,
  BAUD_115200
};

extern void (*received)();
extern void (*inpDetect)();
extern void (*demonstrate)();
extern uint8_t inpStr[2][BUFFER_SIZE];
 
// SerialInitialise - initialise the serial port
// Input: baud rate as defined in the enum
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t) );


// SerialOutputChar - output a char to the serial port
//  note: this version waits until the port is ready (not using interrupts)
// Input: char to be transferred
void SerialOutputChar(uint8_t, SerialPort *serial_port);

void finished_transmission(uint32_t bytes_sent);


// SerialOutputString - output a NULL TERMINATED string to the serial port
// Input: pointer to a NULL-TERMINATED string (if not null terminated, there will be problems)
void SerialOutputString(uint8_t *pt, SerialPort *serial_port);

void SerialInput(uint8_t data[2][BUFFER_SIZE], SerialPort *serial_port);

void ClearInput(SerialPort *serial_port);

void DoubleBuffer(uint8_t data[BUFFER_SIZE], int NumChar);

void EchoInput(uint8_t data[BUFFER_SIZE], int NumChar);

void USART1_EXTI25_IRQHandler(void);

void enable_USART_interrupt(void);

void finished_transmission(uint32_t bytes_sent);

void disable_tx_interrupt(void);

void enable_tx_interrupt(void);

void SerialOut(uint8_t *data, SerialPort *serial_port);

void SerialOut_IRQ(void);
 
#endif
