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
- Implementation of Flex potentiometer modules
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
### Usage
### Valid input
### Functions and modularity
#### Digital_Io Module 
### Testing

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