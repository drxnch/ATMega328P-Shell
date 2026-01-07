#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

void PWM_init(void) {
    //Non-inverting Mode
    //WGM : Fast PWM
    //Prescalar - clk/8
    TCCR1A = (1 << COM1A1) | (1 << WGM10);
    TCCR1B = (1 << WGM12) | (1 << CS11);
}

void LED_brightness(uint8_t brightness) {
    OCR1A = brightness;
}