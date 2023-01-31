#include "SAbstractButton.h"
#include "STextureManager.h"
#include "SPainter.h"
#include "../SCore/SGameApp.h"

SAbstractButton::SAbstractButton()
{
	setGeometry(0, 0, 150, 35);
}

void SAbstractButton::setText(const std::string& text)
{
	m_text = text;
	auto tex = sApp->TextureManager()->cacheText(m_text, d->font, d->fColor);
	SDL_QueryTexture(tex, nullptr, nullptr, &m_textW, &m_textH);
}

void SAbstractButton::mousePressEvent(SDL_MouseButtonEvent* ev)
{
	if (ev->button == SDL_BUTTON_LEFT)
	{
		if(onPressed) onPressed();
	}
}

void SAbstractButton::mouseReleaseEvent(SDL_MouseButtonEvent* ev)
{
	if (ev->button == SDL_BUTTON_LEFT)
	{
		if (onClicked) onClicked();	
		if (onReleased) onReleased();
	}
}
