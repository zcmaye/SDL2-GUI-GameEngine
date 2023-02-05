#ifndef SWINDOW_H_
#define SWINDOW_H_

#include"Configer.h"

struct SSize
{
	int w;
	int h;
};

class SWindow
{
public:
	SWindow(SDL_Window* window = nullptr);
	~SWindow();

	SWindow(const SWindow&) = delete;
	SWindow(SWindow&&) = delete;
	SWindow& operator=(const SWindow&) = delete;
	SWindow& operator=(SWindow&&) = delete;
	SWindow& operator=(SDL_Window* window) { this->m_window = window; };
	
	operator bool() { return m_window != nullptr; }
	operator SDL_Window*() { return m_window; }

	Uint32 windowID()const;
	SDL_Window* windowFromID(Uint32 id) const;

	Uint32 windowFlags()const;

	void setWindowTitle(const std::string& title);
	std::string windowTitle()const;

	//void setWindowIcon(SDL_Surface* icon);
	void setWindowIcon(const std::string& icon);
	SDL_Surface* windowIcon()const;

	void setWindowPosition(const SDL_Point& pos);
	SDL_Point windowPostion()const;

	void setWindowSize(const SSize& size);
	SSize windowSize()const;

	void setWindowMinimumSize(const SSize& size);
	SSize windowMinimumSize()const;

	void setWindowMaximumSize(const SSize& size);
	SSize windowMaximumSize()const;
	
	void setWindowBordered(bool bordered);
	void setWindowResizable(bool resizable);
	void setWindowAlwaysOnTop(bool on_top);

	void show();
	void hide();
	void raise();
	void showMaximized();
	void showMinimized();
	void showNormal();
	void showFullScreen();

	void setOpacity(float opacity);
	float opacity()const;

	bool setWindowModalFor(SDL_Window* parent);
	void setFlash(SDL_FlashOperation operation);	//任务栏闪烁

	static SWindow* CreateWindow(const std::string& title, int w, int h, int flags);

	virtual bool event(SDL_Event* ev);
protected:
#if 1
#define Log(s) SDL_Log("%s %d",s,((SDL_WindowEvent*)ev)->windowID)
#else
#define Log
#endif
	virtual void mousePressEvent(SDL_MouseButtonEvent* ev) { Log(__FUNCTION__); };
	virtual void mouseReleaseEvent(SDL_MouseButtonEvent* ev) { Log(__FUNCTION__); };
	virtual void mouseMoveEvent(SDL_MouseMotionEvent* ev) { Log(__FUNCTION__); };
	virtual void mouseWheelEvent(SDL_MouseWheelEvent* ev) { Log(__FUNCTION__); };
	virtual void mouseDoubleClickEvent(SDL_MouseButtonEvent* ev) { Log(__FUNCTION__); };

	virtual void keyPressEvent(SDL_KeyboardEvent* ev) { Log(__FUNCTION__); };
	virtual void keyReleaseEvent(SDL_KeyboardEvent* ev) { Log(__FUNCTION__); };

	virtual void closeEvent(SDL_Event* ev) { Log(__FUNCTION__); delete this; }

	virtual void showEvent(SDL_WindowEvent* ev) {  Log(__FUNCTION__); };
	virtual void hideEvent(SDL_WindowEvent* ev) { Log(__FUNCTION__); }
	virtual void resizeEvent(SDL_WindowEvent* ev) { Log(__FUNCTION__); };
	virtual void moveEvent(SDL_WindowEvent* ev) { Log(__FUNCTION__); };
	virtual void enterEvent(SDL_WindowEvent* ev) { Log(__FUNCTION__); }
	virtual void leaveEvent(SDL_WindowEvent* ev) { Log(__FUNCTION__); }


private:
	SDL_Window* m_window;
	SDL_Surface* m_icon;
};

#endif // !SWINDOW_H_
