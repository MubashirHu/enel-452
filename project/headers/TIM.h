/**
 * @file TIM.h
 *
 * @brief Timer Configuration for STM32
 *
 * This file contains the prototypes for timer initialization and handlers. 
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#ifndef TIM_H
#define TIM_H
#include <stdint.h>
#define SENDDATA_TIMER 10
#define RECEIVEDATA_TIMER 10
extern volatile uint8_t TIM3_UPDATE_EVENT; 
extern volatile uint8_t TIM4_UPDATE_EVENT;

/**
Initialize the general purpose timer. accepts timer 2, 3 and 4. 
Overall, this initialization function configures timer to count at a rate of 10 kHz 
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

