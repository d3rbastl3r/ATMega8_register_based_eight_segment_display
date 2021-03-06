/**
 * A minimal example how to display hexadecimal values on the eight segment display trough the 8Bit shift register
 * The example code in the main routine shows the values 0 to 9 and A to F on the display.
 *
 * Port assignments for the Shift-Register
 * `-> SER:   PD5 // Port to transmit data
 * `-> SRCLK: PD6 // Clock port for data transmission
 * `-> RCLK:  PD7 // Latch port. Show data.
 *
 * @author Igor Martens
 * @since 06.10.2018
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

// value decoder for the eight segment display
uint8_t eightSegmentDecoder[17] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01100111, // 9
    0b01110111, // A
    0b01111100, // b
    0b00111001, // C
    0b01011110, // d
    0b01111001, // E
    0b01110001, // F
    0b10000000  // .
};

void setup() {
    // Setup output ports
    DDRD |= (1<<DDD5);  // SER Port
    DDRD |= (1<<DDD6);  // SRCLK Port
    DDRD |= (1<<DDD7);  // RCLK Port
}

/**
 * Push the given byte to the register and finally execute latch.
 * The left bit will be pushed first.
 */
void pushByteAndLatch(uint8_t byte) {
    for (uint8_t i=0; i<8; ++i) {
        (byte & 128) ? PORTD |= (1 << PD5) : PORTD &= ~(1 << PD5);
        PORTD |= (1 << PD6);
        PORTD &= ~(1 << PD6);
        byte = byte << 1;
    }

    PORTD |= (1 << PD7);
    PORTD &= ~(1 << PD7);
}

/**
 * Set the data inside register to zero and latch
 * This we only need to save one port on the MCU.
 * If the MCU have enough ports SRCLR on the shift register can be used to clear data instead of this method.
 */
void clearRegister() {
    pushByteAndLatch(0b00000000);
}

int main(void) {
    setup();
    clearRegister();

    while(1) {
        for (uint8_t i=0; i<17; ++i) {
            // Push the data to show 0 to 9 and A to F on the eight segment display
            pushByteAndLatch(eightSegmentDecoder[i]);
            _delay_ms(1000);
        }
    }

    return 0;
}
