#ifndef _SLINEEDIT_H_
#define _SLINEEDIT_H_
#include "SWidget.h"
#include "SString.h"
#define contentW (d->w - m_leftMargin * 2)	//内容矩形宽度
class SLineEdit : public SWidget
{
public:
	SLineEdit();
	~SLineEdit();

	std::string text()const;
	void setText(const std::string& text);
	void clear();
public:
	SSignal<void(const std::string&)> textChanged;
protected:
	void paintEvent()override;
	void mouseMoveEvent(SDL_MouseMotionEvent* ev)override;
	void mousePressEvent(SDL_MouseButtonEvent* ev)override;
	void keyPressEvent(SDL_KeyboardEvent* ev)override;
	bool event(SDL_Event* ev)override;
	int getTextWidth(const SString& str);

	//更新实际渲染的文本宽度
	void updateTextRenderSize();
	//文本是否能完全显示到编辑框内
	inline bool isFullDisplay() { return m_textW <= contentW; }
private:
	SString m_text;
	SDL_Texture* m_texture = nullptr;

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