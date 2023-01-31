#include "SButton.h"
#include "SPainter.h"
#include "SGameApp.h"
SButton::SButton(const std::string& text)
{
	setText(text);
}

SButton::SButton(const std::string& normalTexture, const std::string& hoverTexture)
	:m_normalTexture(normalTexture)
	,m_hoverTexture(hoverTexture)
{
	if (!m_normalTexture.empty())
	{

		auto tex = sApp->TextureManager()->cacheTexture(m_normalTexture);
		int w, h;
		SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
		setFixedSize(w, h);
	}

}

SButton::SButton()
	:SButton("Button")
{
}


void SButton::paintEvent()
{
	SPainter painter(sApp->renderer);
	//如果添加了图片，则不需要自己绘制背景
	if (m_normalTexture.empty())
	{
		if (d->isHovered)
		{
			painter.setColor(d->hColor);
		}
		else
		{
			painter.setColor(d->bColor);
		}

		painter.fillRect({ d->x,d->y,d->w,d->h });
	}

	SDL_Rect rect = { d->x,d->y,d->w,d->h };
	//有图片绘制图片
	if (!m_normalTexture.empty())
	{
		auto destTexture = (d->isHovered && !m_hoverTexture.empty()) ? m_hoverTexture : m_normalTexture;
		STextureManager::drawTexture(sApp->TextureManager()->cacheTexture(destTexture), &rect);
	}
	//有文字绘制文字
	if (!text().empty())
	{
		painter.setFont(d->font);
		painter.setColor(d->fColor);
		painter.drawText(&rect, text());
	}
}