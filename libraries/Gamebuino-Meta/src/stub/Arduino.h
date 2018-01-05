#ifndef ARDUINO_H
#define ARDUINO_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SERCOM_SPI_MODE_0 0
#define SERCOM_SPI_MODE_1 0
#define SERCOM_SPI_MODE_2 0
#define SERCOM_SPI_MODE_3 0
#define F_CPU 48000000

typedef bool boolean;
typedef uint8_t byte;
typedef uint16_t word;

#define SS 0
#define HIGH 1
#define OUTPUT 1
#define digitalWrite(x,y)
#define pinMode(x,y)

void yield( void ) ;

/* system functions */
int main( void );
void init( void );

/* sketch */
void setup( void ) ;
void loop( void ) ;

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif // abs

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() __enable_irq()
#define noInterrupts() __disable_irq()

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define bit(b) (1UL << (b))

#if (ARDUINO_SAMD_VARIANT_COMPLIANCE >= 10606)
// Interrupts
#define digitalPinToInterrupt(P)   ( P )
#endif

#endif
