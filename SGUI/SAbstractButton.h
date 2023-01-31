#ifndef SABSTRACTBUTTON_H_
#define SABSTRACTBUTTON_H_
#include"SWidget.h"

class SAbstractButton : public SWidget
{
public:
	SAbstractButton();
	void setText(const std::string& text);
	inline std::string text()const { return m_text; }
public:
	std::function<void(void)> onClicked;
	std::function<void(void)> onPressed;
	std::function<void(void)> onReleased;
protected:
	void mousePressEvent(SDL_MouseButtonEvent* ev)override;
	void mouseReleaseEvent(SDL_MouseButtonEvent* ev)override;

	int m_textW;
	int m_textH;
private:
	std::string m_text;

};

#endif // !SBUTTON_H_
