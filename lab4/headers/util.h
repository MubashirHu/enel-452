#include <stdint.h>

void sysTickInit(void);
void delay(uint32_t delay); // A delay loop which blocks while a value is decremented to 0
void sleep_ms(uint16_t sleep_time);// A timer based delay loop. Resolution of 1 ms assumes SYSCLK = 24 Mhz
void sleep_us(uint16_t sleep_time_us);// A timer based delay loop.  Resolution of 1 us assumes SYSCLK = 24 Mhz
void clockInit(void); // Clock initialization providing a default SYSCLK of 24 MHz using the PLL and clock visibility of PLL/2 on PA8
void ledIOInit (void); // Configuration for PA5 as a push pull output with peripheral clocks for port A and port C turned on
void sw2led(void); // Read the USER pushbutton and illuminate the Green LED accordingly
void led_flash(void); // Use the delay routine to toggle the state of the Green LED on PA5
void onboardLEDconfig(int c);
void led_blink(void);


//GPIOA_BSRR Bit masks for NUCLEO green led on PA5

 #define NUC_GREEN_ON	0X00000020
 #define NUC_GREEN_OFF	0x00200000 