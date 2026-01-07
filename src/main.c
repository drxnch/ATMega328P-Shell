#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>

#include <sysTime.h>
#include <UART.h>
#include <ADC.h>
#include <PWM.h>
#include <shell.h>

uint32_t lastBlinkTime = 0;
uint32_t lastReadTime = 0;
uint32_t lastPressTime = 0;
uint32_t lastSensorTime = 0; 
uint16_t sensorVal0 = 0;
uint8_t brightness = 0;

const int BLINK_DURATION = 1000;
bool buttonDown  = false;
bool readMode = false;
bool writeMode = false;
char rx_buffer[100];
uint8_t rx_buffer_counter= 0;

int main(void) {
// Setup Code Here
    //Interrupt Setups
    sei();
    sysTime_init();
    UART_init(9600);
    ADC_init();
    PWM_init();

    //Initial Pin Configurations
    DDRD |= (1 << PD2) | (1 << PD3); // LEDs
    DDRB |= (1 << PB1); // PWM LED
    DDRD &= ~(1 << PD7); // Enables internal pull up for button
    
    PORTD |= (1 << PD7); // Setting as active Low
    PORTC &= ~(1 << PC0); // For ADC

    // Initial Shell Welcome
    printString("Welcome to the Shell for the ATMega328P.\r\n");
    printString("Let's get started!\r\n");

    while (1) {

        sensorVal0 = ADC_read(0);
        
        brightness = sensorVal0 >> 8;   // 65535 -> 255, 0 -> 0

        uint32_t currentTime = sysTime();
        

        // Blink
        if (currentTime - lastBlinkTime >= BLINK_DURATION) {
            PORTD ^= (1 << PD2);
            // printString("Time: "); printInt(currentTime); printString("\r\n");
            lastBlinkTime = currentTime;
        }



        // Button Press
        if ((currentTime- lastPressTime >= 5)) {
            lastPressTime = currentTime;
            if (!(PIND & (1 << PD7))) { //Checking using '!'
                PORTD |= (1 << PD3);
                if (buttonDown == false); //printString("Hello World\r\n");
                buttonDown = true;
            }
            else {
                PORTD &= ~(1 << PD3);
                buttonDown = false;
            }
        }


        // Sensor
        if (currentTime - lastSensorTime >= 100) {
            //printString("Sensor value: "); printInt(sensorVal0);printString("\r\n");
            lastSensorTime = currentTime;
            LED_brightness(brightness);
        }

        // Reading UART input
        if (UCSR0A & (1 << RXC0)) { // This feels like non-blocking code, but not too sure
            unsigned char c = USART_Receive(); // read received byte
            UART_sendbyte(c); // Print character (debugging)

            //Add to buffer
            rx_buffer[rx_buffer_counter] = c;
            rx_buffer_counter++;

            if (UDR0 == 13) {
                rx_buffer[rx_buffer_counter] = '\0';
                // printString("the message being sent is..."); printString(rx_buffer); printString("\r\n");
                // printString("Going to parse the message. Hold on!\r\n");
                parseBuffer(rx_buffer);
                
                // Clear rx_buffer
                for (int j = 0; j < 100; j++) rx_buffer[0] = '\0';
                rx_buffer_counter = 0;
            }
        }
    }
    return 0;
}