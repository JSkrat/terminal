#include <stdint.h>
#include <stdbool.h>

extern void fbInit(void);
extern void fbSync(void);
extern void fbClear(void);
extern void fbSetPixel(const uint8_t x, const uint8_t y, bool value);
extern void fbSetBitmap(const uint8_t x, const uint8_t y, const uint8_t *pic, const uint8_t size_x, const uint8_t size_y);
