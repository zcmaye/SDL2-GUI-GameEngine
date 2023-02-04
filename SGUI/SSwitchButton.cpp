#include "SSwitchButton.h"
#include "SPainter.h"
#include "SGameApp.h"
#include "SIndicator.h"
SSwitchButton::SSwitchButton()
	:m_state(false)
	, m_indicator(new SIndicator(this, SIndicator::Elipse))
{
	setFixedSize(64, 32);
	//setOffTexture("assets/images/switch-off.png");
	//setOnTexture("assets/images/switch-on.png");
}

SSwitchButton::~SSwitchButton()
{
	delete m_indicator;
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
	SDL_Rect r = { d->x,d->y,d->w,d->h };
	if (m_offImgFile.empty() && m_onImgFile.empty())
	{
		//绘制边框
		painter.setColor(d->bColor);
		painter.drawRountRect(&r, 20, d->h / 2);

		//绘制指示器
		int spacing = 4;					//指示器距离上下左右边框的距离
		int minSize = SDL_min(r.w, r.h);	//宽高最小的作为指示器的宽度和高度
		m_indicator->rx() = r.x + (m_state ? r.w - minSize + spacing : spacing);
		m_indicator->ry() = r.y + spacing;
		m_indicator->setFixedSize(minSize - spacing * 2, minSize - spacing * 2);
		
		m_indicator->update();
	}
	else
	{
		//绘制图片
		if (!m_state)
		{
			STextureManager::drawTexture(sApp->TextureManager()->getTexture((d->isHovered && !m_offHoverImgFile.empty()) ? m_offHoverImgFile : m_offImgFile), &r);
		}
		else
		{
			STextureManager::drawTexture(sApp->TextureManager()->getTexture((d->isHovered && !m_onHoverImgFile.empty()) ? m_onHoverImgFile : m_onImgFile), &r);
		}
	}
}

void SSwitchButton::mousePressEvent(SDL_MouseButtonEvent* ev)
{
	if (ev->button == SDL_BUTTON_LEFT)
	{
		m_state = !m_state;
		emit switchChanged(m_state);
	}
	SAbstractButton::mousePressEvent(ev);
}

void SSwitchButton::showEvent(SDL_WindowEvent* ev)
{
	emit switchChanged(m_state);
}
