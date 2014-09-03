/*
  UTFT_Geometry.h - Add-on Library for UTFT: Geometric functions
  Copyright (C)2013 Henning Karlsen. All right reserved
  
  This library adds some geometric functions to UTFT which are not
  included in the main library.

  You can always find the latest version of the library at 
  http://electronics.henningkarlsen.com/

  If you make any modifications or improvements to the code, I would 
  appreciate that you share the code with me so that I might include 
  it in the next release. I can be contacted through 
  http://electronics.henningkarlsen.com/contact.php.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.
*/

#ifndef UTFT_Geometry_h
#define UTFT_Geometry_h

#include <UTFT.h>
#if ((!defined(UTFT_VERSION)) || (UTFT_VERSION<241))
	#error : You will need UTFT v2.41 or higher to use this add-on library...
#endif

class UTFT_Geometry
{
	public:
		UTFT_Geometry(UTFT *ptrUTFT);

		void	drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
		void	fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
		void	drawArc(int x, int y, int r, int startAngle, int endAngle, int thickness = 1);
		void	drawPie(int x, int y, int r, int startAngle, int endAngle);

	protected:
		UTFT *_UTFT;
};

#endif
