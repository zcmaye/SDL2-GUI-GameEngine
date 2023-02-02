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

	void drawLine(int x1, int y1, int x2, int y2);

	void drawRect(SDL_Rect* rect);
	void drawRect(const SDL_Rect& rect);

	void fillRect(SDL_Rect* rect);
	void fillRect(const SDL_Rect& rect);

	void drawText(int x, int y, const std::string& text);
	void drawText(SDL_Rect* rect, const std::string& text,SGUI::Alignments alignment = SGUI::Alignment::AlignCenter,bool clip = false);
	void drawText(SDL_Rect* rect, SDL_Texture* tex, SGUI::Alignments alignment = SGUI::Alignment::AlignCenter, bool clip = false);

	void drawTexture(const SDL_Rect& target, const SDL_Texture* pixmap, const SDL_Rect& source);
	void drawTexture(int x, int y, SDL_Texture* tex);
	//void drawTexture(SDL_Rect* rect, SDL_Texture* tex);
	//void drawTexture(const SDL_Rect& rect, SDL_Texture* tex);

	void drawArc( SDL_Rect* rect, double startAngle, double endAngle);
	void drawArc( int x, int y, int hRadius, int vRadius, double startAngle, double endAngle);

	void drawElipse(SDL_Rect* rect);
	void drawFillElipse(SDL_Rect* rect);
	void drawElipse(const SDL_Rect& rect);
	void drawFillElipse(const SDL_Rect& rect);

	void drawRountRect(SDL_Rect* rect, int hRadius, int vRadius);
	void drawFillRountRect(SDL_Rect* rect, int hRadius, int vRadius);
private:
	SDL_Renderer* ren;
	SColor m_color;
	SFont m_font;
};

#endif