#include "VFDCommands.h"

void handle_uart_commands(VFDHandle *handle) {
  static uint8_t col_index = 0, row_index = UINT8_MAX;
  static char action = 0;

  while(uart_unread_count() > 0) {
    char value = (char) uart_read_byte();

    // No active action, set from first byte
    if (action == 0) {
      // Only accept proper commands
      if (value == 'T' || value == 'U')
        action = value;
      continue;
    }

    // No active row, set from second byte
    if (row_index == UINT8_MAX) {
      // Parse row digit if in ASCII range, keep at 0 otherwise as fallback
      if (value >= '0' || value <= '9')
        row_index = value - '0';
      continue;
    }

    // Invalid request, skip all further bytes
    if (row_index >= CHAR_ROWS) continue;

    // Command end has been reached, reset variables
    if (col_index >= CHAR_COLS || value == '\0') {
      col_index = 0;
      row_index = -1;
      action = 0;
      break;
    }

    // If char is unprintable, skip it, without incrementing col_index
    if (value >= 0 && value <= 31) continue;

    switch (action) {

      // Text command
      case 'T':
        // Clear display before setting first char
        if (col_index == 0) vfd_mp_clear_text_row(handle, row_index);
        vfd_mp_set_char(handle, row_index, col_index, value);
        break;

      // Underline command
      case 'U':
        // Clear underlines before setting first bit
        if (col_index == 0) vfd_mp_set_underline_row(handle, row_index, 0);

        // Set underline on current column to value of current byte
        vfd_mp_set_underline(handle, row_index, col_index, value - '0');
        break;
    }

    // Next col
    col_index++;
  }
}