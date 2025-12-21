#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART_init(uint32_t baud);
void UART_sendbyte(uint8_t data );
void printInt(uint32_t value);
void printString(const char* str);
unsigned char USART_Receive(void);

#endif