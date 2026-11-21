#pragma once

/*
This is the ONLY file that should contain GPIO numbers.
Later this file moves to components/board/include/pins.h.

 Wire colors on the breadboard:
    3V3 red | GND black | SCK yellow | MOSI green | CS blue | DC white
    RESET gray | backlight orange | button signals brown | button grounds black
*/

#include "hal/gpio_types.h"
#include "hal/spi_types.h"

/* hardware SPI controller that drives the LCD display */
#define LCD_SPI_HOST   SPI2_HOST
 
#define PIN_LCD_CS     GPIO_NUM_10   /* blue   | module pin 3 (CS)        */
#define PIN_LCD_MOSI   GPIO_NUM_11   /* green  | module pin 6 (SDI/MOSI)  */
#define PIN_LCD_SCK    GPIO_NUM_12   /* yellow | module pin 7 (SCK)       */
#define PIN_LCD_MISO   GPIO_NUM_NC   /* not wired (display only). SDO would use GPIO13 */
#define PIN_LCD_DC     GPIO_NUM_14   /* white  | module pin 5 (DC/RS)     */
#define PIN_LCD_RST    GPIO_NUM_15   /* gray   | module pin 4 (RESET)     */



/* Backlight (module pin 8, LED).
Right now the orange wire goes to the 3V3 rail,
so the backlight is always on.
When PWM brightness is added, move that wire to this GPIO.
*/
#define PIN_LCD_BL     GPIO_NUM_16   /* orange */
 
/* Touch pins (module pins 10 to 14) are not used. */
 
// Buttons
 
/* Each button connects a GPIO to GND. Use the internal 
pull-up resistor, so pressed = 0 */
#define PIN_BTN_1      GPIO_NUM_4    /* brown | previous / up / decrease */
#define PIN_BTN_2      GPIO_NUM_5    /* brown | next / down / increase   */
#define PIN_BTN_3      GPIO_NUM_6    /* brown | brightness / select      */
#define PIN_BTN_4      GPIO_NUM_7    /* brown | menu / back / cancel     */
 
#define PIN_BTN_COUNT        4
#define BTN_PRESSED_LEVEL    0