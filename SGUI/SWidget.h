#ifndef SWIDGET_H_
#define SWIDGET_H_
#include"Configer.h"
#include"../SCore/SFont.h"
#include"../SCore/SColor.h"

struct SWidgetPrivate
{
	std::string objectname;		//对象名
	uint8_t isVisible : 1;		//可见
	uint8_t isHovered : 1;		//鼠标悬停在空间上
	uint8_t kFocus : 1;			//键盘焦点

	int x = 0;
	int y = 0;
	int w = 150;
	int h = 35;

	SColor bColor;
	SColor fColor;
	SColor hColor;
	SFont	 font = SFont("楷体",16);


	SWidgetPrivate()
	{
		isVisible = true;
		isHovered = false;
		kFocus = false;
		bColor = SColor(225, 225, 225);
		fColor = SColor(0,0,0);
		hColor = SColor(229, 241, 251);
	}
};

#define RETRUN_TYPE SWidget*
#define RETURN_VALUE return this
class SWidget
{
public:
	SWidget();
	virtual ~SWidget();

	SWidget(const SWidget& other) = delete;
	SWidget(SWidget&& other) = delete;
	SWidget& operator=(const SWidget& other) = delete;
	SWidget& operator=(SWidget&& other) = delete;

	RETRUN_TYPE setObjectname(const std::string& name);
	std::string objectname()const;

	inline RETRUN_TYPE setVisible(bool visible) { d->isVisible = visible; RETURN_VALUE; };
	inline bool visible() const { return d->isVisible; };
	inline RETRUN_TYPE show() { setVisible(true); RETURN_VALUE;};
	inline RETRUN_TYPE hide() { setVisible(false); RETURN_VALUE;};

	bool contains(const SDL_Rect& rect);
	bool contains(const SDL_Point& point);

	inline bool focus()const { return d->kFocus; }
	inline RETRUN_TYPE setFocus() { d->kFocus = true; RETURN_VALUE;};
	inline RETRUN_TYPE clearFocus() { d->kFocus = false; RETURN_VALUE;};

	inline int& rx() { return d->x; }
	inline int& ry() { return d->y; }
	inline void setWidth(int w) { d->w = w; }
	inline void setHeight(int h) { d->h = h; }
	int width()const;
	int height()const;
	RETRUN_TYPE move(int x, int y);
	RETRUN_TYPE setFixedSize(int w, int h);
	RETRUN_TYPE setGeometry(int x, int y, int w, int h);
	SDL_Rect geometry()const;

	void raise();

	inline RETRUN_TYPE setBackgroundColor(SColor color) { d->bColor = color;  RETURN_VALUE;};
	inline RETRUN_TYPE setforegroundColor(SColor color) { d->fColor = color; RETURN_VALUE;};
	inline RETRUN_TYPE setHoverColor(SColor color) { d->hColor = color; RETURN_VALUE;};
	
	inline RETRUN_TYPE setFont(const SFont& font) { d->font = font; RETURN_VALUE;};
	inline SFont font()const { return d->font; };

	virtual bool event(SDL_Event* ev);
	virtual void paintEvent();
protected:
	virtual void mousePressEvent(SDL_MouseButtonEvent* ev);
	virtual void mouseReleaseEvent(SDL_MouseButtonEvent* ev);
	virtual void mouseMoveEvent(SDL_MouseMotionEvent* ev);
	virtual void mouseWheelEvent(SDL_MouseWheelEvent* ev);
	virtual void mouseDoubleClickEvent(SDL_MouseButtonEvent *ev);

	virtual void keyPressEvent(SDL_KeyboardEvent* ev);
	virtual void keyReleaseEvent(SDL_KeyboardEvent* ev);

	virtual void resizeEvent(SDL_Event* ev);
	virtual void showEvent(SDL_WindowEvent* ev);

	std::unique_ptr<SWidgetPrivate> d;
};

#endif	//SGUIMANAGER_H_
