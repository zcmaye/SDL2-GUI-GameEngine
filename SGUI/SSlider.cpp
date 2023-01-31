#include "SSlider.h"
#include "SPainter.h"
#include "SGameApp.h"
SSlider::SSlider()
{
	setFixedSize(200, 10);
	updateRatio();
}

void SSlider::paintEvent()
{
	SPainter painter(SGameApp::renderer);
	//画出凹槽
	painter.setColor(d->bColor);
	painter.fillRect({ d->x, d->y, d->w, d->h });
	//画出手柄
	//painter.drawEllipse(m_handleRect);
	painter.setColor(d->fColor);
	painter.fillRect(m_handleRect);
}
