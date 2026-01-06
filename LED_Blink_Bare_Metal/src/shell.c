#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>

#include <UART.h>

typedef struct {
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    volatile uint8_t *pin;
    uint8_t bit;
    int8_t adc;   // -1 if not ADC capable
} gpio_pin_t;

const gpio_pin_t gpio_map[] = {
    // D0 – D7  (PORTD)
    { &DDRD, &PORTD, &PIND, 0, -1 }, // D0 //  PD0 // 0
    { &DDRD, &PORTD, &PIND, 1, -1 }, // D1 //  PD1 // 1
    { &DDRD, &PORTD, &PIND, 2, -1 }, // D2 //  PD2 // 2
    { &DDRD, &PORTD, &PIND, 3, -1 }, // D3 //  PD3 // 3
    { &DDRD, &PORTD, &PIND, 4, -1 }, // D4 //  PD4 // 4
    { &DDRD, &PORTD, &PIND, 5, -1 }, // D5 //  PD5 // 5
    { &DDRD, &PORTD, &PIND, 6, -1 }, // D6 //  PD6 // 6
    { &DDRD, &PORTD, &PIND, 7, -1 }, // D7 //  PD7 // 7

    // D8 – D13 (PORTB)
    { &DDRB, &PORTB, &PINB, 0, -1 }, // D8 //  PB0 // 8
    { &DDRB, &PORTB, &PINB, 1, -1 }, // D9 //  PB1 // 9
    { &DDRB, &PORTB, &PINB, 2, -1 }, // D10 // PB2 // 10
    { &DDRB, &PORTB, &PINB, 3, -1 }, // D11 // PB3 // 11
    { &DDRB, &PORTB, &PINB, 4, -1 }, // D12 // PB4 // 12
    { &DDRB, &PORTB, &PINB, 5, -1 }, // D13 // PB5 // 13

    { &DDRC, &PORTC, &PINC, 0, -1 }, // D14 // PC0 // 14
    { &DDRC, &PORTC, &PINC, 1, -1 }, // D15 // PC1 // 15
    { &DDRC, &PORTC, &PINC, 2, -1 }, // D16 // PC2 // 16
    { &DDRC, &PORTC, &PINC, 3, -1 }, // D17 // PC3 // 17
    { &DDRC, &PORTC, &PINC, 4, -1 }, // D18 // PC4 // 18
    { &DDRC, &PORTC, &PINC, 5, -1 }, // D19 // PC5 // 19
};

void readValue(char *arg_1, uint8_t *arg_2) {
    // if (arg_1[0] == 'b') {
    //     printString("We have infiltrated base B!\r\n");
    //     printString("The location is Port "); UART_sendbyte(location[0]); printString(", and position: "); UART_sendbyte(location[1]); printString("\r\n");
    //     if (DDRB & (1 << location[1])) {printInt(1); printString("\r\n");}
    //     else {printInt(0); printString("\r\n");}
    // }
    // if (location[0] == 'c') {
    //     printString("We have infiltrated base C!\r\n");
    //     printString("The location is Port "); UART_sendbyte(location[0]); printString(", and position: "); UART_sendbyte(location[1]); printString("\r\n");
    //     if (DDRC & (1 << location[1])) {printInt(1); printString("\r\n");}
    //     else {printInt(0); printString("\r\n");}
    // }
    // if (location[0] == 'd') {
    //     printString("We have infiltrated base D!\r\n");
    //     printString("The location is Port "); UART_sendbyte(location[0]); printString(", and position: "); UART_sendbyte(location[1]); printString("\r\n");
    //     if (DDRD & (1 << location[1])) {printInt(1); printString("\r\n");}
    //     else {printInt(0); printString("\r\n");}
    // }

    uint8_t pin_arg = arg_2;
    gpio_pin_t pin = gpio_map[pin_arg];
    
    // Read the pin
    uint8_t pin_value = (*(pin.pin) & (1 << pin.bit)) ;
    printInt(pin_value);
}

void setPin(char *arg_1, uint8_t *arg_2) {

}

void parseBuffer(char *line) {
    printString("We are in the parser. Imma chop this line down!\r\n");
    char *action = strtok(line, " ");
    char *object = strtok(NULL, " ");
    char *arg_1 = strtok(NULL, " ");
    uint8_t *arg_2 = strtok(NULL, " ");    

    printString(action); printString(object); printString(arg_1);printString(arg_2);printString("\r\n");
    
    if (!action || !object || !arg_1) {
        printString("I don't have all commands bro.\r\n"); 
        return;
    }
    if (strcmp(action, "read") == 0) {
      if (strcmp(object, "pin") == 0) {
        printString("Reading Pin\r\n");
        readValue(arg_1, arg_2);
      }  
    }
        if (strcmp(action, "write") == 0) {
      if (strcmp(object, "pin") == 0) {
        printString("Writing Pin\r\n");
        setPin(arg_1,arg_2);
      }  
    }
}