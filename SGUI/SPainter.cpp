#include "SPainter.h"
#include "SGameApp.h"
SPainter::SPainter(SDL_Renderer* target)
	:ren(target)
{

}

SColor SPainter::color() const
{
	return m_color;
}

void SPainter::setColor(const SColor& color)
{
	m_color = color;
	SDL_SetRenderDrawColor(ren,m_color.red(),m_color.green(),m_color.blue(),m_color.alpha());
}

SFont SPainter::font() const
{
	return m_font;
}

void SPainter::setFont(const SFont& font)
{
	m_font = font;
}

void SPainter::drawRect(SDL_Rect* rect)
{
	SDL_RenderDrawRect(ren, rect);
}
void SPainter::drawRect(const SDL_Rect& rect)
{
	drawRect((SDL_Rect*)&rect);
}

void SPainter::fillRect(SDL_Rect* rect)
{
	SDL_RenderFillRect(ren, rect);
}
void SPainter::fillRect(const SDL_Rect& rect)
{
	fillRect((SDL_Rect*)&rect);
}

void SPainter::drawText(int x, int y, const std::string& text)
{
	auto tex = sApp->TextureManager()->cacheText(text, m_font, m_color);
	if (!tex)
		return;
	STextureManager::drawTexture(tex, x,y);
}

void SPainter::drawText(SDL_Rect* rect, const std::string& text, SGUI::Alignments alignment)
{
	auto tex = sApp->TextureManager()->cacheText(text, m_font, m_color);
	if (!tex)
		return;

	int textw = 0;
	int texth = 0;
	SDL_QueryTexture(tex, nullptr, nullptr, &textw, &texth);

	int hspace = 0;
	int vspace = 0;
	switch (alignment)
	{
	case  SGUI::Alignment::AlignTop:
		vspace = 0;
		break;
	case  SGUI::Alignment::AlignBottom:
		vspace = rect->h - texth;
		break;
	case  SGUI::Alignment::AlignLeft:
		hspace = 0;
		break;
	case  SGUI::Alignment::AlignRight:
		hspace = rect->w - textw;
		break;
	case  SGUI::Alignment::AlignHCenter:
		hspace = (rect->w - textw) / 2;
		break;
	case  SGUI::Alignment::AlignVCenter:
		vspace = (rect->h - texth) / 2;
		break;
	case  SGUI::Alignment::AlignCenter:
		hspace = (rect->w - textw) / 2;
		vspace = (rect->h - texth) / 2;
		break;
	case  SGUI::Alignment::AlignBottom |  SGUI::Alignment::AlignHCenter:
		vspace = rect->h - texth;
		hspace = (rect->w - textw) / 2;
		break;
	case  SGUI::Alignment::AlignTop |  SGUI::Alignment::AlignHCenter:
		vspace = 0;
		hspace = (rect->w - textw) / 2;
		break;
	case  SGUI::Alignment::AlignLeft |  SGUI::Alignment::AlignVCenter:
		hspace = 0;
		vspace = (rect->h - texth) / 2;
		break;
	case  SGUI::Alignment::AlignRight |  SGUI::Alignment::AlignVCenter:
		hspace = rect->w - textw;
		vspace = (rect->h - texth) / 2;
		break;
	}

	SDL_Rect dr = { hspace + rect->x,vspace + rect->y,textw,texth };
	STextureManager::drawTexture(tex, &dr);
}
