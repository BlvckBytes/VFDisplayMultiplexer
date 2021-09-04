#include "VFDHandle.h"
#include "VFDMultiplex.h"
#include "SerialPort.h"
#include <inttypes.h>

#ifndef VFDCommands_H_
#define VFDCommands_H_

/*
  Protocol definition:
  <Command Char><Row Index><COLCOUNT * char or less with \0 at the end as data>

  Examples:
  - T0This is the content of row 0\0
  - T1This is the content of row 1\0
  - U00000000000000000000000000000000000000001 // Last char underlined for row 0
  - U11\0 // First char underlined for row 1
 */
void handle_uart_commands(VFDHandle *handle);

#endif