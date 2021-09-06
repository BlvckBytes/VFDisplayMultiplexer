#include "VFDMultiplex.h"
#include "VFDCommands.h"
#include "SerialPort.h"
#include "VFDConfig.h"

// #include <stdio.h>

int main(void) {
  // Initialize display
  VFDHandle handle = vfd_init_handle();
  
  // Initialize serial communications
  uart_init(COMM_BAUD, 0x0);
  vfd_mp_set_text(&handle, 0, "Initialized!");

  // int c = 0;
  // char buf[10];
  while (1) {
    // sprintf(buf, "My counter is: %d, some suffix", c);
    // vfd_mp_set_text(&handle, 0, buf);

    // Multiplex display
    vfd_mp_handle(&handle);

    // Read and act on serial commands
    handle_uart_commands(&handle);

    // c++;
  }
}