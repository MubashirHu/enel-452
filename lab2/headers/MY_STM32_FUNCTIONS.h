#include <stdint.h>
// Enumerations for GPIO configuration options
typedef enum {
    GPIO_SPEED_FREQ_LOW,
    GPIO_SPEED_FREQ_MEDIUM,
    GPIO_SPEED_FREQ_HIGH,
    // Add other options as needed
} GPIO_Speed;

typedef enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT_PP,  // Push-Pull output
    GPIO_MODE_OUTPUT_OD,  // Open-Drain output
    GPIO_MODE_AF_PP,      // Alternate Function Push-Pull
    GPIO_MODE_AF_OD,      // Alternate Function Open-Drain
    // Add other options as needed
} GPIO_Mode;

typedef enum {
    GPIO_PORTA,
    GPIO_PORTB,
    GPIO_PORTC,
    // Add other GPIO ports as needed
} GPIO_Port;

typedef enum {
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    // Add other GPIO pins as needed
} GPIO_Pin;


void enablePort(char port);
void enableUSART(int usart);

// Function prototype
int configureGPIO(GPIO_Port port, GPIO_Pin pin, GPIO_Mode mode, GPIO_Speed speed)
{
	if(port == GPIO_PORTA)
	{
		
	}
}
