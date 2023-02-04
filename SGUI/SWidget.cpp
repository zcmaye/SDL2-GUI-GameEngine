#include "SWidget.h"
#include "SGameApp.h"
#include "SPainter.h"
#include"../SCore/SEvent.h"
SWidget::SWidget()
{
	d.reset(new SWidgetPrivate);
	SDL_Log("%s", __FUNCTION__);
}
SWidget::~SWidget()
{
	SDL_Log("%s %s", __FUNCTION__, d->objectname.c_str());
}

RETRUN_TYPE SWidget::setObjectname(const std::string& name)
{
	d->objectname = name;
	RETURN_VALUE;
}
std::string SWidget::objectname()const
{
	return d->objectname;
}
bool SWidget::contains(const SDL_Rect& rect)
{
	//if (ev->x >= d->x && ev->x < d->x + d->w && ev->y >= d->y && ev->y < d->y + d->h)
	//{
	//	return true;
	//}
	return false;
}
bool SWidget::contains(const SDL_Point& pos)
{
	if (pos.x >= d->x && pos.x < d->x + d->w && pos.y >= d->y && pos.y < d->y + d->h)
	{
		return true;
	}
	return false;
}
void SWidget::setWidth(int w)
{
	if (d->w != w)
	{
		
		SResizeEvent ev = { SEventType::ResizeEvent,d->w,d->h,w,d->h };
		d->w = w;
		resizeEvent(&ev);
	}
}
void SWidget::setHeight(int h)
{
	if (d->h != h)
	{
		SResizeEvent ev = { SEventType::ResizeEvent,d->w,d->h,d->w,h };
		d->h = h;
		resizeEvent(&ev);
	}
}
int SWidget::width() const
{
	return d->w;
}
int SWidget::height() const
{
	return d->h;
}
RETRUN_TYPE SWidget::move(int x, int y)
{
	if (d->x != x || d->y != y)
	{
		d->x = x;
		d->y = y;
		moveEvent(nullptr);
	}

	RETURN_VALUE;
}
RETRUN_TYPE SWidget::setFixedSize(int w, int h)
{
	if (d->w != w || d->h != h)
	{
		SResizeEvent ev = { SEventType::ResizeEvent,d->w,d->h,w,h };
		d->w = w;
		d->h = h;
		resizeEvent(&ev);
	}
	RETURN_VALUE;
}
RETRUN_TYPE  SWidget::setGeometry(int x, int y, int w, int h)
{
	move(x, y);
	setFixedSize(w, h);
	RETURN_VALUE;
}
SDL_Rect SWidget::geometry() const
{
	return SDL_Rect{ d->x,d->y,d->w,d->h };
}
void SWidget::raise()
{
	sApp->GUIManager()->removeWidget(this);
	sApp->GUIManager()->addWidget(this);
}
bool SWidget::event(SDL_Event* ev)
{
	if (!d->isVisible)
		return false;

	switch (ev->type)
	{
	case SDL_KEYDOWN:
		if(d->kFocus)
			keyPressEvent(&ev->key);
		break;
	case SDL_KEYUP:
		if(d->kFocus)
			keyReleaseEvent(&ev->key);
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (contains(SDL_Point{ ev->button.x,ev->button.y }))
		{
			setFocus();

			if (ev->button.clicks == 1)
			{
				mousePressEvent(&ev->button);
			}
			else
			{
				mousePressEvent(&ev->button);
				mouseDoubleClickEvent(&ev->button);
			}
		}
		else
		{
			clearFocus();
		}

		break;
	case SDL_MOUSEBUTTONUP:
		if (d->kFocus)
			mouseReleaseEvent(&ev->button);
		break;
	case SDL_MOUSEMOTION:
		//if (d->kFocus)
		mouseMoveEvent(&ev->motion);
		if (contains(SDL_Point{ ev->button.x,ev->button.y }))
		{
			d->isHovered = true;
		}
		else
		{
			d->isHovered = false;
		}		
		break;
	case SDL_MOUSEWHEEL:
		if (d->kFocus)
			mouseWheelEvent(&ev->wheel);
		break;
	case SDL_WINDOWEVENT:
	{
		switch (ev->window.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
		{
			SResizeEvent ev = { SEventType::ResizeEvent };
			SDL_GetWindowSize(sApp->window, &ev.w, &ev.h);
			resizeEvent(&ev);
		}
			
			break;
		case SDL_WINDOWEVENT_SHOWN:
			showEvent(&ev->window);
			break;
		}
		//SDL_Log("window event %d", ev->window.event);
	}
		break;
	}
	return true;
}

void SWidget::paintEvent()
{
	SPainter painter(SGameApp::renderer);
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

void SWidget::mousePressEvent(SDL_MouseButtonEvent* ev)
{

}

void SWidget::mouseReleaseEvent(SDL_MouseButtonEvent* ev)
{
}

void SWidget::mouseMoveEvent(SDL_MouseMotionEvent* ev)
{

}

void SWidget::mouseWheelEvent(SDL_MouseWheelEvent* ev)
{
}

void SWidget::mouseDoubleClickEvent(SDL_MouseButtonEvent* ev)
{
}

void SWidget::keyPressEvent(SDL_KeyboardEvent* ev)
{
}

void SWidget::keyReleaseEvent(SDL_KeyboardEvent* ev)
{
}

void SWidget::resizeEvent(SResizeEvent* ev)
{
}

void SWidget::showEvent(SDL_WindowEvent* ev)
{
}
