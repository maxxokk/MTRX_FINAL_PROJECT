#include "main.h"

// Function to get distance from LiDAR in centimeters
uint16_t get_distance(void);

void initLidar(void);

void SystemClock_Config(void);

static void MX_I2C1_Init(void);

static void MX_SPI1_Init(void);

static void MX_USB_PCD_Init(void);

static void MX_GPIO_Init(void);

void initialise_ptu_i2c(I2C_HandleTypeDef *i2c);


void Error_Handler(void);

