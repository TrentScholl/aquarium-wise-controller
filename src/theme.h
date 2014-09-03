#ifndef THEME_H
#define THEME_H

struct RGB {
  byte r;
  byte g;
  byte b;
};
typedef struct RGB Color;

const Color THEME_BACK = {240, 243, 244};

const Color THEME_HEAD_BACK = {17, 153, 196};
const Color THEME_HEAD_FORE = {255, 255, 255};

const Color THEME_SUBHEAD_BACK = {246, 248, 248};
const Color THEME_SUBHEAD_FORE = {88, 102, 100};

const Color THEME_PRIMARY_BACK = {255, 255, 255};
const Color THEME_PRIMARY_FORE = {88, 102, 100};

const Color THEME_ON = {35, 175, 68};
const Color THEME_OFF = {255, 255, 255};

const Color THEME_BTN_LIGHT = {255, 255, 255};
const Color THEME_BTN_DARK = {39, 194, 76};
const Color THEME_BTN_DARK_A = {35, 175, 68};

const Color THEME_TAB_LIGHT_FORE = {88, 102, 100};
const Color THEME_TAB_LIGHT_BACK = {255, 255, 255};
const Color THEME_TAB_DARK_FORE = {88, 102, 100};
const Color THEME_TAB_DARK_BACK = {237, 241, 242};

const Color THEME_MACRO = {250, 215, 51};
const Color THEME_MICRO = {111, 98, 184};
const Color THEME_GLUT = {35, 183, 229};

#endif
