#ifndef _SLINEEDIT_H_
#define _SLINEEDIT_H_
#include "SWidget.h"

class SLineEdit : public SWidget
{
public:
	SLineEdit();
protected:
	void paintEvent()override;
	void mouseMoveEvent(SDL_MouseMotionEvent* ev)override;
	void mousePressEvent(SDL_MouseButtonEvent* ev)override;
	bool event(SDL_Event* ev)override;
	int getTextWidth(const std::string& str);
private:
	std::string m_text;

	int m_cursor{0};
	int m_cursorX{ 0 };
	int m_cursorY{ 0 };

	int m_leftMargin = 5;

	int cw;
	bool m_isTextEditing = false;;
};

#endif