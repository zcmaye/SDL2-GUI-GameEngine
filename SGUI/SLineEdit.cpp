#include "SLineEdit.h"
#include "SPainter.h"
#include "SGameApp.h"
#include "SIndicator.h"
SLineEdit::SLineEdit()
{
	setFixedSize(150, 35);
	TTF_SetFontKerning(d->font,false);
}

void SLineEdit::paintEvent()
{
	SPainter painter(SGameApp::renderer);
	//绘制边框
	painter.setColor(d->bColor);
	painter.drawRect(geometry());

	//绘制文字

	SDL_Texture* tex = nullptr;
	if (!m_text.empty())
	{
		tex = sApp->TextureManager()->loadText(m_text, d->font, d->fColor);
		painter.setColor(d->fColor);

		SDL_Rect r = { d->x + m_leftMargin ,d->y,d->w - m_leftMargin,d->h };
		painter.drawText(&r, m_text, SGUI::AlignLeft | SGUI::AlignVCenter,true);

		SDL_DestroyTexture(tex);
	}

	//绘制游标
	painter.setColor(d->fColor);
	m_cursorY = (d->h - TTF_FontHeight(d->font)) / 2;
	painter.drawLine(m_leftMargin + d->x + m_cursorX, d->y + m_cursorY, m_leftMargin + d->x + m_cursorX, d->y + m_cursorY + TTF_FontHeight(d->font));
}

void SLineEdit::mouseMoveEvent(SDL_MouseMotionEvent* ev)
{

	
	SWidget::mouseMoveEvent(ev);
}

void SLineEdit::mousePressEvent(SDL_MouseButtonEvent* ev)
{

}

bool SLineEdit::event(SDL_Event* ev)
{
	//接受输入
	if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT)
	{
		if (contains(SDL_Point{ev->button.x,ev->button.y}))
		{
			SDL_StartTextInput();
		}
		else
		{
			SDL_StopTextInput();
		}
		
	}
	//判断是否使用了输入法输入
	if (ev->type == SDL_TEXTEDITING)
	{
		if (ev->edit.text[0])
		{
			m_isTextEditing = true;
		}
		else
		{
			m_isTextEditing = false;
		}
		
		SDL_Log("SDL_TEXTEDITING %s", ev->edit.text);
	}

	//保存输入的文本
	if (ev->type == SDL_TEXTINPUT)
	{
		const char* inputText = ev->text.text;
		m_text.append(ev->text.text);

		//获取字符宽度
		m_cursorX += getTextWidth(inputText);
		m_cursor += strlen(inputText);

		SDL_Log("w:%d w/8:%d", getTextWidth(inputText), getTextWidth(inputText) / 8);
		//SDL_Log(u8"%s cursor:%d （%d,%d）", m_text.data(),m_cursor,m_cursorX,m_cursorY);
	}

	
	if (ev->type == SDL_KEYDOWN)
	{
		//删除文本
		if (ev->key.keysym.sym == SDLK_BACKSPACE)
		{
			std::string str;
			if (!m_text.empty() && m_cursor > 0)
			{
				if (m_text[m_cursor - 1] > 0)	//拉丁字符
				{
					str = m_text.substr(m_cursor - 1, 1);
					m_text.erase(--m_cursor);

					m_cursorX -= getTextWidth(str);
				}
				else if (m_text[m_cursor - 1] < 0) //UTF8
				{
					str = m_text.substr(m_cursor - 3, 3);
					m_text.erase(m_cursor-=3,3);

					m_cursorX -= getTextWidth(str);
				}

				SDL_Log("%s  %s", m_text.data(),str.c_str());
			}		
		}
		//移动光标
		if (ev->key.keysym.sym == SDLK_LEFT)
		{
			if(m_cursor>0)
				--m_cursor;
		}
		else if (ev->key.keysym.sym == SDLK_RIGHT)
		{
			if(m_cursor<m_text.size())
				++m_cursor;
		}
	}
	
	return SWidget::event(ev);
}

int SLineEdit::getTextWidth(const std::string& str)
{
	int w = 0;
	TTF_SizeUTF8(d->font, str.data(), &w, nullptr);
	return w;
}

