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
 */
void CLI_Receive(uint8_t *pData, int* id);

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
Send 2 bytes of data for the arrows to prompt the user to type
*/
void sendPromptArrows(void);

/**
A new line with arrows to prompt the user to type
*/
void newPromptLine(void);

/**
Prepares the terminal to have 2 windows.
window 1: is constant
window 2: scrollable where commands can be enterred
*/
void prepareTerminal(void);
/**
clears the terminal
*/
void clearTerminal(void);

/**
moves the cursor to a given coordinate of row and col
*/
void placeCursor(int row, int col);

/**
Update status window content
returns the cursor back to the original position after updating status content
*/
void updateStatusWindow(void);

#endif // CLI_H
