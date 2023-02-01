#include "SPainter.h"
#include "SGameApp.h"
SPainter::SPainter(SDL_Renderer* target)
	:ren(target)
	, m_color({ 225,225,225 })
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

void SPainter::drawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(sApp->renderer, x1, y1, x2, y2);
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

void SPainter::drawText(SDL_Rect* rect, const std::string& text, SGUI::Alignments alignment, bool clip)
{
	auto tex = sApp->TextureManager()->cacheText(text, m_font, m_color);
	if (!tex)
		return;

	drawText(rect, tex, alignment, clip);
}

void SPainter::drawText(SDL_Rect* rect, SDL_Texture* tex, SGUI::Alignments alignment, bool clip)
{
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
	case  SGUI::Alignment::AlignBottom | SGUI::Alignment::AlignHCenter:
		vspace = rect->h - texth;
		hspace = (rect->w - textw) / 2;
		break;
	case  SGUI::Alignment::AlignTop | SGUI::Alignment::AlignHCenter:
		vspace = 0;
		hspace = (rect->w - textw) / 2;
		break;
	case  SGUI::Alignment::AlignLeft | SGUI::Alignment::AlignVCenter:
		hspace = 0;
		vspace = (rect->h - texth) / 2;
		break;
	case  SGUI::Alignment::AlignRight | SGUI::Alignment::AlignVCenter:
		hspace = rect->w - textw;
		vspace = (rect->h - texth) / 2;
		break;
	}

	if (!clip)
	{
		SDL_Rect dr = { hspace + rect->x,vspace + rect->y,textw,texth };
		STextureManager::drawTexture(tex, &dr);
	}
	else
	{
		//计算裁切矩形
		SDL_Rect srcrect = { 0,0,textw ,texth };
		srcrect.w = (textw > rect->w ? rect->w : textw);
		srcrect.h = (texth > rect->h ? rect->h : texth);
		//计算目标矩形
		SDL_Rect dstrect = { hspace + rect->x,vspace + rect->y,srcrect.w,srcrect.h };
		//绘制
		STextureManager::drawTexture(tex, &srcrect, &dstrect);
	}
}

void SPainter::drawTexture(const SDL_Rect& target, const SDL_Texture* tex, const SDL_Rect& srcrect)
{
	SDL_RenderCopy(sApp->renderer, (SDL_Texture*)tex, &target, &srcrect);
}




void SPainter::drawArc(SDL_Rect* rect, double startAngle, double endAngle)
{
	//半轴长
	int aHalf = rect->w / 2;
	int bHalf = rect->h / 2;

	int x, y;
	//求出圆上每个坐标点
	for (float angle = startAngle; angle < endAngle; angle += 0.2)
	{
		auto radian = 0.01745 * angle;
		x = (rect->x + aHalf) + aHalf * SDL_cos(-radian);
		y = (rect->y + bHalf) + bHalf * SDL_sin(-radian);
		SDL_RenderDrawPoint(ren, x, y);
	}
}

void SPainter::drawArc(int x, int y, int hRadius, int vRadius, double startAngle, double endAngle)
{
	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//SDL_Rect r = { x,y,hRadius*2,vRadius*2 };
	//SDL_RenderFillRect(renderer, &r);
	//半轴长
	int aHalf = hRadius;
	int bHalf = vRadius;
	//SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	int tx, ty;
	//求出圆上每个坐标点
	for (float angle = startAngle; angle < endAngle; angle += 0.2)
	{
		auto radian = 0.01745 * angle;
		tx = (x + aHalf) + aHalf * SDL_cos(-radian);
		ty = (y + bHalf) + bHalf * SDL_sin(-radian);
		SDL_RenderDrawPoint(ren, tx, ty);
	}
}

void SPainter::drawElipse(SDL_Rect* rect)
{
	drawArc(rect, 0, 360);
}

void SPainter::drawFillElipse(SDL_Rect* rect)
{
	SDL_Rect r = *rect;
	for (; r.w / 2 > 0 && r.h / 2 > 0; r.w -= 2, r.h -= 2, r.x++, r.y++)
	{
		drawArc(&r, 0, 360);
	}
}

void SPainter::drawElipse(const SDL_Rect& rect)
{
	drawElipse((SDL_Rect*) & rect);
}

void SPainter::drawFillElipse(const SDL_Rect& rect)
{
	drawFillElipse((SDL_Rect*)&rect);
}

void SPainter::drawRountRect(SDL_Rect* rect, int hRadius, int vRadius)
{
	SDL_RenderDrawLine(ren, rect->x + hRadius, rect->y, rect->x + rect->w - hRadius, rect->y);
	SDL_RenderDrawLine(ren, rect->x + rect->w, rect->y + vRadius, rect->x + rect->w, rect->y + rect->h - vRadius);
	SDL_RenderDrawLine(ren, rect->x + hRadius, rect->y + rect->h, rect->x + rect->w - hRadius, rect->y + rect->h);
	SDL_RenderDrawLine(ren, rect->x, rect->y + vRadius, rect->x, rect->y + rect->h - vRadius);


	drawArc(rect->x, rect->y, hRadius, vRadius, 90, 180);
	drawArc(rect->x + rect->w - hRadius * 2, rect->y, hRadius, vRadius, 0, 90);
	drawArc(rect->x + rect->w - hRadius * 2, rect->y + rect->h - vRadius * 2, hRadius, vRadius, 270, 360);
	drawArc(rect->x, rect->y + rect->h - vRadius * 2, hRadius, vRadius, 180, 270);
}

void SPainter::drawFillRountRect( SDL_Rect* rect, int hRadius, int vRadius)
{
	SDL_Rect r = *rect;
	while (r.w >= 0 && r.h >= 0)
	{
		drawRountRect(&r, hRadius--, vRadius--);
		++r.x;
		++r.y;
		r.w -= 2;
		r.h -= 2;
	}
}