#include "VFDHandle.h"
#include "VFDMultiplex.h"
#include "SerialPort.h"
#include <inttypes.h>

#ifndef VFDCommands_H_
#define VFDCommands_H_

void handle_uart_commands(VFDHandle *handle);

#endif