// Enable or disable the use of a display shield designed for use on 
// an Arduino Uno (or compatible) on an Arduino Mega
//
// ** Currently only available for 8bit display shields **
//
// Uncomment the following line to enable this feature
//#define USE_UNO_SHIELD_ON_MEGA 1
//********************************************************************

// *** Hardwarespecific defines ***
#define cbi(reg, bitmask) *reg &= ~bitmask
#define sbi(reg, bitmask) *reg |= bitmask

#define cbiCS	PORTG &= ~(1<<1)	
#define sbiCS	PORTG |= (1<<1)

#define cbiRS	PORTD &= ~(1<<7)	
#define sbiRS	PORTD |= (1<<7)

#define cbiWR	PORTG &= ~(1<<2)	
#define sbiWR	PORTG |= (1<<2)

//TODO check that !
//#define pulse_high_WR PORTG |= (1<<2); delayMicroseconds(1); PORTG &= ~(1<<2);
#define pulse_high_WR PORTG |= (1<<2); __asm__ volatile ("nop"); __asm__ volatile ("nop"); __asm__ volatile ("nop"); PORTG &= ~(1<<2);
//3 nop sont n�cessaires sinon l'�cran reste gris
#define pulse_low_WR PORTG &= ~(1<<2); __asm__ volatile ("nop"); __asm__ volatile ("nop"); __asm__ volatile ("nop"); __asm__ volatile ("nop"); PORTG |= (1<<2)

#define pulse_high(reg, bitmask) sbi(reg, bitmask); cbi(reg, bitmask);
#define pulse_low(reg, bitmask) cbi(reg, bitmask); sbi(reg, bitmask);

#define cport(port, data) port &= data
#define sport(port, data) port |= data

#define swap(type, i, j) {type t = i; i = j; j = t;}

#define fontbyte(x) pgm_read_byte(&cfont.font[x])  

#define regtype volatile uint8_t
#define regsize uint8_t
#define bitmapdatatype unsigned int*
