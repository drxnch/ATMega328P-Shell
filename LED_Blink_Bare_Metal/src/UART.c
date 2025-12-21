#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>


void UART_init(uint32_t baud) {
    uint16_t ubrr_value = (F_CPU/(16UL * baud))-1; // From formula
    UBRR0H = (uint8_t)(ubrr_value>>8);
    UBRR0L = (uint8_t)(ubrr_value);
    UCSR0B = (1 << TXEN0) | (1 << RXEN0); // TX Example
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Allows 8 data bits

}

void UART_sendbyte(uint8_t data ){
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
    }

void printString(const char* str) { //printString("Hello World")
    while (*str) {
        UART_sendbyte(*str++);
    }
}

void printInt(uint32_t value) {
    char buffer[12];
    ultoa(value, buffer, 10);\
    printString(buffer);
}

unsigned char USART_Receive(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}