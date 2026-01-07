#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*Voltatile - don't optimise the value out - 'tick dependent'*/
/*Static - only this file can change the variable*/
volatile static uint32_t ms_counter = 0;

void sysTime_init(void) {
    // Waveform settings
    /* We want to form 1000ms exact timers*/
    TCCR0A |= (1 << WGM01); /* This sets CTC Mode*/
    // Prescalars
    // 16,000,000 / 64 = 250,000 / 1000ms => 250 ticks/ms
    /* So our prescalar should be set to 64*/
    TCCR0B |= (1 << CS01) | (1 << CS00);
    TCCR0B &= ~(1 << CS02);

    // Calling Interrupts
    OCR0A = 249; /* To do this, we will do count to 249 as top (this compare value will be in OCR0A register)*/
    TIMSK0 |= (1 << OCIE0A); /* This calls the ISR*/

    sei(); // enables global interrupts
}

ISR(TIMER0_COMPA_vect) {
    ms_counter++;
}

uint32_t sysTime(void) {
    uint32_t time;
    cli(); //stops all interrupts
    time = ms_counter;
    sei();
    return time;
}