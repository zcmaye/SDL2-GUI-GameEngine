#include "SLineEdit.h"
#include "SPainter.h"
#include "SGameApp.h"
#include "SIndicator.h"


SLineEdit::SLineEdit()
{
	setFixedSize(150, 35);
	TTF_SetFontKerning(d->font,0);
	SDL_Log("lineskip:%d", TTF_FontLineSkip(d->font));
}

void SLineEdit::paintEvent()
{
	SPainter painter(SGameApp::renderer);
	painter.setFont(d->font);
	//绘制边框
	painter.setColor(d->bColor);
	painter.drawRect(geometry());

	//绘制文字
	SDL_Texture* tex = nullptr;
	if (!m_text.empty())
	{
		tex = sApp->TextureManager()->loadText(m_text.toString(), d->font, d->fColor);
		painter.setColor(d->fColor);

		int tw = 0;
		int th = 0;
		SDL_QueryTexture(tex, nullptr, nullptr, &tw, &th);
		m_textW = tw;
		m_textH = th;

		//光标移动文字滚动未实现
		m_srcRect.x = (tw < d->w) ? 0 : tw - d->w + m_leftMargin * 2;
		m_srcRect.y = (th < d->h) ? 0 : (d->h - th) / 2;
		m_srcRect.w = (tw > d->w) ? d->w - m_leftMargin * 2 : tw;
		m_srcRect.h = (th > d->h) ? d->h : th;

		SDL_Rect drect
		{	d->x + m_leftMargin,
			d->y + (d->h-th)/2,
			m_srcRect.w,
			m_srcRect.h
		};

		STextureManager::drawTexture(tex, &m_srcRect, &drect);
		//painter.drawText(&r, m_text.toString(), SGUI::AlignLeft | SGUI::AlignVCenter, true);
		
		SDL_DestroyTexture(tex);
	}

	//绘制游标
	painter.setColor(d->fColor);
	m_cursorY = (d->h - TTF_FontHeight(d->font)) / 2;
	painter.drawLine(
		m_leftMargin + d->x + m_cursorX, 
		d->y + m_cursorY, 
		m_leftMargin + d->x + m_cursorX, 
		d->y + m_cursorY + TTF_FontHeight(d->font));
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
		m_text.push_back(ev->text.text);

		//获取字符宽度
		//m_cursorX += getTextWidth(inputText);
		//m_cursor += SString(inputText).size();
		if (m_cursorX < d->w - m_leftMargin*2)
		{
			m_cursorX += getTextWidth(inputText);
		}
		m_cursor += SString(inputText).size();

		if (onTextChanged) onTextChanged(m_text.toString());

		SDL_Log("w:%d total w:%d", getTextWidth(inputText),getTextWidth(m_text));
		SDL_Log(u8"%s cursor:%d （%d,%d）", m_text.toString().c_str(),m_cursor,m_cursorX,m_cursorY);
	}
	
	if (ev->type == SDL_KEYDOWN)
	{
		//删除文本
		if (ev->key.keysym.sym == SDLK_BACKSPACE)
		{
			if (!m_text.empty() && m_cursor > 0)
			{		
				m_cursorX -= getTextWidth(m_text[m_cursor - 1]);

				m_text.erase(--m_cursor);
				SDL_Log("m_cursor : %d", m_cursor);
			}		
		}
		//移动光标
		if (ev->key.keysym.sym == SDLK_LEFT)
		{
			if (m_cursor > 0 && m_cursorX > 0)
			{
				m_cursorX -= getTextWidth(m_text[--m_cursor]);
			}
			//如果文本过长
			if (m_cursor > 0&&m_textW > d->w - m_leftMargin * 2 && m_cursorX <= 0)
			{
				m_srcRect.x -= getTextWidth(m_text[--m_cursor]);
			}
			SDL_Log(u8"---- cursor:%d （%d,%d）", m_cursor, m_cursorX, m_cursorY);
		}
		else if (ev->key.keysym.sym == SDLK_RIGHT)
		{
			if (m_cursor < m_text.size() && m_cursorX < d->w)
			{
				m_cursorX += getTextWidth(m_text[m_cursor++]);
			}
		}
	}
	
	return SWidget::event(ev);
}

int SLineEdit::getTextWidth(const SString& str)
{
	int w = 0;
	TTF_SizeUTF8(d->font, str.toString().data(), &w, nullptr);
	return w;
}

