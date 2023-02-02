#include "SLineEdit.h"
#include "SPainter.h"
#include "SGameApp.h"
#include "SIndicator.h"


SLineEdit::SLineEdit()
{
	setFixedSize(150, 35);
	TTF_SetFontKerning(d->font,0);
	SDL_Log("lineskip:%d", TTF_FontLineSkip(d->font));

	onTextChanged = [=](const std::string& text)
	{
		//文本改变时，重新计算文本尺寸
		TTF_SizeUTF8(d->font, m_text.toString().c_str(), &m_textW, &m_textH);

		//计算哪些文字显示出来
		//m_srcRect.x = (m_textW < d->w) ? 0 : m_textW - d->w + m_leftMargin * 2;
		m_srcRect.y = (m_textH < d->h) ? 0 : (d->h - m_textH) / 2;
		//m_srcRect.w = (m_textW > d->w) ? d->w - m_leftMargin * 2 : m_textW;
		m_srcRect.h = (m_textH > d->h) ? d->h : m_textH;

		//SDL_Log(u8"%s cursor:%d （%d,%d）", m_text.toString().c_str(), m_cursor, m_cursorX, m_cursorY);
		//SDL_Log("Rect(%d,%d %d*%d)", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h);
	};
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

		SDL_Rect drect
		{	d->x + m_leftMargin,
			d->y + (d->h-m_textH)/2,
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
		//记录当前光标所在的字符
		SString str(ev->text.text);
		m_text.insert(m_text.begin() + m_cursor, str);
		m_cursor += str.size();
		

		//文本改变信号
		if (onTextChanged) onTextChanged(m_text.toString());

		//更新光标位置
		if (m_cursorX < d->w - m_leftMargin * 2 /*&& m_textW <= d->w - m_leftMargin * 2*/)
		{
			m_cursorX += getTextWidth(str);
		}
		else
		{
			m_cursorX = d->w - m_leftMargin * 2;
		}

		int charW = getTextWidth(str);
		//更新需要绘制出来的文本的x
		if (m_textW < d->w)
			m_srcRect.x = 0;
		else
			m_srcRect.x = m_textW - d->w + m_leftMargin * 2;

		//更新需要绘制出来的文本的w
		if (m_srcRect.w < d->w - m_leftMargin * 2)
			m_srcRect.w += charW;
		else
			m_srcRect.w = d->w - m_leftMargin * 2;

		SDL_Log("charW:%d m_cursorX:%d   %s", charW,m_cursorX,m_text.toString().c_str());
		SDL_Log("textsize:%d m_cursor:%d", m_text.size(), m_cursor);
		SDL_Log("Rect(%d,%d %d*%d)", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h);
	}
	
	if (ev->type == SDL_KEYDOWN)
	{
		//删除文本
		if (ev->key.keysym.sym == SDLK_BACKSPACE)
		{
			if (!m_text.empty() && m_cursor > 0)
			{		
				int charW = getTextWidth(m_text[m_cursor - 1]);

				m_text.erase(--m_cursor);

				if (onTextChanged) onTextChanged(m_text.toString());
			
				if (m_textW <= d->w)
					m_srcRect.x = 0;
				else
					m_srcRect.x -= charW;
				
				if(m_textW <= d->w * m_leftMargin * 2)
					m_cursorX -= charW;


				if (m_srcRect.w > 0)
				{
					SDL_Log("1 charW %d srcRect.w %d", charW, m_srcRect.w);
					m_srcRect.w -= (m_textW > d->w) ? 0 : charW;
					SDL_Log("2 charW %d srcRect.w %d", charW,m_srcRect.w);
				}
								
			}		
		}
		//移动光标
		if (ev->key.keysym.sym == SDLK_LEFT)
		{	
			if(m_cursor > 0)
			{
				m_cursorX -= getTextWidth(m_text[--m_cursor]);
				//不能完全展示
				if (m_srcRect.x > 0)
				{
					//光标在最左的的时候需要校正位置
					if (m_cursorX < 0)
					{			
						m_srcRect.x -= SDL_abs(m_cursorX);
						m_cursorX = 0;
					}
				}
			}
			SDL_Log(u8"-left--- cursor:%d （%d,%d）", m_cursor, m_cursorX, m_cursorY);
		}
		else if (ev->key.keysym.sym == SDLK_RIGHT)
		{
			//if (m_cursor < m_text.size())
			//{
			//	m_cursorX += getTextWidth(m_text[m_cursor++]);
			//
			//}

			if (m_cursor < m_text.size() && m_cursorX < d->w - m_leftMargin *2)
			{
				m_cursorX += getTextWidth(m_text[m_cursor++]);
			}

			//如果文本过长
			else if (m_cursor < m_text.size() && m_textW > d->w - m_leftMargin * 2 && m_cursorX >= d->w - m_leftMargin*2)
			{
				m_srcRect.x += getTextWidth(m_text[m_cursor++]);
			}
			SDL_Log(u8"-right--- cursor:%d （%d,%d）", m_cursor, m_cursorX, m_cursorY);
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

