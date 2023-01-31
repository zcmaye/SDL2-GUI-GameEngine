#include "SAbstractSlider.h"
#include "SPainter.h"
#include "SGameApp.h"
SAbstractSlider::SAbstractSlider()
	:m_orientation(SGUI::Horizontal)
{
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
	m_max = max;
	m_min = min;
	updateRatio();
}

void SAbstractSlider::setValue(int val)
{
	m_value = val;
	if (orientation() == SGUI::Horizontal)
	{
		m_handleRect.x = d->x + m_value * m_ratio ;
	}
	else if (orientation() == SGUI::Vertical)
	{
		m_handleRect.y = d->y + m_value * m_ratio;
	}

}

void SAbstractSlider::setMaximum(int max)
{
	m_max = max;
	updateRatio();
}

void SAbstractSlider::setMinimum(int min)
{
	m_min = min;
	updateRatio();
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
		//画出手柄
		//painter.drawEllipse(m_handleRect);
		painter.fillRect(m_handleRect);
	}
	else if (m_orientation == SGUI::Vertical)
	{
		//画出凹槽
		painter.fillRect({ d->x + d->w / 3, d->y, d->w / 3, d->h });
		//画出手柄
		//painter.drawEllipse(m_handleRect);
		painter.fillRect(m_handleRect);
	}
}

void SAbstractSlider::mousePressEvent(SDL_MouseButtonEvent* ev)
{
	//if (m_handleRect.contains(ev->button.x, ev->button.y))
	if (ev->x >= m_handleRect.x && ev->x < m_handleRect.x + m_handleRect.w && ev->y >= m_handleRect.y && ev->y < m_handleRect.y + m_handleRect.h)
	{
		m_isPressHandle = true;
	}
}

void SAbstractSlider::mouseReleaseEvent(SDL_MouseButtonEvent* ev)
{
	m_isPressHandle = false;
}

void SAbstractSlider::mouseMoveEvent(SDL_MouseMotionEvent* ev)
{
	if (m_isPressHandle)
	{
#if 1
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
#endif
	}
}

void SAbstractSlider::showEvent(SDL_WindowEvent* ev)
{
	int minLen = SDL_min(d->w, d->h);
	m_handleRect = { d->x, d->y, minLen, minLen };
}

void SAbstractSlider::updateRatio()
{
	int avaLen = SDL_max(d->w, d->h) - SDL_min(d->w, d->h);	//凹槽有效长度（像素）
	m_ratio = (double)avaLen / (m_max - m_min);		//m_value每变化1,滑块移动的像素数量	
}
