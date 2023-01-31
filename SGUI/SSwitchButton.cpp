#include "SSwitchButton.h"
#include "SPainter.h"
#include "SGameApp.h"

SSwitchButton::SSwitchButton()
	:m_offImgFile("switch/off")
	,m_offHoverImgFile("switch/off-hover")
	,m_onImgFile("switch/on")
	,m_onHoverImgFile("switch/on-hover")
	,m_state(false)
{
	setFixedSize(64, 32);
	//setOffTexture("assets/images/switch-off.png");
	//setOnTexture("assets/images/switch-on.png");
}

void SSwitchButton::setOffTexture(const std::string& offTex, const std::string& offHoverTex)
{
	m_offImgFile = offTex;
	m_offHoverImgFile = offHoverTex;
}

void SSwitchButton::setOnTexture(const std::string& onTex, const std::string& onHoverTex)
{
	m_onImgFile = onTex;
	m_onHoverImgFile = onHoverTex;
}


void SSwitchButton::paintEvent()
{
	SPainter painter(sApp->renderer);
	SDL_Rect rect = { d->x,d->y,d->w,d->h };

	//绘制图片
	if (!m_state)
	{
		STextureManager::drawTexture(sApp->TextureManager()->getTexture((d->isHovered && !m_offHoverImgFile.empty())? m_offHoverImgFile : m_offImgFile), &rect);
	}
	else 
	{
		STextureManager::drawTexture(sApp->TextureManager()->getTexture((d->isHovered && !m_onHoverImgFile.empty())? m_onHoverImgFile : m_onImgFile), &rect);
	}
	//绘制文字
	if (!text().empty())
	{
		painter.setFont(d->font);
		painter.setColor(d->fColor);
		SDL_Rect r = {d->x + d->w,d->y,d->w,d->h};
		painter.drawText(d->x + d->w + 10, (d->h - m_textH)/2 + d->y, text());
	}
}

void SSwitchButton::mousePressEvent(SDL_MouseButtonEvent* ev)
{
	if (ev->button == SDL_BUTTON_LEFT)
	{
		m_state = !m_state;
		if(onSwitchChanged) onSwitchChanged(m_state);
	}
	SAbstractButton::mousePressEvent(ev);
}

void SSwitchButton::showEvent(SDL_WindowEvent* ev)
{
	if (onSwitchChanged) onSwitchChanged(m_state);
}
