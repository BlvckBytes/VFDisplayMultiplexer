#include <inttypes.h>

#ifndef VFDConfig_H_
#define VFDConfig_H_

//////////////////////////////////////
//      Configurable variables      //
//////////////////////////////////////

#define GRID_WIDTH 5 // Pixel grid width
#define GRID_HEIGHT 7 // Pixel grid height
#define CHAR_COLS 40 // Number of display columns
#define CHAR_ROWS 2 // Number of display rows
#define UNUSED_INTERROWS 4 // How many unused pixel slots there are between 
#define HAS_UNDERSCORE 1 // 1 if has, 0 otherwise

#define PIN_PIXEL_DATA 0 // Pixel registers data line
#define PIN_PIXEL_CLK 1 // Pixel registers clock line
#define PIN_COL_CLK 2 // Column registers clock line
#define PIN_COL_DATA 3 // Column registers data line

#define COMM_BAUD 57600

// As many bytes as there are columns, +2 (1 for action, 1 for row), times rows, times 2 (text and underlines)
// This can be decreased if the baud rate gets decreased, or the F_CPU is high enough
// Otherwise you'll run a risk to loose data and get corrupted messages on fast transitions
#define COMM_RX_BUFFER ((CHAR_COLS + 2) * CHAR_ROWS * 2)

#define DELAY_CYCLES_CLK asm_delay_loop(8) // Clock pulse length, in n * 3 cycles
#define DELAY_CYCLES_DATARDY asm_delay_loop(4) // Stabilize data signals before clock-edge, in n * 3 cycless

//////////////////////////////////////
//         Utility macros           //
//////////////////////////////////////

#define PIN_PIXEL_DATA_MASK (0x1 << PIN_PIXEL_DATA)
#define PIN_PIXEL_CLK_MASK (0x1 << PIN_PIXEL_CLK)
#define PIN_COL_CLK_MASK (0x1 << PIN_COL_CLK)
#define PIN_COL_DATA_MASK (0x1 << PIN_COL_DATA)

#define TOTAL_PIXELS_PER_MATRIX (GRID_WIDTH * GRID_HEIGHT + HAS_UNDERSCORE) // All pixels a matrix contains
#define TOTAL_PIXELS_PER_COL (TOTAL_PIXELS_PER_MATRIX * CHAR_ROWS + (CHAR_ROWS - 1) * UNUSED_INTERROWS) // All pixels a col contains

#define INT_CEIL(a, b) ((a) + (b) - 1) / (b) // Round up integer division
#define ULONG_BITLEN sizeof(uint32_t) * 8 // Bit length of a ULONG
#define UCHAR_BITLEN sizeof(uint8_t) * 8

#endif