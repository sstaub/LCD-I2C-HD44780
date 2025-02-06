
/* Arduino TextLCD Library, for a 4-bit LCD based on HD44780
 * Copyright (c) 2025, sstaub
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// for float support add in platformio.ini following line 
// #build_flags = -Wl,-u,vfprintf -lprintf_flt -lm

#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "Arduino.h"
#include "Print.h"
#include "Wire.h"

// commands
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_RETURN_HOME 0x02
#define LCD_ENTRY_MODE_SET 0x04
#define LCD_DISPLAY_CONTROL 0x08
#define LCD_CURSOR_SHIFT 0x10
#define LCD_FUNCTION_SET 0x20
#define LCD_SET_CGRAM_ADDR 0x40
#define LCD_SET_DDRAM_ADDR 0x80

// flags for display entry mode
#define LCD_ENTRY_RIGHT 0x00
#define LCD_ENTRY_LEFT 0x02
#define LCD_ENTRY_SHIFT_INCREMENT 0x01
#define LCD_ENTRY_SHIFT_DECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAY_ON 0x04
#define LCD_DISPLAY_OFF 0x00
#define LCD_CURSOR_ON 0x02
#define LCD_CURSOR_OFF 0x00
#define LCD_BLINK_ON 0x01
#define LCD_BLINK_OFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAY_MOVE 0x08
#define LCD_CURSOR_MOVE 0x00
#define LCD_MOVE_RIGHT 0x04
#define LCD_MOVE_LEFT 0x00

// flags for function set
#define LCD_8BIT_MODE 0x10
#define LCD_4BIT_MODE 0x00
#define LCD_2_LINE 0x08
#define LCD_1_LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#define LCD_BACKLIGHT_ON 0x08
#define LCD_BACKLIGHT_OFF 0x00

#define EN 0b00000100  // Enable bit
#define RW 0b00000010  // Read/Write bit
#define RS 0b00000001  // Register select bit

#define OFFSET 1

typedef enum {
	DISPLAY_ON,
	DISPLAY_OFF,
	CURSOR_ON,
	CURSOR_OFF,
	BLINK_ON,
	BLINK_OFF,
	SCROLL_LEFT,
	SCROLL_RIGHT,
	LEFT_TO_RIGHT,
	RIGHT_TO_LEFT,
	AUTOSCROLL_ON,
	AUTOSCROLL_OFF,
	BACKLIGHT_ON,
	BACKLIGHT_OFF,
	} dispmode_t;

class LCDi2c : public Print {
	public:
		/**
		 * @brief construct a new LCD object
		 * 
		 * @param address I2C address, default 0x27
		 * @param wire interface, e.g. Wire, Wire1 ..., default Wire
		 */
		LCDi2c (uint8_t i2cAddr = 0x27, TwoWire &wire = Wire);

		/**
		 * @brief start using the lcd display
		 * 
		 * @param rows default 2
		 * @param columns default 16
		 * @param dotsize default LCD_5x8DOTS some displays have also LCD_5x10DOTS
		 */
		void begin(uint8_t rows = 2, uint8_t columns = 16, uint8_t dotsize = LCD_5x8DOTS);

		/**
		 * @brief clear display, reset cursor position first row / column
		 * 
		 */
		void cls();

		/**
		 * @brief clear row, set cursor position of the row to the first position
		 * 
		 * @param row number of the row
		 */
		void clr(uint8_t row);

		/**
		 * @brief clear part, set cursor position of the row to first column
		 * 
		 * @param row number of the row
		 * @param column number of the column
		 * @param numbers of chars to delete
		 */
		void clp(uint8_t row, uint8_t column, uint8_t numbers);

		/**
		 * @brief set cursor position to home position
		 * 
		 */
		void home();

		/**
		 * @brief set display modes
		 * 
		 * @param mode 
		 * - DISPLAY_ON Turn the display on
		 * - DISPLAY_OFF Turn the display off
		 * - CURSOR_ON Turns the underline cursor on
		 * - CURSOR_OFF Turns the underline cursor off
		 * - BLINK_ON Turn the blinking cursor on
		 * - BLINK_OFF Turn the blinking cursor off
		 * - SCROLL_LEFT These command scroll the display without changing the RAM
		 * - SCROLL_RIGHT These commands scroll the display without changing the RAM
		 * - LEFT_TO_RIGHT This is for text that flows Left to Right
		 * - RIGHT_TO_LEFT This is for text that flows Right to Left
		 * - AUTOSCROLL_ON This will 'right justify' text from the cursor
		 * - AUTOSCROLL_OFF This will 'left justify' text from the cursor
		 */
		void display(dispmode_t mode);

		/**
		 * @brief set the cursor to a given position
		 * 
		 * @param row number of the row
		 * @param column number of the column
		 */
		void locate(uint8_t row, uint8_t column);

		/**
		 * @brief create a user defined char object 
		 * 
		 * @param location position 0 .. 7 available
		 * @param charmap is a 8 byte array (5x7)
		 */
		void create(uint8_t location, uint8_t charmap[]); 

		/**
		 * @brief writes a single char to a given position
		 * 
		 * @param row number of the row
		 * @param column number of the column
		 * @param c char to print
		 */
		void character(uint8_t row, uint8_t column, char c);

		/**
		 * @brief prints a formated string
		 * 
		 * @param format 
		 * @param ... 
		 */
		void printf(const char *format, ...);

		// used for Print class
		using Print::write;
		virtual size_t write(uint8_t);

	private:
		void setRowOffsets(int row1, int row2, int row3, int row4);
		void send(uint8_t value, uint8_t mode);
		void write4bits(uint8_t value);
		void expanderWrite(uint8_t data);
		void command(uint8_t value);
		void pulseEnable(uint8_t value);

		TwoWire *wire;
		uint8_t i2cAddr;
		uint8_t displayfunction;
		uint8_t displaycontrol;
		uint8_t displaymode;
		uint8_t backlight;
		uint8_t columns;
		uint8_t rows;
		uint8_t rowOffsets[4];
	};

#endif
