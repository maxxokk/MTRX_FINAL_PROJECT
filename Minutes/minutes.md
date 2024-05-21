Lab 1 Minutes

# MTRX Group 5 - "LeTron James"
##  Date - 24/04/2024
### Present: Max, Marco, Nadeesha, Tom
### Apologies: None

### Meeting Purpose: Group Creation and Formulation of Idea

### Previous Actions: N/A

### Current Items: Brainstorming of ideas, distribution of tasks, creating GitHub repository.

Summary of Meeting: We are going to create a mini basketball shooter minigame, which can sense how far away the user is shooting from, as well as how many the shooter has made in a row, within a certain time frame.

New Items:  Tom will complete LDR sensor in "shooter". Max will do 3D printing of basketball, hoop, and shooter (shooting mechanism yet to be decided). Max will also do the scoreboard module (serial to Raspberry Pi connected to 16x16 UnicornHAT). Nadeesha will do the LIDAR distance module. Marco will complete flexible potentiometer module (for detecting ball going through the hoop)

### Next Meeting Date: 10/04/2024

Lab 2 Minutes

# MTRX Group 5 - "LeTron James"
##  Date - 1/05/2024
### Present: Max (zoom), Marco (zoom), Nadeesha, Tom
### Apologies: None

### Meeting Purpose: Update on Progress with Individual Modules

### Previous Actions: N/A

### Current Items:
Max - 3D printing, scoreboard
Marco - flex potentiometer
Tom - LDR sensor
Nadeesha - LIDAR distance measurement

### New Items:
#### 3D printing: 
Max has signed up for AMME fablab induction, is waiting to be invited to canvas page. WIll explore possibility of using 3D printer in Susal Wakil building.
#### Scoreboard: 
Scoreboard is functionally complete, with some cosmetic changes needed.
#### LIDAR: 
LIDAR is reading, but Nadeesha has had difficulties outputting distance measurement via serial.
#### LDR: 
LDR is functional - if covered up, then the LED in breadboard turns off. Connection to STM32 works too. 
#### Flex Potentiometer: 
Marco's flex potentiometer was not working properly - he will source and buy another one before the next meeting. 

### Next Meeting Date: 8/05/2024

Lab 3 Minutes

# MTRX Group 5 - "LeTron James"
##  Date - 8/05/2024
### Present: Max, Tom, Nadeesha, Marco
### Apologies: 

### Meeting Purpose: Update on Individual Modules

### Previous Actions: Update on Individual Modules

### Current Items:
Max - 3D printing, scoreboard, timer
Marco - flex potentiometer
Tom - LDR sensor, main C code
Nadeesha - LIDAR distance measurement

### New Items:
#### 3D printing: Max has booked in a session at the Susan Wakil 3D printers. 
#### Scoreboard: 
The scoreboard has yet to be cosmetically improved as Max has been busy with MECH3660 project. Max has also been provided with a small LCD screen that can be used as a timer, as Tkinter is not suited to constantly refreshing a clock. This LCD screen will connect up to Max's Raspberry Pi and will communicate with the STM32 via serial. This is a new, separate module from the scoreboard.
#### Timer: 
As above, Max will set up the Raspberry Pi LCD screen to display a timer for shooting via serial communication from the STM32 to the Raspberry Pi. 
#### LIDAR: 
Nadeesha has not been able to get output to PuTTy. Max will push serial IO code to GitHub. Nadeesha has said that he will be able to get help from a friend over the weekend, to assist him with the LIDAR functionality and modularisation. 
#### LDR: 
Tom will look into using the STM32 to set the threshold, to enable configuration. 
#### Flex Potentiometer: 
Marco is still waiting on delivery. To demonstrate functionality, he is using a regular potentiometer. It currently employs a polling approach, which will not be suitable as the user will not always make the shot, therefore the program will not stop polling. Marco will investigate an interrupt based approach. This can be achieved by digitising the signal from the potentiometer on a breadboard, so that when the input pin reads HIGH, then an interrupt will fire. For the LDR, a polling approach would be appropriate as the player MUST place the ball in the holder before each shot, while they are not so obligated to make the shot, which is why the flex must be interrupt based. Another approach is to use one STM board to manage the threshold and the analogue input, which then outputs via GPIO for the main STM32 to input via its own GPIO port. 
#### Main C code: 
Tom is working on bare-bones scaffold of the main C code, which will allow us to sub in the individual modules when they are completed. Max will also work on this in the coming week. 

### Next Meeting Date: 15/05/2024

Lab 4.1 Minutes (Meeting outside of lab time)

# MTRX Group 5 - "LeTron James"
##  Date - 20/05/2024
### Present: Max, Tom, Nadeesha, Marco
### Apologies: 

### Meeting Purpose: Combining Individual Modules (Integration)

### Previous Actions: Update on Individual Modules

### Current Items:
Max - 3D printing, scoreboard, main C code
Marco - flex potentiometer
Tom - LDR sensor
Nadeesha - LIDAR distance measurement

### New Items:
#### 3D printing: 
Ramp is currently being printed, and will be picked up tomorrow for our next meeting
#### Scoreboard: 
The serial output to the scoreboard is now implemented using a packet of information. This needs to be fully implemented, and also needs some aesthetic polish. 
#### LIDAR: 
getDistance and setDistance functions are now implemented and working. Someone in the group will bring in a piece of cardboard which will go behind the backboard to ensure the sensor acquires an accurate measurement. 
#### LDR: 
Tom has function which uses polling approach, and is working on a function that will output whether the ball is on the stand or not (as a Boolean). 
#### Flex Potentiometer: 
Flex sensor module is implemented. 
#### Main C code: 
Standard game mode has been programmed. Other game modes to be completed before next meeting. 

### Next Meeting Date: 21/05/2024

Lab 4 Minutes

# MTRX Group 5 - "LeTron James"
##  Date - 15/05/2024
### Present: Max, Tom, Nadeesha, Marco
### Apologies: 

### Meeting Purpose: Update on Individual Modules

### Previous Actions: Update on Individual Modules

### Current Items:
Max - 3D printing, scoreboard, main C code, timer
Marco - flex potentiometer
Tom - LDR sensor, main C code
Nadeesha - LIDAR distance measurement

### New Items:
#### 3D printing: 
First 3D print has been completed, second is booked in for Monday. The exact operation of the game has changed - instead of the user flicking the ball directly into the hoop, they will now flick it along the ground/table to a ramp which will lead up into a short 3cm high hoop. This is because it was too difficult to give the ball sufficient height. 
#### Scoreboard: 
Scoreboard serial has been implemented. Some aesthetic polish still needed. 
#### Timer: 
Timer will not happen as the nature of the game means that the time taken between shots is not under the control of the user. Rushing can also cause the ramp/hoop to move, which would defeat the purpose of the distance measurement. 
#### LIDAR: 
Distance measurements are successfully coming through to PuTTy. Nadeesha will still need to polish off the modularity, and encapsulate all the code in a single function.
#### LDR: 
Unfortunately Tom's board was broken this week, but the LDR module is largely complete. He will finish the ADC component in the STM32 before Monday. 
#### Flex Potentiometer: 
Flex readings are coming through to the STM32. 
#### Main C code: 
Max and Tom will continue on the main integration code this week. Scaffold should be complete before Monday to enable 'plugging in' the individual modules. 

### Next Meeting Date: 20/05/2024
