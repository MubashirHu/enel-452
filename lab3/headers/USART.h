/**
 * @file USART.h
 *
 * @brief USART2 Configuration for STM32 Communication with Host Terminal.
 *
 * This header file enables the USART2 port, which is used for communication between
 * an STM32 board and a host terminal. It utilizes the same communication interface
 * as the ST-Link debugger. The file also includes functions previously used in ENEL 351.
 *
 * @note This file may contain functions from ../351/UART.h.
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#ifndef USART_H
#define USART_H
#include <stdint.h>

extern volatile uint8_t dataReceivedFlag; // Declaration, no initialization

/** Configure and enable the device. */
void serial_open(void);

/**
Undo whatever serial_open() did, setting the peripheral back to
its reset configuration.
*/
void serial_close(void);

/**
Send an 8-bit byte to the serial port, using the configured bit-rate, # of bits, etc.
Returns 0 on success and non-zero on failure.
@param b the 8-bit quantity to be sent.
@pre must have already called serial_open()
*/
int sendbyte(uint8_t b, uint32_t Timeout);

/**
Gets an 8-bit character from the serial port, and returns it.
@pre must have already called serial_open()
*/
char getbyte(void);


/**
Reads the USART_SR_RXNE register to determine whether dataReceivedFlag should be set
*/
void USART2_IRQHandler(void);

/**
Initialize the general purpose timer(TIM2 to TIM5)
*/
void init_general_purpose_timer(uint8_t timer);

#endif //USART_H

