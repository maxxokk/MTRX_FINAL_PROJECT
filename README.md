# MTRX_FINAL_PROJECT  Team "Letron Games"
## Project Details:
The C Lab assignment is comprised of creating a carnival game. Our team decided on a shoots and hoop game similar to the basketball.the project consists of 4 modules which are  LIDAR, Flex potentiometer , LDR sensor and ........

## Group Members
- Max
- Nadeesha Perera (510585347)
- Tom
- Marco

## Roles & Responsibilities
### Max:
- 3D printing ( board,hoop and shooter)
- Scoreboard(GUI)
- Documentation 
- Integration of modules

### Tom:
- Implementation of LDR sensor module
- Documentation 

### Nadeesha Perera:
- Implementation of LIDAR Module
- Documentation 

### Marco:
- Implementation of Flex Resistor module
- Implementation of Buzzers module.
- Documentation 

## Design Breakdown
## Module 1: LIDAR module
### Summary
This task involved creating a serial comnmunication between the pantilt unit and micro controller to use only the lidar sensor to output the distance beteen the shooter and the board.

### Usage
Sensor project was created to conduct the LIDAR module. Consists of main.c , serial.c and distance.c.  Main.c initialises board and enables the clocks and Initialize the uart,all configured peripherals. It also intialises the HAl drivers to output PVM signal and capture it from the LIDAR sensor.Next it calls the get_distance function which is created in the distance.c file and outputs the values returning from that function as the distance.

In the get_distance function , it commands the LiDAR to perform a measurement, Reads the status register to check if measurement is done,Read the measured distance from the LiDAR registers and Combine the MSB and LSB to form the distance in cm and output the value as distance


### Valid input
Valid input is running the project and calling on the function

### Functions and modularity
The project has one module – LIDAR Module

#### LIDAR Module 

- EXTIO_IRQHandler: function to handle the button handler which is a pointer to the reset state
- SystemClock_Config,MX_I2C1_Init,MX_SPI1_Init: function toinitialiase and enable Cockes,GPIOA, Button pin, lidar sensor
- get_distance : function to get PWM signals from LIDAR sensor and calculate distance and save to the variable 'distance'

### Testing
- check if the serial output is working by sending it to a terminal (putty)
- holding a board  away from the lidar snesor andd checking if the measurement is accurate
- repeating the above task but slowly moving further away
- checking the minimum and maximum diatcance for accarcy of the sensor
minimum - 30cm    maximimum 300cm





## Module 2: LDR Sensor module
### Summary
LDR is used to determine whether the ball is in the holder. If the ball is in the holder, then a basket will not count for that player, as this would be impossible for the ball to be both in the hoop and in the holder at the same time. It also prevents certain illegal moves, where the flex sensor is activated but the ball is in the holder.
### Usage
The LDR is attached to the ball holder. This allows us to know at all times when the ball is in the holder. The code used is partly taken from W07-C-Analog-input of the MTRX2700 Github. It uses the ContinuousReadSingleChannelADC function to determine the state of the system based off the voltage of PA1 that is connected to the LDR. From the LDR input, the function also implements a trigger value, above which is high, and below is low. There is also bounded conditions to prevent it flickering between HIGH and LOW. When it is considered HIGH, the output pin PA4 is set also to high.
### Valid input
Valid input is an analog signal that goes through an LDR to PA1.
### Functions and modularity
#### void initialise_board()
Initialises the board for PA1 as input and PA4 as output. The LED output registers are also initialised for debugging purposes.

#### void ContinuousReadSingleChannelADC()
This function initialises and calibrates the ADC, and reads the input from PA1. It then sets a trigger point based off the initial light on the LDR, then sets boundaries for ADC->DR value considered HIGH and LOW. It then outputs this high or low value through PA4, which is used in the main game loop.


### Digital_Io Module 
#### void ContinuousReadSingleChannelADC()
The function uses the input from PA1, and gives output through PA4 on the STM32f3Discovery.

### Testing
To test the ADC performance, an LED in the STM32f3Discovery is turned on for HIGH input, and off for LOW input. This allows us to test the performance of input for the LDR, which tells us when the system thinks the ball is in the holder. An output LED on a breadboard can also be used to test the output of PA4, which should be HIGH, whenever the ADC tells us its HIGH. This tells us when the system is outputting HIGH and LOW into the game loop.

## Module 3: flexi potentiometer module
### Summary
### Usage
### Valid input
### Functions and modularity
#### Digital_Io Module 
### Testing

## Module 4: LIDAR module
### Summary
### Usage
### Valid input
### Functions and modularity
#### Digital_Io Module 
### Testing


## Integration of modules
### Summary
### Usage
### Valid input
### Functions and modularity
#### Digital_Io Module 
### Testing
