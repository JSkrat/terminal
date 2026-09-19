#include <stdbool.h>
#include <stdint.h>

void LCD_Clear(void);
void LCD_Home(void);
void LCD_EntryModeSet(bool directionForward, bool shift);
void LCD_Control(bool display, bool cursor, bool cursorBlink);
void LCD_Shift(bool shiftDisplay, bool right);
void LCD_FunctionSet(bool fullByteData, bool extendedInstruction);
void LCD_SetCGRAM_Address(uint8_t address);
void LCD_SetDDRAM_Address(uint8_t address);
void LCD_Write(uint8_t data);
void LCD_Standby(void);
void LCD_Scroll(bool scrollPosition);
void LCD_Reverse(bool R1, bool R0);
void LCD_ExtFunctionSet(bool DL, bool RE, bool G);
void LCD_SetScrollAddress(uint8_t address);
void LCD_SetGDRAM_Address(uint8_t y, uint8_t x);
