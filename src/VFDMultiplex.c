#include "VFDMultiplex.h"

void asm_delay_loop(uint8_t count) {
  __asm__ volatile (
    "1: dec %0" "\n\t"
    "brne 1b"
    : "=r" (count)
    : "0" (count)
  );
}

void vfd_mp_setup_pins() {
  DDRB |= PIN_COL_CLK_MASK | PIN_COL_DATA_MASK | PIN_PIXEL_CLK_MASK | PIN_PIXEL_DATA_MASK;
  PORTB &= ~(PIN_COL_CLK_MASK | PIN_COL_DATA_MASK | PIN_PIXEL_CLK_MASK | PIN_PIXEL_DATA_MASK);
}

static void vfd_mp_pulse_col_clk() {
  PORTB |= PIN_COL_CLK_MASK;
  DELAY_CYCLES_CLK;
  PORTB &= ~PIN_COL_CLK_MASK;
}

static void vfd_mp_pulse_pixel_clk() {
  PORTB |= PIN_PIXEL_CLK_MASK;
  DELAY_CYCLES_CLK;
  PORTB &= ~PIN_PIXEL_CLK_MASK;
}

void vfd_mp_clear_pixel_registers() {
  PORTB &= ~PIN_PIXEL_DATA_MASK;
  DELAY_CYCLES_DATARDY;
  for (int i = TOTAL_PIXELS_PER_COL - 1; i >= 0; i--)
    vfd_mp_pulse_pixel_clk();
}

void vfd_mp_clear_display(VFDHandle* handle) {
  vfd_mp_clear_pixel_registers();
  vfd_mp_clear_underline(handle);
  vfd_mp_clear_text(handle);
  PORTB &= ~PIN_COL_DATA_MASK;
  DELAY_CYCLES_DATARDY;
  for (int i = 0; i < CHAR_COLS; i++)
    vfd_mp_pulse_col_clk();
}

void vfd_mp_clear_underline(VFDHandle* handle) {
  // Set every array entry to zeros
  for (int i = 0; i < CHAR_ROWS; i++)
    for (int j = 0; j < INT_CEIL(CHAR_COLS, UCHAR_BITLEN); j++)
      handle->active_underlines[i][j] = 0U;
}

void vfd_mp_set_underline(VFDHandle* handle, uint8_t row, uint8_t index, _Bool state) {
  // Protect from out of range memory corruption
  if (row >= CHAR_ROWS) return;
  if (index >= CHAR_COLS) return;

  // Calculate index of target uint, create mask relative to this uchar
  uint8_t part_index = INT_CEIL(index + 1, UCHAR_BITLEN) - 1;
  uint8_t part_bitmask = 0x1 << (index - part_index * UCHAR_BITLEN);

  // Apply ON or OFF bit-manipulation
  unsigned char* value = &(handle->active_underlines[row][part_index]);
  *value = state ? (*value | part_bitmask) : (*value & ~part_bitmask);
}

void vfd_mp_set_underline_row(VFDHandle* handle, uint8_t row, _Bool state) {
  // Protect from out of range memory corruption
  if (row >= CHAR_ROWS) return;

  for (int i = 0; i < INT_CEIL(CHAR_COLS, UCHAR_BITLEN); i++)
    handle->active_underlines[row][i] = state ? UCHAR_MAX : 0x0;
}

void vfd_mp_put_row_chars(VFDHandle* handle, uint8_t col) {
  // Protect from out of range memory corruption
  // Catch last row double-draw
  if (col == CHAR_COLS) col = CHAR_COLS - 1;
  if (col > CHAR_COLS) return;

  // Calculate index of target uchar from active_underlines
  // create mask relative to this uchar to get specific target bit from it
  uint8_t uscore_mask_part = INT_CEIL(col + 1, UCHAR_BITLEN) - 1;
  uint8_t uscore_part_shift = (col - uscore_mask_part * UCHAR_BITLEN);

  for (int i = 0; i < CHAR_ROWS; i++) {
    CharBits bits = get_char_bits(handle->row_contents[i][col]);

    // Underscore is LSb on displays with dedicated segment, otherwise last row (better than nothing)
    uint8_t uscore_mask = HAS_UNDERSCORE ? (
      // Mask current underline position
      (handle->active_underlines[i][uscore_mask_part] & (0x1 << uscore_part_shift))
      >> uscore_part_shift // Shift away trailing zeros
    ) : GRID_WIDTH;

    // Current mask for the following loop, starting out at MSb of upper part of bits
    uint32_t curr_bit_mask = 1UL << (TOTAL_PIXELS_PER_MATRIX - 1 - ULONG_BITLEN);

    // Shifting into register needs to be reversed (first is last)
    for (int j = TOTAL_PIXELS_PER_MATRIX - 1; j >= 0; j--) {
      // Decide to either use lower or upper bits
      // Also apply underscore masking at this point, then mask current bit
      // > 0 will result in true by default, no other shifts required
      _Bool value = (j < ULONG_BITLEN ? (bits.lower | uscore_mask) : bits.upper) & curr_bit_mask;

      // Shift in current value bit
      PORTB = value ? (PORTB | PIN_PIXEL_DATA_MASK) : (PORTB & (~PIN_PIXEL_DATA_MASK));
      DELAY_CYCLES_DATARDY;
      vfd_mp_pulse_pixel_clk();

      // Next bit (top down)
      curr_bit_mask = curr_bit_mask >> 1;

      // MSb's done, now go to LSBs
      if (curr_bit_mask == 0) curr_bit_mask = 1UL << (ULONG_BITLEN - 1);
    }

    // If not at last row, clock in zeros for unused interrow-pixels
    if (i != CHAR_ROWS - 1) {
      PORTB &= ~PIN_PIXEL_DATA_MASK;
      DELAY_CYCLES_DATARDY;
      for (int j = 0; j < UNUSED_INTERROWS; j++)
      vfd_mp_pulse_pixel_clk();
    }
  }
}

void vfd_mp_handle(VFDHandle* handle) {
  // No operations during shutdown
  if (handle->shutdown_state) return;

  // One more column that there is, so last column has constant brightness too
  for (int i = 0; i <= CHAR_COLS; i++) {

    // Update display contents for current col
    vfd_mp_put_row_chars(handle, i);

    // First iteration - shift in an active column
    // This active column will be moved from left to right while multiplexing
    if (i == 0) {
      PORTB |= PIN_COL_DATA_MASK;
      DELAY_CYCLES_DATARDY;
    }

    // Clock in col bit
    vfd_mp_pulse_col_clk();

    // Turn off again, then leave off for all other iterations
    if (i == 0)
      PORTB &= ~PIN_COL_DATA_MASK;
  }
}

void vfd_mp_set_text(VFDHandle* handle, uint8_t row, const char* text) {
  // Protect from out of range memory corruption
  if (row >= CHAR_ROWS) return;

  // Copy text, with length constrain of cols
  char* target_row = handle->row_contents[row];
  _Bool str_end = 0;
  for (int i = 0; i < CHAR_COLS; i++) {
    if (text[i] == '\0') str_end = 1;
    target_row[i] = str_end ? ' ' : text[i];
  }
}

void vfd_mp_clear_text(VFDHandle* handle) {
  for (int i = 0; i < CHAR_ROWS; i++)
    vfd_mp_clear_text_row(handle, i);
}

void vfd_mp_clear_text_row(VFDHandle* handle, uint8_t row) {
  // Protect from out of range memory corruption
  if (row >= CHAR_ROWS) return;

  for (int i = 0; i < CHAR_COLS; i++)
      handle->row_contents[row][i] = ' ';
}

void vfd_mp_set_char(VFDHandle* handle, uint8_t row, uint8_t col, char value) {
  // Protect from out of range memory corruption
  if (row >= CHAR_ROWS) return;
  if (col >= CHAR_COLS) return;

  handle->row_contents[row][col] = value;
}

void vfd_mp_shutdown(VFDHandle* handle) {
  handle->shutdown_state = 1;
  vfd_mp_setup_pins(); // Turn off pins for power saving
}

void vfd_mp_resume(VFDHandle* handle) {
  handle->shutdown_state = 0;
}

VFDHandle vfd_init_handle() {
  VFDHandle handle;

  vfd_mp_resume(&handle);
  vfd_mp_setup_pins(&handle);
  vfd_mp_clear_display(&handle);
  
  return handle;
}