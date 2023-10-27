/**
 * @file MY_STM_32_FUNCTIONS.h
 *
 * @brief Utility functions for simplified port and peripheral configuration.
 *
 * This header file contains a collection of utility functions designed to simplify the
 * configuration of ports and peripherals on STM32 microcontrollers. It provides convenient
 * methods for quickly enabling and disabling various peripherals and simplifying configuration.
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#ifndef MY_STM32_FUNCTIONS_H
#define MY_STM32_FUNCTIONS_H

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

/**
 * @brief Enables the specified port.
 *
 * This function is used to enable a specific port, which may be associated with hardware
 * peripherals or GPIO pins, for operation.
 *
 * @param port The identifier or name of the port to be enabled.
 */
void enablePort(char port);

/**
 * @brief Disables the specified port.
 *
 * This function is used to disable a specific port, which may be associated with hardware
 * peripherals or GPIO pins, to stop its operation.
 *
 * @param port The identifier or name of the port to be disabled.
 */
void disablePort(char port);

/**
 * @brief Enables the specified USART (Universal Synchronous Asynchronous Receiver-Transmitter).
 *
 * This function is used to enable a specific USART module for communication.
 *
 * @param usart The identifier or index of the USART module to be enabled.
 */
void enableUSART(int usart);

/**
 * @brief Disables the specified USART (Universal Synchronous Asynchronous Receiver-Transmitter).
 *
 * This function is used to disable a specific USART module to halt communication through it.
 *
 * @param usart The identifier or index of the USART module to be disabled.
 */
void disableUSART (int usart);

//WIP
/**
 * @brief Configures a GPIO (General Purpose Input/Output) pin with specific settings.
 *
 * This function configures a GPIO pin on a specified port with the given mode and configuration.
 * It allows you to set the mode of the pin (input, output, alternate function, etc.) and configure
 * additional settings as needed.
 *
 * @param port The GPIO port to which the pin belongs.
 * @param pin The GPIO pin number to configure.
 * @param mode The desired mode for the GPIO pin (e.g., input, output, alternate function).
 * @param configuration Additional configuration options for the GPIO pin (e.g., pull-up, pull-down).
 */
void configureGPIO(GPIO_Port port, GPIO_Pin pin, GPIO_Mode mode, GPIO_Cnf configuration);

#endif //MY_STM32_FUNCTIONS_H