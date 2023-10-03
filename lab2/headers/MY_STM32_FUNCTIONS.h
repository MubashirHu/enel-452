#include <stdint.h>
// Enumerations for GPIO configuration options

typedef enum {
	
	//In input mode MODE[1:0] = 00
    GPIO_CNF_INPUT_ANALOG_MODE = 0,
		GPIO_CNF_INPUT_FLOATING_INPUT = 1,
		GPIO_CNF_INPUT_WITH_PULLUP_OR_PULLDOWN = 2,
		GPIO_CNF_INPUT_RESERVED = 3,
	
	//In output mode MODE[1:0] > 00
    GPIO_CNF_OUTPUT_PP = 0,  // Push-Pull output
    GPIO_CNF_OUTPUT_OD = 1,  // Open-Drain output
    GPIO_CNF_OUTPUT_AF_PP = 2,      // Alternate Function Push-Pull
    GPIO_CNF_OUTPUT_AF_OD = 3,      // Alternate Function Open-Drain
	
    // Add other options as needed
} GPIO_Cnf;

typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT_10MHZ = 1,    
    GPIO_MODE_OUTPUT_2MHZ = 2,      
    GPIO_MODE_OUTPUT_50MHZ = 3      
} GPIO_Mode;

typedef enum {
    GPIO_PORTA,
    GPIO_PORTB,
    GPIO_PORTC,
		GPIO_PORTD,
		GPIO_PORTE,
} GPIO_Port;

typedef enum {
    GPIO_PIN_0 = 0,
    GPIO_PIN_1 = 1,
    GPIO_PIN_2 = 2,
		GPIO_PIN_3 = 3,
		GPIO_PIN_4 = 4,
		GPIO_PIN_5 = 5,
		GPIO_PIN_6 = 6,
		GPIO_PIN_7 = 7,
		GPIO_PIN_8 = 8,
		GPIO_PIN_9 = 9,
		GPIO_PIN_10 = 10,
		GPIO_PIN_11 = 11,
		GPIO_PIN_12 = 12,
		GPIO_PIN_13 = 13,
		GPIO_PIN_14 = 14,
		GPIO_PIN_15 = 15
    // Add other GPIO pins as needed
} GPIO_Pin;

void enablePort(char port);
void disablePort(char port);
void enableUSART(int usart);
void disableUSART (int usart);

// Function prototype
void configureGPIO(GPIO_Port port, GPIO_Pin pin, GPIO_Mode mode, GPIO_Cnf configuration);
