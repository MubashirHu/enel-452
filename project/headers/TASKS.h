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

//PRIORITIES

#define BLINKY_TASK_PRIORITY 5

//INPUTS
#define CLI_TASK_PRIORITY 15

//OUTPUTS
#define LCD_TASK_PRIORITY 15
#define MUX_TASK_PRIORITY 15

//CONTROL LOGIC
#define ELEVATOR_CONTROL_TASK_PRIORITY 15

// QUEUE SIZES
#define CLI_QUEUE_LENGTH 1
#define CLI_QUEUE_ITEM_SIZE sizeof(uint8_t)
	
#define BLINKY_QUEUE_LENGTH 1
#define BLINKY_QUEUE_ITEM_SIZE sizeof(uint16_t)
	
#define MUX_QUEUE_LENGTH 5
#define MUX_QUEUE_ITEM_SIZE sizeof(uint16_t)

#define ELEVATOR_UP_QUEUE_LENGTH 5
#define ELEVATOR_UP_QUEUE_ITEM_SIZE sizeof(uint16_t)
	
#define ELEVATOR_DOWN_QUEUE_LENGTH 5
#define ELEVATOR_DOWN_QUEUE_ITEM_SIZE sizeof(uint16_t)
	
#define CURRENT_FLOOR_QUEUE_LENGTH 5
#define CURRENT_FLOOR_QUEUE_ITEM_SIZE sizeof(uint16_t)
	
#define LCD_QUEUE_LENGTH 5
#define LCD_QUEUE_ITEM_SIZE sizeof(ElevatorInformation)
	
#define IN_ELEVATOR_BUTTONS_QUEUE_LENGTH 1
#define IN_ELEVATOR_BUTTONS_QUEUE_ITEM_SIZE sizeof(uint16_t)

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

static void vLCDTask(void * parameters);

static void vMUXTask(void * parameters);

static void vELEVATORCONTROLTask(void * parameters);

#endif //TIM_H
