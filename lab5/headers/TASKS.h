
#define BLINKY_TASK_PRIORITY 5
#define CLI_TASK_PRIORITY 5
#define CLI_QUEUE_LENGTH 1
#define CLI_QUEUE_ITEM_SIZE sizeof(uint8_t)
#define BLINKY_QUEUE_LENGTH 1
#define BLINKY_QUEUE_ITEM_SIZE sizeof(uint16_t)

void createQueues(void);
void createTasks(void);
static void vCLITask(void * parameters);
static void vBlinkTask(void * parameters);