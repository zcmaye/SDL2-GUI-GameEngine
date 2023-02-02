#ifndef _SLINEEDIT_H_
#define _SLINEEDIT_H_
#include "SWidget.h"
#include "SString.h"
class SLineEdit : public SWidget
{
public:
	SLineEdit();
public:
	std::function<void(std::string)> onTextChanged;
protected:
	void paintEvent()override;
	void mouseMoveEvent(SDL_MouseMotionEvent* ev)override;
	void mousePressEvent(SDL_MouseButtonEvent* ev)override;
	bool event(SDL_Event* ev)override;
	int getTextWidth(const SString& str);
private:
	SString m_text;

	int m_cursor{0};
	int m_cursorX{ 0 };
	int m_cursorY{ 0 };

	int m_leftMargin = 5;

	SDL_Rect m_srcRect{0};
	int m_textW{0};		//文本总宽度
	int m_textH{0};

	bool m_isTextEditing = false;;
};

#endif