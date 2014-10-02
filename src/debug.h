#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINTLN(str)    \
Serial.print(millis());     \
Serial.print(": ");         \
Serial.print(__PRETTY_FUNCTION__); \
Serial.print(' ');          \
Serial.println(str);
#else
#define DEBUG_PRINTLN(str)
#endif
#endif
