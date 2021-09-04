#include <inttypes.h>

#ifndef CharBits_H_
#define CharBits_H_

/* This struct acts as a custom data structure to keep all matrix bits

 * Let's say width is 5 and height is 7: 5 * 7 = 35 + 1 (underscore) = 36.
 * An unsigned long is 32, so 4 remaining, unsigned char is 8 (more than enough)
 * If there are bigger pixel-matrices on your display, just change the char
 * to something bigger, since you need to re-do the char_table anyways
 */
typedef struct
{
  uint8_t upper; // MSbs of pixel-matrix
  uint32_t lower; // LSbs of pixel-matrix
} CharBits;

extern CharBits char_table[95];
CharBits get_char_bits(char c);

#endif