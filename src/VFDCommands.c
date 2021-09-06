#include "VFDCommands.h"

/*
  Protocol definition:
  <Start-Char><Command Char><Row Index><COLCOUNT * char or less with \0 at the end as data>

  T - Text characters
  U - Underline bits

  Examples:
  - \2T0This is the content of row 0\0
  - \2T1This is the content of row 1\0
  - \2U00000000000000000000000000000000000000001 // Last char underlined for row 0
  - \2U11\0 // First char underlined for row 1

  Start char is used to "synchronize"
 */

void handle_uart_commands(VFDHandle *handle) {
  volatile static uint8_t col_index = 0, row_index = UINT8_MAX;
  volatile static char action = 0;
  volatile static _Bool command_mode = 0;

  while(uart_unread_count() > 0) {
    char value = (char) uart_read_byte();

    // Char is unused control symbol, filter it out
    if ((value >= 3 && value <= 31) || value == 1 || value == 127)
      continue;

    // Wait for command mode
    // Guard until command mode has been entered by \2
    if (!command_mode) {
      // Enter command mode
      if (value == 2) {
        command_mode = 1;
        action = 0;
      }
      continue;
    }

    // Listen for action to perform
    if (action == 0) {
      // Invalid action provided, quit command mode
      if (value != 'T' && value != 'U') {
        command_mode = 0;
        continue;
      }

      // Change to desired action, enable row listen
      action = value;
      row_index = UINT8_MAX;
      continue;
    }

    // Listen for row
    if (row_index == UINT8_MAX) {
      // Invalid row provided, quit command mode
      if (value < '0' && value > '9') {
        command_mode = 0;
        continue;
      }

      // Change to desired row, constrain upwards
      row_index = value - '0';
      if (row_index >= CHAR_COLS) row_index = 0;
      col_index = 0; // Start out at column 0
      continue;
    }

    // Command has been terminated
    if (value == '\0') {
      command_mode = 0;
      continue;
    }

    switch (action) {

      // Text command
      case 'T':
        // Clear display before setting first char
        if (col_index == 0) vfd_mp_clear_text_row(handle, row_index);

        // Set char of current column to current char
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

    // Command has reached max data length, quit
    if (col_index == CHAR_COLS) {
      command_mode = 0;
      continue;
    }
  }
}