// lib modifiée - Maël 02/2013 - ONLY FOR SSD1289 (sainsmart, see below) WITH ARDUINO MEGA
//Screen : SainSmart 3.2" TFT LCD Display+Touch Panel+PCB adapter SD Slot for Arduino 2560
//Shield : SainSmart TFT LCD Adjustable Shield for Arduino Mega 2560 R3 1280 A082 Plug

// *** Hardwarespecific functions ***
void UTFT::_hw_special_init()
{
}

void UTFT::LCD_Writ_Bus(char VH,char VL)
{   
  PORTA = VH;
  PORTC = VL;
  pulse_low(P_WR, B_WR); 
}

void UTFT::_set_direction_registers(byte mode)
{
	DDRA = 0xFF;
 	DDRC = 0xFF;
}

void UTFT::_fast_fill_16(int ch, int cl, long pix)
{
	long blocks;

	PORTA = ch;
	PORTC = cl;

	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low_WR;// pulse_low(P_WR, B_WR);
		pulse_low_WR;
		pulse_low_WR;
		pulse_low_WR;
		pulse_low_WR;
		pulse_low_WR;
		pulse_low_WR;
		pulse_low_WR;
		pulse_low_WR;
		pulse_low_WR;
		pulse_low_WR;
		pulse_low_WR;
		pulse_low_WR;
		pulse_low_WR;
		pulse_low_WR;
		pulse_low_WR;
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16); i++)
		{
			pulse_low_WR;
		}
}

void UTFT::_fast_fill_8(int ch, long pix)
{
	long blocks;

	PORTA = ch;

	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low_WR; pulse_low_WR;//pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low_WR; pulse_low_WR;
		pulse_low_WR; pulse_low_WR;
		pulse_low_WR; pulse_low_WR;
		pulse_low_WR; pulse_low_WR;
		pulse_low_WR; pulse_low_WR;
		pulse_low_WR; pulse_low_WR;
		pulse_low_WR; pulse_low_WR;
		pulse_low_WR; pulse_low_WR;
		pulse_low_WR; pulse_low_WR;
		pulse_low_WR; pulse_low_WR;
		pulse_low_WR; pulse_low_WR;
		pulse_low_WR; pulse_low_WR;
		pulse_low_WR; pulse_low_WR;
		pulse_low_WR; pulse_low_WR;
		pulse_low_WR; pulse_low_WR;
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16); i++)
		{
			pulse_low_WR; pulse_low_WR;
		}
}
