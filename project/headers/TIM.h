/**
 * @file TIM.h
 *
 * @brief USART2 Configuration for STM32 Communication with Host Terminal.
 *
 * This header file enables the USART2 port, which is used for communication between
 * an STM32 board and a host terminal. It utilizes the same communication interface
 * as the ST-Link debugger. The file also includes functions previously used in ENEL 351.
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#ifndef TIM_H
#define TIM_H
#include <stdint.h>
#define SENDDATA_TIMER 10
#define RECEIVEDATA_TIMER 10
extern volatile uint8_t TIM3_UPDATE_EVENT; // Declaration, no initialization
extern volatile uint8_t TIM4_UPDATE_EVENT; // Declaration, no initialization

/**
Initialize the general purpose timer TIM2
Overall, this initialization function configures TIM2 to count at a rate of 10 kHz 
(0.1 ms per count), with a period of 1 ms. The timer is set to start counting from 0 
and will generate an update event every 1 ms.
*/
void initTIM(int timer);

/**
Initialize the NVIChandler as well as the register for Timer interupt
*/
void initTIMInterrupt(int timer);

/**
Set the Time period for TIM2 to TIM4
*/
void configTIM(int timer, uint16_t time_period);

/**
prototype for the NVIC TIM3 handler
*/
void TIM3_IRQHandler(void);

/**
prototype for the NVIC TIM4 handler
*/
void TIM4_IRQHandler(void);

#endif //TIM_H

