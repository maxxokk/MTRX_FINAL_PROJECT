# MTRX_FINAL_PROJECT  Team "Letron Games"
## Project Details:
The C Lab assignment is comprised of creating a carnival game. Our team decided on a shoots and hoop game similar to the basketball.the project consists of 4 modules which are  LIDAR, Flex potentiometer , LDR sensor and ........

## Group Members
- Max
- Nadeesha Perera (510585347)
- Tom Bray (510426888)
- Marco Jara

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

## Module 3: Flex Resistor and Buzzers module
### Summary
This code is designed to manage ADC readings from a flex resistor, control LEDs in order to visualize some readings , and operate buzzers using PWM. It enables clocks for various GPIO ports, configures them as inputs or outputs, and continuously reads ADC values from the flex resistor, beeping the buzzers based on the ADC input.

### Usage
Initialization:

- Enable necessary clocks for GPIO ports.
- Initialize board I/O, setting specific GPIO pins as outputs (LED's).
- Configure ADC and start continuous reading.
- Set up GPIO pins for output (e.g., PA3) and alternate functions for PWM (PA8, PA9).

Operation:
- Continuously read ADC values from Flex resistor.
- Update LEDs based on ADC values.
- Control buzzers using PWM based on specific ADC thresholds (when the FLex resistor gets a determinate value).
### Valid input
- ADC Channel: The code reads from a specified ADC channel (PA0).
- GPIO Pins: Configures specific pins (PA3, PA8, PA9) for output or alternate function.
- PWM Parameters: Frequency and duty cycle values for PWM control, designed for the buzzers

### Functions and modularity

Clock and Board Initialization:

enable_clocks(): Enables clocks for GPIOA, GPIOC, and GPIOE.
initialise_board(): Initializes GPIOE pins 8-15 as outputs. (LED's)

ADC Operations:

ReadADC(uint32_t channel): Reads ADC value from specified channel.
ContinuousReadSingleChannelADC(): Continuously reads ADC values, updates LEDs, and controls buzzers when input from Flex resistors.

GPIO Configuration:

setup_gpio_pa3(): Configures PA3 as a GPIO output to send a HIGH signal to the main board.
on_adc_value_change(): Handles ADC value changes (placeholder for user-defined actions).

PWM Configuration for Buzzers:

setup_timer2_pwm(): Sets up Timer 2 for PWM, one for each buzzer.
start_pwm_2_buzzers(uint32_t frequency, uint32_t duty_cycle): Starts PWM on PA8 and PA9 with specified frequency and duty cycle.
setup_gpio_PA8_PA9(): Configures PA8 and PA9 as output for alternate function mode for PWM.

### Testing
Initial Setup: Verify that clocks for GPIOA, GPIOC, and GPIOE are enabled.
GPIO Configuration: Check that GPIOE pins 8-15 are set as outputs and PA3 is set as output.
ADC Reading: Test ADC readings from specified channels and ensure accurate digital conversion.
LED Control: Verify LEDs on GPIOE are updated based on ADC values. Each time Flex resistor bends according to the game LD5 should be on.
PWM for Buzzers: Test PWM signals on PA8 and PA9 to ensure correct frequency and duty cycle operation.
On main board:
Flex Sensor Input: Check the input state of PA6 and ensure correct boolean output.

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

#### Player* setPlayer(Game* gamePtr, uint8_t p_index)
Given a pointer to a game, and a player index, this function will return a pointer to a player. If `p_index == -1`, then we set the pointer to the player who's turn it is. This is used in many of the functions, where we want to access the player who's turn it is.

#### void score(Game* gamePtr)
This is called when a player scores a basket. It will update the score for that player by adding 1 to their score and update the total distance by adding the distance of the last shot to the cumulative distance for that player. It will also update personal bests for score, single shot distance, and total distance.

#### struct Player;
The player holds all data relevant to a player in the game. This includes their score, shots taken, current shot distance, cumulative made shots distance, personal best score, personal best single shot distance, and personal best total cumulative distance.

#### struct Game
The struct game, holds two pointers to each player, the game mode, and whos turn it is.


#### Digital_Io Module 
### Testing
