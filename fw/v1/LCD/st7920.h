#include <stdbool.h>
#include <stdint.h>

extern void LCD_init(void);
extern void LCD_Clear(void);
extern void LCD_Home(void);
extern void LCD_EntryModeSet(bool directionForward, bool shift);
extern void LCD_Control(bool display, bool cursor, bool cursorBlink);
extern void LCD_Shift(bool shiftDisplay, bool right);
extern void LCD_FunctionSet(bool fullByteData, bool extendedInstruction);
extern void LCD_SetCGRAM_Address(uint8_t address);
extern void LCD_SetDDRAM_Address(uint8_t address);
extern void LCD_Write(uint8_t data);
extern void LCD_Standby(void);
extern void LCD_Scroll(bool scrollPosition);
extern void LCD_Reverse(bool R1, bool R0);
extern void LCD_ExtFunctionSet(bool DL, bool RE, bool G);
extern void LCD_SetScrollAddress(uint8_t address);
extern void LCD_SetGDRAM_Address(uint8_t y, uint8_t x);
