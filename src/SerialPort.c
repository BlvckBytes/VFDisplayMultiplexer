#include "SerialPort.h"

volatile static uint8_t uart_tx_busy = 0;
volatile static uint16_t uart_rx_unread = 0;
volatile static uint16_t uart_rx_writepos = 0;
volatile static uint8_t uart_rx_buffer[RX_BUFFER_SIZE] = {0x0};

#ifdef SerialPort_RX_EN
ISR(USART_RX_vect) {
  // Write I/O buffer into rx buffer at next write position
  uart_rx_buffer[uart_rx_writepos++] = UDR0;

  // Make write position wrap around the buffer
  if (uart_rx_writepos >= RX_BUFFER_SIZE) uart_rx_writepos = 0;

  // New unread byte
  uart_rx_unread++;
}
#endif

// Done with transmitting
#ifdef SerialPort_TX_EN
ISR(USART_TX_vect) { uart_tx_busy = 0; }
#endif

void uart_init(uint32_t baud, uint8_t high_speed) {
  // Enable high speed flag
  if (high_speed)
    UCSR0A |= 0x1 << U2X0;

  // Scale baud rate according to speed mode and cpu frequency
  baud = F_CPU / ((UCSR0A & (0x1 << U2X0)) ? 8 : 16 * baud) - 1;

  // Set 4 MSb's to H register, and 8 LSB's to L register
  UBRR0H = (baud & 0x0F00) >> 0x8;
  UBRR0L = (baud & 0x00FF); 

  // Enable RX, TX and both interrupts
  #ifdef SerialPort_TX_EN
  UCSR0B |= (0x1 << TXEN0) | (0x1 << TXCIE0);
  #endif

  #ifdef SerialPort_RX_EN
  UCSR0B |= (0x1 << RXEN0) | (0x1 << RXCIE0);
  #endif

  // Enable global interrupts
  sei();
}

#ifdef SerialPort_TX_EN
void uart_transmit_byte(uint8_t byte) {
  while (uart_tx_busy); // Wait if transmitter is already busy
  uart_tx_busy = 1;
  UDR0 = byte; // Set value to data register
}

void uart_transmit_bytes(uint8_t* byte, uint16_t len) {
  for (int i = 0; i < len; i++) uart_transmit_byte(byte[i]);
}

void uart_transmit_string(char* string) {
  // Send string char by char
  uint16_t index = 0;
  while (string[index] != '\0')
    uart_transmit_byte(string[index++]);

  // Send termination char
  uart_transmit_byte('\0');
}
#endif

#ifdef SerialPort_RX_EN
uint8_t uart_unread_count() {
  return uart_rx_unread;
}

uint8_t uart_read_byte() {
  static uint16_t uart_rx_readpos = 0;

  // Read data at current position, decrement unread number
  uint8_t data = uart_rx_buffer[uart_rx_readpos++];
  uart_rx_unread--;

  // Make read position wrap around the buffer
  if (uart_rx_readpos >= RX_BUFFER_SIZE)
    uart_rx_readpos = 0;

  return data;
}
#endif