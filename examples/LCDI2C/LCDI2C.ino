#include <LCDi2c.h>

#define LCD_ROWS    2
#define LCD_COLUMNS 16

// special chars
uint8_t upArrow[8] = {  
	0b00100,
	0b01010,
	0b10001,
	0b00100,
	0b00100,
	0b00100,
	0b00000,
	};

uint8_t downArrow[8] = {
	0b00000,
	0b00100,
	0b00100,
	0b00100,
	0b10001,
	0b01010,
	0b00100,
	};

uint8_t rightArrow[8] = {
	0b00000,
	0b00100,
	0b00010,
	0b11001,
	0b00010,
	0b00100,
	0b00000,
	};

uint8_t leftArrow[8] = {
	0b00000,
	0b00100,
	0b01000,
	0b10011,
	0b01000,
	0b00100,
	0b00000,
	};

float data = 0.1f;

LCDi2c lcd(0x27, Wire);

void setup() {
  lcd.begin(LCD_ROWS, LCD_COLUMNS);
  lcd.create(0, downArrow);
	lcd.create(1, upArrow);
	lcd.create(2, rightArrow);
	lcd.create(3, leftArrow);
  }

void loop() {
	lcd.cls();
	lcd.printf("hello world %f", data);
	// print user chars
	lcd.character(2, 1, 0);
	lcd.character(2, 3, 1);
	lcd.character(2, 5, 2);
	lcd.character(2, 7, 3);

	delay(1000);
	lcd.display(DISPLAY_OFF);
	delay(1000);
	lcd.display(DISPLAY_ON);
	delay(1000);
	lcd.display(BACKLIGHT_OFF);
	delay(1000);
	lcd.display(BACKLIGHT_ON);
	delay(1000);
	lcd.display(CURSOR_ON);
	delay(1000);
	lcd.display(BLINK_ON);
	delay(1000);
	lcd.display(BLINK_OFF);
	delay(1000);
	lcd.display(CURSOR_OFF);
  delay(1000);
	// scroll 16 positions to the right
	for (uint8_t pos = 0; pos < 16; pos++) {
		lcd.display(SCROLL_RIGHT); // scroll one position to right
		delay(500); // step time
		}
	// scroll 16 positions to the left
	for (uint8_t pos = 0; pos < 16; pos++) {
		lcd.display(SCROLL_LEFT); // scroll one position to left
		delay(500); // step time
		}
	delay(500);
	lcd.cls();
	lcd.locate(1, 1);
	lcd.printf("hello line 1");
	delay(500);
	lcd.locate(2, 1);
	lcd.printf("hello line 2");
	delay(500);
	// clear parts and rows
	lcd.clp(1, 1, 3);
	delay(500);
	lcd.clp(2, 4, 3);
	delay(500);
	lcd.clp(1, 6, 3);
	delay(500);
	lcd.clp(2, 10, 3);
	delay(500);
	lcd.clr(1);
	delay(500);
	lcd.clr(2);
	delay(500);
	}
