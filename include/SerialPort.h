#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>

#ifndef SerialPort_H_
#define SerialPort_H_

#define RX_BUFFER_SIZE 64

ISR(USART_TX_vect);
ISR(USART_RX_vect);

void uart_init(uint32_t baud, uint8_t high_speed);
void uart_transmit_byte(uint8_t byte);
void uart_transmit_bytes(uint8_t* byte, uint16_t len);
void uart_transmit_string(char* string);
uint8_t uart_unread_count();
uint8_t uart_read_byte();

#endif