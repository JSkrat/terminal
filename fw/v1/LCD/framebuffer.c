#include <stdbool.h>
#include <stdint.h>
#include "framebuffer.h"
#include "st7920.h"

#define LCD_WIDTH 128
// condensed size. Highest bit in each byte is left most pixel
#define LCD_BUFFER_WIDTH (LCD_WIDTH/8)
#define LCD_HEIGHT 64
#define max(a,b) \
 ({ __typeof__ (a) _a = (a); \
		 __typeof__ (b) _b = (b); \
	 _a > _b ? _a : _b; })
#define min(a,b) \
 ({ __typeof__ (a) _a = (a); \
		 __typeof__ (b) _b = (b); \
	 _a < _b ? _a : _b; })

// mirror represents what is currently on the screen
// buffer is working array
// when syncing we compare and push the difference to the screen
static uint8_t mirror[LCD_HEIGHT][LCD_BUFFER_WIDTH];
static uint8_t buffer[LCD_HEIGHT][LCD_BUFFER_WIDTH];
static bool bufferLock = false;

static void fbSyncInternal(bool optimization);

void fbInit(void) {
	LCD_init();
	// clear screen and mirror buffer here
	fbSyncInternal(false);
	bufferLock = false;
}

static int coordConvY(const int y, const int x) {
	// LCD controller autoadvances cursor after the end of the line to the line half screen lower (and it is looped around)
	if (LCD_BUFFER_WIDTH <= x) {
		return y + LCD_HEIGHT/2;
	} else {
		return y;
	}
}

static int coordConvX(const int y, const int x) {
	// LCD controller autoadvances cursor after the end of the line to the line half screen lower (and it is looped around)
	(void) y;
	if (LCD_BUFFER_WIDTH <= x) {
		return x - LCD_BUFFER_WIDTH;
	} else {
		return x;
	}
}

static void fbSyncInternal(bool optimization) {
	bufferLock = true;
	for (int y_ = 0; y_ < LCD_HEIGHT/2; ++y_) {
		// lcd controller does not autoincrement row coordinate, force skip
		bool skip = true;
		for (int x_ = 0; x_ < LCD_BUFFER_WIDTH*2; ++x_) {
			// x_;y_ is coordinates on LCD, x;y is coordinates in buffer
			int y = coordConvY(y_, x_);
			int x = coordConvX(y_, x_);
			if (skip) {
				if (optimization && (mirror[y][x] == buffer[y][x])) {
					// they are identical, we continue skip
					continue;
				}
				// we set coordinates only in the beginning of each line and after skip of unchanged regions
				LCD_SetGDRAM_Address((uint8_t) y, (uint8_t) x);
				skip = false;
			} else {
				// this branch won't trigger if skip was true but became false
				if (optimization) {
					skip = true;
					// look 3 coordinates ahead. If current and next 2 are identical, we skip.
					// If the gap with identical values is less than 3 items long, it is faster to not skip then.
					for (int d = 0; d < 3; ++d) {
						int y_gap = coordConvY(y_, x_+d);
						int x_gap = coordConvX(y_, x_+d);
						// we reached the end of the line and found no changed data, keep skipping then
						if (LCD_BUFFER_WIDTH <= x_gap) break;
						if (buffer[y_gap][x_gap] != mirror[y_gap][x_gap]) {
							skip = false;
							break;
						}
					}
					if (skip) continue;
				}
			}
			mirror[y][x] = buffer[y][x];
			LCD_Write(mirror[y][x]);
		}
	}
	bufferLock = false;
}

void fbSync(void) {
	fbSyncInternal(true);
}

void fbClear(void) {
	for (int y = 0; y < LCD_HEIGHT; ++y)
		for (int x = 0; x < LCD_BUFFER_WIDTH; ++x)
			buffer[y][x] = 0;
}

void fbSetPixel(const uint8_t x, const uint8_t y, bool value) {
	if (LCD_HEIGHT <= y) return;
	if (LCD_WIDTH <= x) return;
	const int x_buf = x / 8;
	const int x_rem = 8 - (x % 8);
	if (value)
		buffer[y][x_buf] |= 1 << x_rem;
	else
		buffer[y][x_buf] &= ~(1 << x_rem);
}

///@brief copy given condensed bitmap at given top-left coordinates. pic should have dimensions (size_x/8)*size_y;
///       if x is not factor of 8, it should be (size_x/8+1)*size_y
void fbSetBitmap(const uint8_t x, const uint8_t y, const uint8_t *pic, const uint8_t size_x, const uint8_t size_y) {
	if (LCD_HEIGHT <= y) return;
	if (LCD_WIDTH <= x) return;
	int size_x_b = x / 8;
	const int offset = x % 8;
	if (0 != offset) size_x_b += 1;
	for (int y_ = 0; y_ < min(size_y, LCD_HEIGHT - y); ++y_) {
        for (int x_ = 0; x_ < min(size_x_b, LCD_BUFFER_WIDTH - x/8); ++x_) {
            const int nextByte = *(pic + x_ + y_*size_x_b);
            const int leftPart = nextByte << offset;
            const int leftMask = ~(0xFF << offset);
            const int rightPart = nextByte >> (8-offset);
            const int rightMask = ~(0xFF >> (8-offset));
            #define LEFT_BUF buffer[y + y_][(x/8) + x_]
            #define RIGHT_BUF buffer[y + y_][(x/8) + x_ + 1]
            LEFT_BUF &= leftMask; LEFT_BUF |= leftPart;
            RIGHT_BUF &= rightMask; RIGHT_BUF |= rightPart;
        }
	}
}
