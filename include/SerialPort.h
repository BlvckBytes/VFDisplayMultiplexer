#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>

#ifndef SerialPort_H_
#define SerialPort_H_

#define SerialPort_RX_EN
#define SerialPort_TX_EN

#ifdef SerialPort_TX_EN
ISR(USART_TX_vect);
#endif

#ifdef SerialPort_RX_EN
ISR(USART_RX_vect);
#endif

void uart_init(uint32_t baud, uint8_t high_speed);

#ifdef SerialPort_TX_EN
void uart_transmit_byte(uint8_t byte);
void uart_transmit_bytes(uint8_t* byte, uint16_t len);
void uart_transmit_string(char* string, _Bool terminated);
#endif

#ifdef SerialPort_RX_EN
uint8_t uart_unread_count();
uint8_t uart_read_byte();
void uart_reset_readbuf();
#endif

#endif