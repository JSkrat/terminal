//#include "stm32g0xx_ll_gpio.h"
#include <stdbool.h>
#include <stdint.h>
#include "st7920.h"

void delayMicroseconds(uint32_t microseconds);

#define LCD_DATA_PORT GPIOA
#define LCD_RS_PORT GPIOC
#define LCD_RS_PIN LL_GPIO_PIN_13
#define LCD_RW_PORT GPIOC
#define LCD_RW_PIN LL_GPIO_PIN_14
#define LCD_E_PORT GPIOC
#define LCD_E_PIN LL_GPIO_PIN_15
#define LCD_RESET_PORT GPIOB
#define LCD_RESET_PIN LL_GPIO_PIN_0

static void LCD_send(bool RS, bool RW, uint8_t data) {
	digitalWrite(LCD_DI, RS);
  digitalWrite(LCD_RW, RW);
  digitalWrite(PA0, data & 0b00000001);
  digitalWrite(PA1, data & 0b00000010);
  digitalWrite(PA2, data & 0b00000100);
  digitalWrite(PA3, data & 0b00001000);
  digitalWrite(PA4, data & 0b00010000);
  digitalWrite(PA5, data & 0b00100000);
  digitalWrite(PA6, data & 0b01000000);
  digitalWrite(PA7, data & 0b10000000);
  delayMicroseconds(1);
  digitalWrite(LCD_E, HIGH);
  delayMicroseconds(1); // 140ns
  digitalWrite(LCD_E, LOW);
  delayMicroseconds(1); // total E cycle is 1.2us
}

static void LCD_command(uint8_t command) {
  LCD_send(false, false, command);
}

static void LCD_data(uint8_t data) {
  LCD_send(true, false, data);
}

void LCD_init(void) {
	pinMode(LCD_RESET, OUTPUT);
  digitalWrite(LCD_RESET, false);
  pinMode(LCD_DI, OUTPUT);
  pinMode(LCD_E, OUTPUT);
  digitalWrite(LCD_E, false);
  pinMode(LCD_RW, OUTPUT);
  pinMode(PA0, OUTPUT);
  pinMode(PA1, OUTPUT);
  pinMode(PA2, OUTPUT);
  pinMode(PA3, OUTPUT);
  pinMode(PA4, OUTPUT);
  pinMode(PA5, OUTPUT);
  pinMode(PA6, OUTPUT);
  pinMode(PA7, OUTPUT);
  // magic lcd init sequence
  delayMicroseconds(40000);
  digitalWrite(LCD_RESET, true);
  delayMicroseconds(40000);
  LCD_command(0x30);
  delayMicroseconds(100); // > 100us
  LCD_command(0x30);
  delayMicroseconds(37); // delay > 37us
  // display off
  LCD_command(0x0C); // can write DCB properly
  delayMicroseconds(100); // delay >100us
  // clear
  LCD_command(0x01);
  delayMicroseconds(10000);
  // mode set
  LCD_command(0x06);
  //delay(1); 
  // 8-bit, turn RE on
  LCD_send(false, false, 0x32);
  delayMicroseconds(72);
  // 8bit, RE on, graphic display on
  LCD_send(false, false, 0x36);
  delayMicroseconds(72);
}

void LCD_Clear(void) {
	LCD_command(0x01);
	delayMicroseconds(1600);
}

void LCD_Home(void) {
	LCD_command(0x02);
	delayMicroseconds(72);
}


void LCD_EntryModeSet(bool directionForward, bool shift) {
	LCD_command(0x04 + 2*(directionForward) + 1*shift);
	delayMicroseconds(72);
}

void LCD_Control(bool display, bool cursor, bool cursorBlink) {
	LCD_command(0x8 + 4*display + 2*cursor + 1*cursorBlink);
	delayMicroseconds(72);
}

void LCD_Shift(bool shiftDisplay, bool right) {
	LCD_command(0x10 + 8*shiftDisplay + 4*right);
	delayMicroseconds(72);
}

void LCD_FunctionSet(bool fullByteData, bool extendedInstruction) {
	LCD_command(0x20 + 0x10*fullByteData + 4*extendedInstruction);
	delayMicroseconds(72);
}

void LCD_SetCGRAM_Address(uint8_t address) {
	LCD_command(0x40 + (address & 0x3F));
	delayMicroseconds(72);
}

void LCD_SetDDRAM_Address(uint8_t address) {
	LCD_command(0x80 + (address & 0x7F));
	delayMicroseconds(72);
}

void LCD_Write(uint8_t data) {
	LCD_data(data);
	delayMicroseconds(72);
}
/////// extended instruction
///@brief Enter standby mode, any other instruction can terminate. COM1…32 are halted.
void LCD_Standby(void) {
	LCD_command(0x01);
	delayMicroseconds(72);
}

void LCD_Scroll(bool scrollPosition) {
	LCD_command(0x02 + 1*scrollPosition);
	delayMicroseconds(72);
}

void LCD_Reverse(bool R1, bool R0) {
	LCD_command(0x04 + 2*R1 + 1*R0);
	delayMicroseconds(72);
}

void LCD_ExtFunctionSet(bool DL, bool RE, bool G) {
	LCD_command(0x20 + 0x10*DL + 0x04*RE + 0x02*G);
	delayMicroseconds(72);
}

void LCD_SetScrollAddress(uint8_t address) {
	LCD_command(0x40 + (address & 0x3F));
	delayMicroseconds(72);
}

void LCD_SetGDRAM_Address(uint8_t y, uint8_t x) {
	LCD_command(0x80 + (y & 0x0F));
	delayMicroseconds(72);
	LCD_command(0x80 + (x & 0x3F));
	delayMicroseconds(72);
}
