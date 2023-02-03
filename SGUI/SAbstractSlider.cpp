#include "SAbstractSlider.h"
#include "SPainter.h"
#include "SGameApp.h"
#include "SIndicator.h"
SAbstractSlider::SAbstractSlider()
	:m_orientation(SGUI::Horizontal)
{
	m_indicator = sApp->GUIManager()->addWidget(new SIndicator(this));
	//setMouseTracking(true);
	setFixedSize(150, 20);
	updateRatio();
}

void SAbstractSlider::setOrientation(SGUI::Orientation ori)
{
	if (ori == SGUI::Horizontal && m_orientation == SGUI::Vertical
		|| ori == SGUI::Vertical && m_orientation == SGUI::Horizontal)
	{
		int t = d->w;
		d->w = d->h;
		d->h = t;
	}
	m_orientation = ori;
}

SGUI::Orientation SAbstractSlider::orientation() const
{
	return m_orientation;
}

void SAbstractSlider::setRange(int min, int max)
{
	if (m_min != min || m_max != max)
	{
		m_max = max;
		m_min = min;
		updateRatio();
		emit rangeChanged(m_min, m_max);
	}

}

void SAbstractSlider::setValue(int val)
{
	if (m_value != val)
	{
		m_value = val;
		emit valueChanged(m_value);

		if (orientation() == SGUI::Horizontal)
		{
			m_indicator->rx() = d->x + m_value * m_ratio;
		}
		else if (orientation() == SGUI::Vertical)
		{
			m_indicator->ry() = d->y + m_value * m_ratio;
		}
	}
}

void SAbstractSlider::setMaximum(int max)
{
	if (m_max != max)
	{

		m_max = max;
		updateRatio();
		emit rangeChanged(m_min, m_max);
	}
}

void SAbstractSlider::setMinimum(int min)
{
	if (m_min != min)
	{
		m_min = min;
		updateRatio();
		emit rangeChanged(m_min, m_max);
	}
}

int SAbstractSlider::maximum() const
{
	return m_max;
}

int SAbstractSlider::minimum() const
{
	return m_min;
}

int SAbstractSlider::value() const
{
	return m_value;
}

void SAbstractSlider::paintEvent()
{
	SPainter painter(SGameApp::renderer);
	if (m_orientation == SGUI::Horizontal)
	{
		//画出凹槽
		painter.fillRect({ d->x, d->y + d->h / 3, d->w, d->h / 3 });

	}
	else if (m_orientation == SGUI::Vertical)
	{
		//画出凹槽
		painter.fillRect({ d->x + d->w / 3, d->y, d->w / 3, d->h });
	}
	//画出手柄
	//m_indicator->paintEvent();
}

void SAbstractSlider::mouseMoveEvent(SDL_MouseMotionEvent* ev)
{
	if (m_indicator->isDown())
	{
#if 0
		if (m_orientation == SGUI::Horizontal)
		{
			//移动滑块
			m_handleRect.x = ev->x - m_handleRect.w / 2;
			//限定滑块的滑动位置
			if (m_handleRect.x < d->x)
			{
				m_handleRect.x = d->x;
			}
			else if (m_handleRect.x + m_handleRect.w > d->x + d->w)
			{
				m_handleRect.x = d->x + d->w - m_handleRect.w;
			}
			//正常移动的时候，改变数据
			m_distance = m_handleRect.x - d->x;		//滑块当前相对于起点的距离
		}
		else if (m_orientation == SGUI::Vertical)
		{
			m_handleRect.y = ev->y - m_handleRect.h / 2;
			if (m_handleRect.y <= d->y)
			{
				m_handleRect.y = d->y;
			}
			else if (m_handleRect.y+ m_handleRect.h > d->y + d->h)
			{
				m_handleRect.y = d->y + d->h - m_handleRect.h;
			}
			m_distance = m_handleRect.y - d->y;		//滑块当前相对于起点的距离
		}

		//根据m_distance求出当前值
		//if ((int)m_ratio != 0 && m_distance % (int)m_ratio == 0)
		{
			if (m_value != m_distance / m_ratio)
			{
				m_value = m_distance / m_ratio;
				if (onValueChanged)
				{
					onValueChanged(m_value);
				}
				sclog <<"valueChanged" << m_value << " " << m_distance << " " << m_ratio << std::endl;
			}		
		}	
#else
		if (m_orientation == SGUI::Horizontal)
		{
			//移动滑块
			m_indicator->rx() = ev->x - m_indicator->width() / 2;
			//限定滑块的滑动位置
			if (m_indicator->rx() < d->x)
			{
				m_indicator->rx() = d->x;
			}
			else if (m_indicator->rx() + m_indicator->width() > d->x + d->w)
			{
				m_indicator->rx() = d->x + d->w - m_indicator->width();
			}
			//正常移动的时候，改变数据
			m_distance = m_indicator->rx() - d->x;		//滑块当前相对于起点的距离
		}
		else if (m_orientation == SGUI::Vertical)
		{
			m_indicator->ry() = ev->y - m_indicator->height() / 2;
			if (m_indicator->ry() <= d->y)
			{
				m_indicator->ry() = d->y;
			}
			else if (m_indicator->ry() + m_indicator->height() > d->y + d->h)
			{
				m_indicator->ry() = d->y + d->h - m_indicator->height();
			}
			m_distance = m_indicator->ry() - d->y;		//滑块当前相对于起点的距离
		}

		//根据m_distance求出当前值
		//if ((int)m_ratio != 0 && m_distance % (int)m_ratio == 0)
		{
			if (m_value != m_distance / m_ratio)
			{
				m_value = m_distance / m_ratio;
				emit valueChanged(m_value);
				sclog << "valueChanged" << m_value << " " << m_distance << " " << m_ratio << std::endl;
			}
		}
#endif
		emit sliderMoved(m_value);
	}
}

void SAbstractSlider::showEvent(SDL_WindowEvent* ev)
{
	int minLen = SDL_min(d->w, d->h);
	m_indicator->setGeometry( d->x, d->y, minLen, minLen );
}

void SAbstractSlider::updateRatio()
{
	int avaLen = SDL_max(d->w, d->h) - SDL_min(d->w, d->h);	//凹槽有效长度（像素）
	m_ratio = (double)avaLen / (m_max - m_min);		//m_value每变化1,滑块移动的像素数量	
}
