#ifndef SCOLOR_H_
#define SCOLOR_H_

#include"Configer.h"

union SColorPrivate
{
	SDL_Color sColor;
	Uint32	  nColor;
};

class SColor
{
public:
	SColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
	SColor(SGUI::GlobalColor color);
	SColor(const SDL_Color& color);
	SColor();
	operator SDL_Color()const { return d.sColor; }
	operator Uint32()const { return d.nColor; }

	int red()const;
	int green()const;
	int blue()const;
	int alpha()const;
private:
	SColorPrivate d;
};


#endif // !FONT_H_


