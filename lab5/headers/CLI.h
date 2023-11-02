/**
 * @file CLI.h
 *
 * @brief Command Line Interface (CLI) Prototypes and Data Buffers for Host Terminal Communication.
 *
 * This header file includes prototypes and data buffers essential for implementing a Command Line Interface (CLI).
 * The CLI allows communication with the host terminal, which can be applications like Putty or Teraterm. 
 * The functions defined in this header file facilitate the exchange of data and commands between the microcontroller
 * and the host terminal, enabling interactive control and feedback.
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */
 
#ifndef CLI_H
#define CLI_H
#define BACKSPACE 8
#define NEW_LINE_FEED 10
#define CARRIAGE_RETURN 13
#define SPACE 32
#define CLEAR_TERMINAL 0
#define SAVE_CURSOR_POSITION 1
#define RESTORE_CURSOR_POSITION 2
#define SET_SCROLLABLE_ROW 3

#include <stdint.h>

/**
 * @brief Transmits data from the microcontroller to the host one byte at a time.
 *
 * This function sends the data in the provided buffer to the host or external device by
 * transmitting one byte at a time. It is commonly used for serial communication to send
 * data in a byte-by-byte fashion.
 *
 * @param pData Pointer to the data buffer containing the data to be transmitted.
 * @param Size The number of bytes to transmit from the buffer.
 */
void CLI_Transmit(uint8_t *pData, uint16_t Size);

/**
 * @brief Receives data into the buffer and processes it for command input.
 *
 * This function reads data from the input source, populates the provided data buffer, and
 * processes it in real-time for command input. It handles backspacing, carriage return, and
 * newline characters to facilitate command input.
 *
 * @param pData Pointer to the buffer where received data is stored.
 * @param id pointer to the elementID of the buffer where the received data is stored.
 * @return returns 0 if normal execution and non-zero when led-speed is parsed
 */
int CLI_Receive(uint8_t *pData, int* id, int* speed);

/**
 * @brief Parses the data stored in the buffer pointed to by the pData pointer and performs corresponding actions.
 *
 * This function processes incoming data and checks for specific command strings. Depending on the command,
 * it sends responses and may trigger certain actions, such as controlling an on-board LED or providing help information.
 *
 * @param pData Pointer to the data buffer containing the received data.
 * @param Size Size of the data in the buffer.
 */
int parseReceivedData(uint8_t *pData, int Size);

/**
 * @brief Send two bytes of data to display arrow prompts for user input.
 */
void sendPromptArrows(void);

/**
 * @brief Display a new line with arrow prompts to guide the user for input.
 */
void newPromptLine(void);

/**
 * @brief Prepare the terminal for a two-window layout.
 *
 * This function sets up the terminal with two windows:
 * - Window 1: A constant window.
 * - Window 2: A scrollable window where commands can be entered.
 */
void prepareTerminal(void);

/**
 * @brief Move the cursor to the specified row and column coordinates.
 *
 * @param row The target row to move the cursor to.
 * @param col The target column to move the cursor to.
 */
void placeCursor(int row, int col);

/**
 * @brief Update the content of the status window and restore the cursor position.
 */
void updateStatusWindow(void);

/**
 * @brief Send an ANSI escape code represented by a directive over USART2.
 *
 * This function sends a string of characters over USART2, which corresponds to an ANSI escape code.
 *
 * @param ANSI The ANSI code directive representing the ANSI command.
 */
void sendEscapeAnsi(uint16_t ANSI);

#endif // CLI_H
