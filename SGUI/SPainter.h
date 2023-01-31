#ifndef SPAINTER_H_
#define SPAINTER_H_
#include "Configer.h"
#include "SFont.h"
#include "SColor.h"
class SPainter
{
public:
	SPainter(SDL_Renderer* target);
	SColor color()const;
	void setColor(const SColor& color);

	SFont font()const;
	void setFont(const SFont& font);

	void drawRect(SDL_Rect* rect);
	void drawRect(const SDL_Rect& rect);

	void fillRect(SDL_Rect* rect);
	void fillRect(const SDL_Rect& rect);

	void drawText(int x, int y, const std::string& text);
	void drawText(SDL_Rect* rect, const std::string& text,SGUI::Alignments alignment = SGUI::Alignment::AlignCenter);
private:
	SDL_Renderer* ren;
	SColor m_color;
	SFont m_font;
};

#endif