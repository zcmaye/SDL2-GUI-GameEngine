#include "SSlider.h"
#include "SPainter.h"
#include "SGameApp.h"
#include "SIndicator.h"
SSlider::SSlider()
{
	setFixedSize(200, 10);
	updateRatio();
	d->hColor = SColor(104, 104, 104);
}

void SSlider::paintEvent()
{
	SPainter painter(SGameApp::renderer);
	//画出凹槽
	painter.setColor(d->bColor);
	painter.fillRect({ d->x, d->y, d->w, d->h });
	//画出手柄
	m_indicator->raise();
	//painter.setColor(m_hoverHandle ? d->hColor : m_handColor);
	//painter.fillRect(m_handleRect);
}

void SSlider::mouseMoveEvent(SDL_MouseMotionEvent* ev)
{

	SAbstractSlider::mouseMoveEvent(ev);
}
