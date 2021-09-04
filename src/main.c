#include "VFDMultiplex.h"
#include "VFDCommands.h"
#include "SerialPort.h"
#include "VFDConfig.h"

int main(void) {
  // Initialize display
  VFDHandle handle = vfd_init_handle();
  
  // Initialize serial communications
  uart_init(COMM_BAUD, 0x0);

  while (1) {
    // Multiplex display
    vfd_mp_handle(&handle);

    // Read and act on serial commands
    handle_uart_commands(&handle);
  }
}