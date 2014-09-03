#include "uText.h"

uText::uText() {
}

uText::uText( UTFT* utftDev, uint16_t width, uint16_t height ) {
    utft = utftDev;
    deviceWidth = width;
    deviceHeight = height;
    mr = 0xa0;
    mg = 0xa0;
    mb = 0xa0;
    cr = 0xff;
    cg = 0xff;
    cb = 0xff;
}

void uText::setBackground(uint8_t r, uint8_t g, uint8_t b) {
    mr = r;
    mg = g;
    mb = b;
}

void uText::setForeground(uint8_t r, uint8_t g, uint8_t b) {
    cr = r;
    cg = g;
    cb = b;
}

int uText::setFont(prog_uchar font[]) {
    int p1 = pgm_read_byte_near(font + 0);
    int p2 = pgm_read_byte_near(font + 1);
    if ( p1 != 'Z' || p2 != 'F' ) {
//			Serial.print("Invalid font prefix ");
//			Serial.print( p1 );
//			Serial.print( " " );
//			Serial.println( p2 );
        currentFont = NULL;
        return -1;
    }
    int fontType = pgm_read_byte_near(font + 2);
    if ( fontType != ANTIALIASED_FONT && fontType != BITMASK_FONT ) {
//			Serial.println("Unsupported font type");
        currentFont = NULL;
        return -1;
    }
    currentFont = font;
    return 0;
}

void uText::print(int16_t xx, int16_t yy, String text, int8_t kerning[]) {
	printString(xx, yy, text, 0, kerning);
}

void uText::clean(int16_t xx, int16_t yy, String text, int8_t kerning[]) {
	printString(xx, yy, text, 1, kerning);
}

void uText::printString(int16_t xx, int16_t yy, String text, int clean, int8_t kerning[]) {

    if ( currentFont == NULL ) {
        return;
    }

    int fontType = pgm_read_byte_near(currentFont + 2);
    if ( fontType != ANTIALIASED_FONT && fontType != BITMASK_FONT ) {
        return;
    }

    int kernPtr = 0;
    int kern = -100; // no kerning

    int glyphHeight = pgm_read_byte_near(currentFont + 3);

    int x1 = xx;

    for (int t = 0; t < text.length(); t++) {
        char c = text.charAt(t);

        int width = 0;
        boolean found = false;
        int ptr = HEADER_LENGTH;
        while ( 1 ) {
            char cx = (char)(((int)pgm_read_byte_near(currentFont + ptr + 0) << 8) + pgm_read_byte_near(currentFont + ptr + 1));
            if ( cx == 0 ) {
              break;
            }
            int length = (((int)(pgm_read_byte_near(currentFont + ptr + 2) & 0xff) << 8) + (int)(pgm_read_byte_near(currentFont + ptr + 3) & 0xff));

            if ( cx == c ) {
                if ( length < 8 ) {
//						Serial.print( "Invalid "  );
//						Serial.print( c );
//						Serial.println( " glyph definition. Font corrupted?" );
                    break;
                }
                found = true;

                width = 0xff & pgm_read_byte_near(currentFont + ptr + 4);

                int marginLeft = 0x7f & pgm_read_byte_near(currentFont + ptr + 5);
                int marginTop = 0xff & pgm_read_byte_near(currentFont + ptr + 6);
                int marginRight = 0x7f & pgm_read_byte_near(currentFont + ptr + 7);
                int effWidth = width - marginLeft - marginRight;

                int ctr = 0;

                if ( fontType == ANTIALIASED_FONT ) {

                    boolean vraster = (0x80 & pgm_read_byte_near(currentFont + ptr + 5)) > 0;

                    if ( vraster ) {
                        int marginBottom = marginRight;
                        int effHeight = glyphHeight - marginTop - marginBottom;

                        for ( int i = 0; i < length - 8; i++ ) {
                            int b = 0xff & pgm_read_byte_near(currentFont + ptr + 8 + i);
                            int x = ctr / effHeight;
                            int y = ctr % effHeight;

                            if ( (0xc0 & b) > 0 ) {
                                int len = 0x3f & b;
                                ctr += len;
                                if ( (0x80 & b) > 0 ) {
                                	if ( clean > 0 ) {
                                    	utft->setColor(mr, mg, mb);
                                    } else {
                                    	utft->setColor(cr, cg, cb);
                                    }
                                    while ( y + len > effHeight ) {
                                        utft->drawLine(x1 + marginLeft + x, yy + marginTop + y, x1 + marginLeft + x, yy + marginTop + effHeight);
                                        len -= effHeight - y;
                                        y = 0;
                                        x++;
                                    }
                                    utft->drawLine(x1 + marginLeft + x, yy + marginTop + y, x1 + marginLeft + x, yy + marginTop + y + len);
                                }
                            } else {
                                if ( clean > 0 ) {
                                   	utft->setColor(mr, mg, mb);
								} else {
	                                int opacity = (0xff & (b * 4));
	                                int sr = (cr * (255 - opacity) + mr * (opacity))/255;
	                                int sg = (cg * (255 - opacity) + mg * (opacity))/255;
	                                int sb = (cb * (255 - opacity) + mb * (opacity))/255;
									utft->setColor(sr, sg, sb);
								}
                                utft->drawPixel(x1 + marginLeft + x, yy + marginTop + y);
                                ctr++;
                            }
                        }

                    } else {

                        for ( int i = 0; i < length - 8; i++ ) {
                            int b = 0xff & pgm_read_byte_near(currentFont + ptr + 8 + i);
                            int x = ctr % effWidth;
                            int y = ctr / effWidth;

                            if ( (0xc0 & b) > 0 ) {
                                int len = 0x3f & b;
                                ctr += len;
                                if ( (0x80 & b) > 0 ) {
	                                if ( clean > 0 ) {
    	                               	utft->setColor(mr, mg, mb);
									} else {
            	                        utft->setColor(cr, cg, cb);
            	                        }
                                    while ( x + len > effWidth ) {
                                        utft->drawLine(x1 + marginLeft + x - 1, yy + marginTop + y, x1 + marginLeft + effWidth - 1, yy + marginTop + y);
                                        len -= effWidth - x;
                                        x = 0;
                                        y++;
                                    }
                                    utft->drawLine(x1 + marginLeft + x - 1, yy + marginTop + y, x1 + marginLeft + x + len - 1, yy + marginTop + y);
                                }
                            } else {
                                if ( clean > 0 ) {
                                   	utft->setColor(mr, mg, mb);
								} else {
	                                int opacity = (0xff & (b * 4));
	                                int sr = (cr * (255 - opacity) + mr * (opacity))/255;
	                                int sg = (cg * (255 - opacity) + mg * (opacity))/255;
	                                int sb = (cb * (255 - opacity) + mb * (opacity))/255;
	                                utft->setColor(sr, sg, sb);
	                            }
                                utft->drawPixel(x1 + marginLeft + x, yy + marginTop + y);
                                ctr++;
                            }
                        }
                    }

                } else if ( fontType == BITMASK_FONT ) {

                    if ( clean > 0 ) {
						utft->setColor(mr, mg, mb);
					} else {
						utft->setColor( (int)cr, (int)cg, (int)cb );
					}

                    boolean compressed = (pgm_read_byte_near(currentFont + ptr + 7) & 0x80) > 0;
                    if ( compressed ) {
                        boolean vraster = (pgm_read_byte_near(currentFont + ptr + 5) & 0x80) > 0;
                        if ( vraster ) {
                            int marginBottom = marginRight;
                            int effHeight = glyphHeight - marginTop - marginBottom;

                            for ( int i = 0; i < length - 8; i++ ) {
                                int len = 0x7f & pgm_read_byte_near(currentFont + ptr + 8 + i);
                                boolean color = (0x80 & pgm_read_byte_near(currentFont + ptr + 8 + i)) > 0;
                                if ( color ) {
                                    int x = ctr / effHeight;
                                    int y = ctr % effHeight;
                                    while ( y + len > effHeight ) {
                                        utft->drawLine(x1 + marginLeft + x, yy + marginTop + y, x1 + marginLeft + x, yy + marginTop + effHeight);
                                        len -= effHeight - y;
                                        ctr += effHeight - y;
                                        y = 0;
                                        x++;
                                    }
                                    utft->drawLine(x1 + marginLeft + x, yy + marginTop + y, x1 + marginLeft + x, yy + marginTop + y + len);
                                }
                                ctr += len;
                            }
                        } else {
                            for ( int i = 0; i < length - 8; i++ ) {
                                int len = 0x7f & pgm_read_byte_near(currentFont + ptr + 8 + i);
                                boolean color = (0x80 & pgm_read_byte_near(currentFont + ptr + 8 + i)) > 0;
                                if ( color ) {
                                    int x = ctr % effWidth;
                                    int y = ctr / effWidth;
                                    while ( x + len > effWidth ) {
                                        utft->drawLine(x1 + marginLeft + x, yy + marginTop + y, x1 + marginLeft + effWidth, yy + marginTop + y);
                                        len -= effWidth - x;
                                        ctr += effWidth - x;
                                        x = 0;
                                        y++;
                                    }
                                    utft->drawLine(x1 + marginLeft + x, yy + marginTop + y, x1 + marginLeft + x + len, yy + marginTop + y);
                                }
                                ctr += len;
                            }
                        }
                    } else {
                        for ( int i = 0; i < length - 8; i++ ) {
                            int b = 0xff & pgm_read_byte_near(currentFont + ptr + 8 + i);
                            int x = i * 8 % effWidth;
                            int y = i * 8 / effWidth;
                            for ( int j = 0; j < 8; j++ ) {
                                if ( x + j == effWidth ) {
                                    x = -j;
                                    y++;
                                }
                                int mask = 1 << (7 - j);
                                if ( (b & mask) == 0 ) {
                                    utft->drawLine(x1 + marginLeft + x + j, yy + marginTop + y, x1 + marginLeft + x + j, yy + marginTop + y + 1);
                                }
                            }
                        }
                    }
                }
                break;
            }
            ptr += length;
        }

        if ( kerning != NULL && kerning[kernPtr] > -100 ) {
            kern = kerning[kernPtr];
            if (kerning[kernPtr+1] > -100) {
                kernPtr++;
            }
        }

        if ( found ) {
            x1 += width;
            if ( kern > -100 ) {
                x1+= kern;
            }
        }
    }

    utft->setColor( cr, cg, cb );
}

int16_t uText::getLineHeight() {
    if ( currentFont == NULL ) {
        return 0;
    }

    int fontType = pgm_read_byte_near(currentFont + 2);
    if ( fontType != ANTIALIASED_FONT && fontType != BITMASK_FONT ) {
        return 0;
    }

    return pgm_read_byte_near(currentFont + 3);
}

int16_t uText::getBaseline() {
    if ( currentFont == NULL ) {
        return 0;
    }

    int fontType = pgm_read_byte_near(currentFont + 2);
    if ( fontType != ANTIALIASED_FONT && fontType != BITMASK_FONT ) {
        return 0;
    }

    return pgm_read_byte_near(currentFont + 4);
}

int16_t uText::getTextWidth(String text, int8_t kerning[]) {
    if ( currentFont == NULL ) {
        return 0;
    }

    int kernPtr = 0;
    int kern = -100; // no kerning
    int x1 = 0;

    for (int t = 0; t < text.length(); t++) {
        char c = text.charAt(t);

        int width = 0;
        boolean found = false;
        int ptr = HEADER_LENGTH;
        while ( 1 ) {
            char cx = (char)(((int)pgm_read_byte_near(currentFont + ptr + 0) << 8) + pgm_read_byte_near(currentFont + ptr + 1));
            if ( cx == 0 ) {
              break;
            }
            int length = (((int)(pgm_read_byte_near(currentFont + ptr + 2) & 0xff) << 8) + (int)(pgm_read_byte_near(currentFont + ptr + 3) & 0xff));

            if ( cx == c ) {
                if ( length < 8 ) {
//						Serial.print( "Invalid "  );
//						Serial.print( c );
//						Serial.println( " glyph definition. Font corrupted?" );
                    break;
                }
                Serial.print( c );
                found = true;
                width = 0xff & pgm_read_byte_near(currentFont + ptr + 4);
            }

            ptr += length;
        }

        if ( kerning != NULL && kerning[kernPtr] > -100 ) {
            kern = kerning[kernPtr];
            if (kerning[kernPtr+1] > -100) {
                kernPtr++;
            }
        }

        if ( found ) {
            x1 += width;
            if ( kern > -100 ) {
                x1+= kern;
            }
        }
    }

    return x1;
}

