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

#ifndef TIM_H
#define TIM_H
#include <stdint.h>
#define SENDDATA_TIMER 10
#define RECEIVEDATA_TIMER 10

/**
Initialize the general purpose timer TIM2
Overall, this initialization function configures TIM2 to count at a rate of 10 kHz 
(0.1 ms per count), with a period of 1 ms. The timer is set to start counting from 0 
and will generate an update event every 1 ms.
*/
void initTIM(int timer);
void initTIMInterrupt(int timer);

#endif //TIM_H

