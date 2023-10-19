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
#define SENDDATA_TIMER 10
#define RECEIVEDATA_TIMER 10

extern volatile uint8_t DATA_RECEIVED_FLAG; // Declaration, no initialization

/** Configure and enable the device. */
void serialOpen(void);

/**
Undo whatever serial_open() did, setting the peripheral back to
its reset configuration.
*/
void serialClose(void);

/**
Send an 8-bit byte to the serial port, using the configured bit-rate, # of bits, etc.
Returns 0 on success and non-zero on failure.
@param b the 8-bit quantity to be sent.
@pre must have already called serial_open()
*/
int sendByte(uint8_t b);

/**
Gets an 8-bit character from the serial port, and returns it.
@pre must have already called serial_open()
*/
char getByte(void);

/**
Reads the USART_SR_RXNE register to determine whether dataReceivedFlag should be set
*/
void USART2_IRQHandler(void);

/**
Initialize the general purpose timer TIM2
Overall, this initialization function configures TIM2 to count at a rate of 10 kHz 
(0.1 ms per count), with a period of 1 ms. The timer is set to start counting from 0 
and will generate an update event every 1 ms.
*/
void initTIM2(void);

/**
Initialize the USART2 receive Interrupt and enable the handler in the NVIC table
*/
void initUSART2Interrupt(void);

#endif //USART_H

