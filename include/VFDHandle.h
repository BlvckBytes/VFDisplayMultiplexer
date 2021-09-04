#include "VFDConfig.h"
#include <inttypes.h>

#ifndef VFDHandle_H_
#define VFDHandle_H_

typedef struct
{
  char row_contents[CHAR_ROWS][CHAR_COLS]; // Contents of individual rows
  
  // Bitmask state of underlines, using as many uchars as needed, based on column width
  uint8_t active_underlines[CHAR_ROWS][INT_CEIL(CHAR_COLS, UCHAR_BITLEN)];

  _Bool shutdown_state; // State of operation
} VFDHandle;

#endif