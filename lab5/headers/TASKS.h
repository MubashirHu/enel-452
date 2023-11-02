/**
 * @file TASKS.h
 *
 * @brief Task Configuration for STM32 Communication with Host Terminal.
 *
 * This head holds the prototypes for Tasks that are only local to this module. As well as wrapper function
 * prototypes.
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#ifndef TIM_H
#define TIM_H

#define BLINKY_TASK_PRIORITY 5
#define CLI_TASK_PRIORITY 50
#define CLI_QUEUE_LENGTH 1
#define CLI_QUEUE_ITEM_SIZE sizeof(uint8_t)
#define BLINKY_QUEUE_LENGTH 1
#define BLINKY_QUEUE_ITEM_SIZE sizeof(uint16_t)

void createQueues(void);
void createTasks(void);
static void vCLITask(void * parameters);
static void vBlinkTask(void * parameters);
void USART2_IRQHandler(void);

#endif //TIM_H
