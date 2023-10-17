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

/** Configure and enable the device. */
void serial_open(void);
/**
Undo whatever serial_open() did, setting the peripheral back to
its reset configuration.
2
*/
void serial_close(void);
/**
Send an 8-bit byte to the serial port, using the configured
bit-rate, # of bits, etc. Returns 0 on success and non-zero on
failure.
@param b the 8-bit quantity to be sent.
@pre must have already called serial_open()
*/
void sendbyte(char b);
/**
Gets an 8-bit character from the serial port, and returns it.
@pre must have already called serial_open()
*/
char getbyte(void);

void USART2_IRQHandler(void);

#endif //USART_H

