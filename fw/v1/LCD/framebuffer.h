#include <stdint.h>
#include <stdbool.h>

typedef enum {
    cNone,
    cSet,
    cReset,
} eColor;

extern void fbInit(void);
extern void fbSync(void);
extern void fbClear(void);
extern void fbSetPixel(const uint8_t x, const uint8_t y, bool value);
extern void fbSetBitmap(const uint8_t x, const uint8_t y, const uint8_t *pic, const uint8_t size_x, const uint8_t size_y);
extern void fbRect(const uint8_t x, const uint8_t y, const uint8_t size_x, const uint8_t size_y, const eColor fill);
extern void fbBorder(const uint8_t x, const uint8_t y, const uint8_t size_x, const uint8_t size_y, const eColor border, const uint8_t borderSize, const eColor fill);
