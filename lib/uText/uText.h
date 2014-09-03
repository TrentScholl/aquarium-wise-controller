#ifndef UTEXT_H
#define UTEXT_H

#if defined(__AVR__)
    #include <Arduino.h>
#elif defined(__SAM3X8E__)
    #include <Arduino.h>
    #define PROGMEM
    #define prog_uchar const unsigned char
    #define pgm_read_byte(x)        (*((char *)x))
    #define pgm_read_word(x)        ( ((*((unsigned char *)x + 1)) << 8) + (*((unsigned char *)x)))
    #define pgm_read_byte_near(x)   (*((char *)x))
    #define pgm_read_byte_far(x)    (*((char *)x))
//  #define pgm_read_word(x)        (*((short *)(x & 0xfffffffe)))
//  #define pgm_read_word_near(x)   (*((short *)(x & 0xfffffffe))
//  #define pgm_read_word_far(x)    (*((short *)(x & 0xfffffffe)))
    #define pgm_read_word_near(x)   ( ((*((unsigned char *)x + 1)) << 8) + (*((unsigned char *)x)))
    #define pgm_read_word_far(x)    ( ((*((unsigned char *)x + 1)) << 8) + (*((unsigned char *)x))))
    #define PSTR(x)  x
#else
    #define PROGMEM
    #define prog_uchar byte
#endif

#include <UTFT.h>

#define BITMASK_FONT 1
#define ANTIALIASED_FONT 2
#define HEADER_LENGTH 5

class uText
{
private:
        /* device layer */
        UTFT *utft;
        /* device */
        uint16_t deviceWidth;
        uint16_t deviceHeight;
        /* currently selected font */
        prog_uchar* currentFont;
        /* character color */
        uint8_t cr;
        uint8_t cg;
        uint8_t cb;
        /* matte color */
        uint8_t mr;
        uint8_t mg;
        uint8_t mb;

        void printString(int16_t xx, int16_t yy, String text, int clean, int8_t kerning[] = NULL);

public:
        uText();
        uText(UTFT* utft, uint16_t width, uint16_t height);
        int setFont(prog_uchar font[]);
        void setBackground(uint8_t r, uint8_t g, uint8_t b);
        void setForeground(uint8_t r, uint8_t g, uint8_t b);
        void print(int16_t xx, int16_t yy, String text, int8_t kerning[] = NULL);
        void clean(int16_t xx, int16_t yy, String text, int8_t kerning[] = NULL);
        int16_t getLineHeight();
        int16_t getBaseline();
        int16_t getTextWidth(String text, int8_t kerning[] = NULL);
};

#endif
