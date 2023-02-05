#include "SWindow.h"
#include "SGameApp.h"
SWindow::SWindow(SDL_Window* window)
	:m_window(window)
	, m_icon(nullptr)
{

}

SWindow::~SWindow()
{
	SDL_DestroyWindow(m_window);
}

Uint32 SWindow::windowID() const
{
	return SDL_GetWindowID(m_window);
}

SDL_Window* SWindow::windowFromID(Uint32 id) const
{
	return SDL_GetWindowFromID(id);
}

Uint32 SWindow::windowFlags() const
{
	return SDL_GetWindowFlags(m_window);
}

void SWindow::setWindowTitle(const std::string& title)
{
	SDL_SetWindowTitle(m_window, title.c_str());
}

std::string SWindow::windowTitle() const
{
	return SDL_GetWindowTitle(m_window);
}

//void SWindow::setWindowIcon(SDL_Surface* icon)
//{
//	SDL_SetWindowIcon(m_window,icon);
//}

void SWindow::setWindowIcon(const std::string& icon)
{
	auto sfc = IMG_Load(icon.c_str());
	if (!sfc)
	{
		SDL_Log("surface load faild:%s", SDL_GetError());
		return;
	}
	if (m_icon)
	{
		SDL_FreeSurface(m_icon);
	}
	SDL_SetWindowIcon(m_window, m_icon = sfc);
}

SDL_Surface* SWindow::windowIcon() const
{
	return m_icon;
}

void SWindow::setWindowPosition(const SDL_Point& pos)
{
	SDL_SetWindowPosition(m_window, pos.x, pos.y);
}

SDL_Point SWindow::windowPostion() const
{
	SDL_Point pos;
	SDL_GetWindowPosition(m_window, &pos.x, &pos.y);
	return pos;
}

void SWindow::setWindowSize(const SSize& size)
{
	SDL_SetWindowSize(m_window, size.w, size.h);
}

SSize SWindow::windowSize() const
{
	SSize size;
	SDL_GetWindowSize(m_window, &size.w, &size.h);
	return size;
}

void SWindow::setWindowMinimumSize(const SSize& size)
{
	SDL_SetWindowMinimumSize(m_window, size.w, size.h);
}

SSize SWindow::windowMinimumSize() const
{
	SSize size;
	SDL_GetWindowMinimumSize(m_window, &size.w, &size.h);
	return size;
}

void SWindow::setWindowMaximumSize(const SSize& size)
{
	SDL_SetWindowMaximumSize(m_window, size.w, size.h);
}

SSize SWindow::windowMaximumSize() const
{
	SSize size;
	SDL_GetWindowMaximumSize(m_window, &size.w, &size.h);
	return size;
}

void SWindow::setWindowBordered(bool bordered)
{
	SDL_SetWindowBordered(m_window, (SDL_bool)bordered);
}

void SWindow::setWindowResizable(bool resizable)
{
	SDL_SetWindowResizable(m_window, (SDL_bool)resizable);
}

void SWindow::setWindowAlwaysOnTop(bool on_top)
{
	SDL_SetWindowAlwaysOnTop(m_window, (SDL_bool)on_top);
}

void SWindow::show()
{
	SDL_ShowWindow(m_window);
}

void SWindow::hide()
{
	SDL_HideWindow(m_window);
}

void SWindow::raise()
{
	SDL_RaiseWindow(m_window);
}

void SWindow::showMaximized()
{
	SDL_MaximizeWindow(m_window);
}

void SWindow::showMinimized()
{
	SDL_MinimizeWindow(m_window);
}

void SWindow::showNormal()
{
	SDL_RestoreWindow(m_window);
}

void SWindow::showFullScreen()
{
	SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void SWindow::setOpacity(float opacity)
{
	SDL_SetWindowOpacity(m_window, opacity);
}

float SWindow::opacity() const
{
	float o;
	SDL_GetWindowOpacity(m_window, &o);
	return o;
}

bool SWindow::setWindowModalFor(SDL_Window* parent)
{
	return SDL_SetWindowModalFor(m_window, parent) == 0;
}

void SWindow::setFlash(SDL_FlashOperation operation)
{
	SDL_FlashWindow(m_window, operation);
}

SWindow* SWindow::CreateWindow(const std::string& title, int w, int h, int flags)
{
	return new SWindow(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags));
}

bool SWindow::event(SDL_Event* ev)
{
	SDL_Log("ev Window id:%d  currentId:%d", ev->window.windowID, windowID());
	if (ev->window.windowID != windowID())
		return false;
	SDL_Log("hello world");
	if(windowID() == 1)
		sApp->GUIManager()->event(ev);
	switch (ev->type)
	{
		case SDL_QUIT:
			//closeEvent(ev);
			break;
		case SDL_KEYDOWN:
			keyPressEvent(&ev->key);
			break;
		case SDL_KEYUP:
			keyReleaseEvent(&ev->key);
			break;
		case SDL_MOUSEMOTION:
			//mouseMoveEvent(&ev->motion);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (ev->button.clicks == 1)
			{
				mousePressEvent(&ev->button);
			}
			else
			{
				mousePressEvent(&ev->button);
				mouseDoubleClickEvent(&ev->button);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			mouseReleaseEvent(&ev->button);
			break;
		case SDL_MOUSEWHEEL:
			mouseWheelEvent(&ev->wheel);
			break;
	}
	if (ev->type == SDL_WINDOWEVENT)
	{
		switch (ev->window.event)
		{
		case SDL_WINDOWEVENT_NONE:
			break;
		case SDL_WINDOWEVENT_SHOWN:
			showEvent(&ev->window);
			break;
		case SDL_WINDOWEVENT_HIDDEN:
			hideEvent(&ev->window);
			break;
		case SDL_WINDOWEVENT_EXPOSED:
			Log("exposed");
			break;
		case SDL_WINDOWEVENT_MOVED:
			moveEvent(&ev->window);
			break;
		case SDL_WINDOWEVENT_RESIZED:
			resizeEvent(&ev->window);
			break;
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			break;
		case SDL_WINDOWEVENT_MINIMIZED:
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			break;
		case SDL_WINDOWEVENT_RESTORED:
			break;
		case SDL_WINDOWEVENT_ENTER:
			enterEvent(&ev->window);
			break;
		case SDL_WINDOWEVENT_LEAVE:
			leaveEvent(&ev->window);
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			break;
		case SDL_WINDOWEVENT_CLOSE:
			closeEvent(ev);
			break;
		case SDL_WINDOWEVENT_TAKE_FOCUS:
			break;
		case SDL_WINDOWEVENT_HIT_TEST:
			break;
		case SDL_WINDOWEVENT_ICCPROF_CHANGED:
			break;
		case SDL_WINDOWEVENT_DISPLAY_CHANGED:
			break;
		}
	}
	return false;
}
