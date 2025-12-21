#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void ADC_init(void) {
    // Selects ADC to begin
    ADMUX = (1 << REFS0); // Selects voltage reference to 5V, 
    ADMUX = ~(1 << MUX3) & ~(1 << MUX2) & ~(1 << MUX1) & ~(1 << MUX0);  // Sets to ADC0

    // ADEN = ADC Enable
    // ADPS2, ADPS1, ADPS0 = ADC Prescalar (To system clock)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

uint16_t ADC_read(uint8_t channel) {
    ADMUX = (ADMUX & 0b11110000) | (channel & 0b00001111);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}