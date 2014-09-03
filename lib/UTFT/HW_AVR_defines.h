// lib modifiée - Maël 02/2013 - ONLY FOR SSD1289 (sainsmart, see below) WITH ARDUINO MEGA
//Screen : SainSmart 3.2" TFT LCD Display+Touch Panel+PCB adapter SD Slot for Arduino 2560
//Shield : SainSmart TFT LCD Adjustable Shield for Arduino Mega 2560 R3 1280 A082 Plug

/*Infos pins
number - arduino pin - use
38 PD7  RS 
39 PG2  WR
40 PG1  CS
41 PG0  RST
*/

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
//3 nop sont nécessaires sinon l'écran reste gris
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
