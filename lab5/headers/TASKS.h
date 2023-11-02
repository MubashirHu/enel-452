/**
 * @file TASKS.h
 *
 * @brief Task Configuration for STM32 Communication with Host Terminal.
 *
 * This head holds the prototypes for Tasks that are static (only local to this module). As well as wrapper function
 * prototypes.
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#ifndef TASKS_H
#define TASKS_H

#define BLINKY_TASK_PRIORITY 5
#define CLI_TASK_PRIORITY 5
#define CLI_QUEUE_LENGTH 1
#define CLI_QUEUE_ITEM_SIZE sizeof(uint8_t)
#define BLINKY_QUEUE_LENGTH 1
#define BLINKY_QUEUE_ITEM_SIZE sizeof(uint16_t)

/**
 * @brief Creating Task function
 *
 * Creates queues for tasks communication with length and item size defined in this header file
 */
void createQueues(void);

/**
 * @brief Creating Task function
 *
 * Creates tasks that need to be scheduled
 */
void createTasks(void);

/**
 * @brief CLI Task Function
 *
 * This function represents the Command Line Interface (CLI) task. It is responsible
 * for processing and handling user input from a CLI and performing associated actions.
 *
 * @param parameters A pointer to task-specific parameters (if needed).
 */
static void vCLITask(void * parameters);

/**
 * @brief CLI Task Function
 *
 * This function represents the tasks for blinking an LED at a defined speed. Speed can be
 * changed through task communication between vCLITask and vBlinkTask.
 *
 * @param parameters A pointer to task-specific parameters (if needed).
 */
static void vBlinkTask(void * parameters);

#endif //TIM_H
