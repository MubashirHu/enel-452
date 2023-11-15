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

#define CLI_TASK_PRIORITY 15
#define LCD_TASK_PRIORITY 15
#define ELEVATOR_CONTROL_TASK_PRIORITY 15

//QUEUE-SIZES
#define CLI_QUEUE_LENGTH 1
#define CLI_QUEUE_ITEM_SIZE sizeof(uint8_t)
	
#define ELEVATOR_UP_QUEUE_LENGTH 3
#define ELEVATOR_UP_QUEUE_ITEM_SIZE sizeof(uint16_t)
	
#define ELEVATOR_DOWN_QUEUE_LENGTH 3
#define ELEVATOR_DOWN_QUEUE_ITEM_SIZE sizeof(uint16_t)
		
#define LCD_QUEUE_LENGTH 3
#define LCD_QUEUE_ITEM_SIZE sizeof(ElevatorInformation)
	
#define IN_ELEVATOR_BUTTONS_QUEUE_LENGTH 1
#define IN_ELEVATOR_BUTTONS_QUEUE_ITEM_SIZE sizeof(uint16_t)
	
#define MAINTENANCE_MODE_QUEUE_LENGTH 1
#define MAINTENANCE_MODE_QUEUE_ITEM_SIZE sizeof(uint16_t)

/**
 * @brief Creating Queue function
 *
 * Creates queues for inter-task and interrupt communication. 
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
 * @brief LCD task that displays elevators current position and direction.
 *
 * @param void pointer that is passed in. Task prototype syntax.
 */
static void vLCDTask(void * parameters);

/**
 * @brief This Task is continously trying to manage the requests passed through the command line. 
 *
 * This task contains the buffer of type elevatorInformation that stores the relevant information 
 * for determining how to manage the requests from different floors.
 *
 * Responsible for the elevator logic. 
 * such as 
 * <1> Serving Up Requests
 * <2> Serving Down Requests
 * <3> Determining when to change direction
 * <4> Opening/Closing door sequence
 * <5> Send the elevator to the ground floor when maintenance/emergency button is pressed
 *
 * @param void pointer that is passed in. Task prototype syntax.
 */
static void vELEVATORCONTROLTask(void * parameters);

#endif //TIM_H
