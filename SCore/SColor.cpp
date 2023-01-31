#include "SColor.h"

SColor::SColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	:d{ {r,g,b,a } }
{
}

SColor::SColor(SGUI::GlobalColor color)
	:d{ 
		{
		(Uint8)(color)
		,(Uint8)(color>>8)
		,(Uint8)(color>>16)
		,(Uint8)(color>>24)
		} 
	}
{

}

SColor::SColor(const SDL_Color& color)
	:d{color}
{
}

SColor::SColor()
	:d{SGUI::GlobalColor::Black}
{
}

int SColor::red() const
{
	return d.sColor.r;
}

int SColor::green() const
{
	return d.sColor.g;
}

int SColor::blue() const
{
	return d.sColor.b;
}

int SColor::alpha() const
{
	return d.sColor.a;
}
