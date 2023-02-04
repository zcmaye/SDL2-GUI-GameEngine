#include "SIndicator.h"
#include "SPainter.h"
#include "SGameApp.h"
SIndicator::SIndicator(SWidget*parent,SIndicator::ShapeType type)
	:m_type(type)
	, m_parent(parent)
{
	setBackgroundColor(SColor(191, 191, 191));
	setHoverColor(SColor(104, 104, 104));
	setFixedSize(32, 32);
}

void SIndicator::paintEvent()
{
	if (m_parent && !m_parent->visible())
		return;

	SPainter painter(SGameApp::renderer);
	painter.setColor(d->isHovered ? d->hColor : d->bColor);
	switch (m_type)
	{
	case SIndicator::Rect:
	{
		auto rect = geometry();
		painter.fillRect(rect);
	}
		
		break;
	case SIndicator::Elipse:
		painter.drawFillElipse(geometry());
		break;
	default:
		break;
	}

}

void SIndicator::mousePressEvent(SDL_MouseButtonEvent* ev)
{
	if (ev->button == SDL_BUTTON_LEFT)
	{
		m_isDown = true;
	}
}

void SIndicator::mouseReleaseEvent(SDL_MouseButtonEvent* ev)
{
	if (ev->button == SDL_BUTTON_LEFT)
	{
		m_isDown = false;
	}
}
