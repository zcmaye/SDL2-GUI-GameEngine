#include "SLineEdit.h"
#include "SPainter.h"
#include "SGameApp.h"
#include "SIndicator.h"

#define contentW (d->w - m_leftMargin * 2)	//内容矩形宽度


SLineEdit::SLineEdit()
{
	setFixedSize(150, 35);

	onTextChanged = [=](const std::string& text)
	{
		//文本改变时，释放掉旧的纹理，重新渲染文本
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;

		if (text.empty())
			return;

		m_texture = sApp->TextureManager()->loadText(m_text.toString(), d->font, d->fColor);
		//文本改变时，重新计算文本尺寸
		TTF_SizeUTF8(d->font, m_text.toString().c_str(), &m_textW, &m_textH);


		//计算哪些文字显示出来
		//m_srcRect.x = (m_textW < d->w) ? 0 : m_textW - d->w + m_leftMargin * 2;
		m_srcRect.y = (m_textH < d->h) ? 0 : (d->h - m_textH) / 2;
		//m_srcRect.w = (m_textW > d->w) ? contentW : m_textW;
		m_srcRect.h = (m_textH > d->h) ? d->h : m_textH;

		//SDL_Log(u8"%s cursor:%d （%d,%d）", m_text.toString().c_str(), m_cursor, m_cursorX, m_cursorY);
		//SDL_Log("Rect(%d,%d %d*%d)", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h);
	};

	//setText("hello world");
}

SLineEdit::~SLineEdit()
{
	SDL_DestroyTexture(m_texture);
}

std::string SLineEdit::text() const
{
	return m_text.toString();
}

void SLineEdit::setText(const std::string& text)
{
	if (m_text.toString() != text)
	{
		m_text = text;
		if (onTextChanged) onTextChanged(text);
	}
}

void SLineEdit::clear()
{
	m_text.clear();
	m_cursor = { 0 };
	m_cursorX = { 0 };
	m_cursorY = { 0 };
	m_srcRect = { 0 };
	m_textW = { 0 };		//文本总宽度
	m_textH = { 0 };
	if (onTextChanged) onTextChanged(m_text.toString());
}

void SLineEdit::paintEvent()
{
	SPainter painter(SGameApp::renderer);
	painter.setFont(d->font);
	//绘制边框
	painter.setColor(d->bColor);
	painter.drawRect(geometry());

	//绘制文字
	if (!m_text.empty())
	{

		SDL_Rect drect
		{ d->x + m_leftMargin,
			d->y + (d->h - m_textH) / 2,
			m_srcRect.w,
			m_srcRect.h
		};
		//SDL_Log("-paint--srcRect(%d,%d %d*%d)  m_cursorX:%d ", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h, m_cursorX);
		STextureManager::drawTexture(m_texture, &m_srcRect, &drect);
		//painter.drawText(&r, m_text.toString(), SGUI::AlignLeft | SGUI::AlignVCenter, true);	
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
	if (ev->button == SDL_BUTTON_RIGHT)
	{
		clear();
	}
}

bool SLineEdit::event(SDL_Event* ev)
{
	//接受输入
	if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT)
	{
		if (contains(SDL_Point{ ev->button.x,ev->button.y }))
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
		if (m_cursorX < contentW)
		{
			m_cursorX += getTextWidth(str);
		
			//如果光标移动之后，越界了
			if (m_cursorX > contentW)
			{
				m_cursorX = contentW;
			}
			else
			{
				m_srcRect.x -= getTextWidth(str);
			}

			//if(m_textW)
			
		}
		else
		{
			m_cursorX = contentW;
		}
		
		//更新需要绘制出来的文本的x
		if (m_textW <= contentW)
		{
			m_srcRect.x = 0;
			m_srcRect.w = m_textW;
		}
		else
		{
			if (m_cursor == m_text.size())
			{
				m_srcRect.x = m_textW - (contentW);
			}
			else if (m_cursor == 0)
			{

			}
			else
			{
				
				
			}
		
			//m_srcRect.x = m_textW - (contentW);
			//m_srcRect.x -= getTextWidth(str);
			m_srcRect.w = contentW;
		}
		SDL_Log(u8"srcRect(%d,%d %d*%d)  newTextW：%d  m_textW:%d", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h, getTextWidth(str), m_textW);
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

				//如果文本能完全展示
				if (m_textW <= contentW)
				{

					m_cursorX = m_cursorX - charW + m_srcRect.x;	//从不能完全展示到能完全展示时，要把最后的偏移加上
					m_srcRect.x = 0;
					m_srcRect.w = m_textW;
				}
				else
				{
					//m_cursorX -= charW;
					m_srcRect.x -= charW;
					m_srcRect.w = contentW;
					SDL_Log("11111");
				}


				SDL_Log("srcRect(%d,%d %d*%d)", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h);

			}
		}
		//移动光标
		if (ev->key.keysym.sym == SDLK_LEFT)
		{
			if (m_cursor > 0)
			{
				--m_cursor;
				m_cursorX -= getTextWidth(m_text[m_cursor]);

				if (m_cursorX < 0 && m_textW > contentW)
				{
					m_srcRect.x -= SDL_abs(m_cursorX);
					//if (m_srcRect.x < 0)
						//m_srcRect.x = 0;
					m_srcRect.w = contentW;
					m_cursorX = 0;
				}
			}
			                        


			SDL_Log("-left--srcRect(%d,%d %d*%d)  m_cursorX:%d ", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h, m_cursorX);

			//如果没有移动到第一个字符，就可以向左移动
			//if (m_cursor > 0)
			//{
			//
			//	m_cursorX -= getTextWidth(m_text[--m_cursor]);
			//	//光标在最左的的时候需要校正位置
			//	if (m_cursorX < 0)
			//	{
			//		m_srcRect.x -= SDL_abs(m_cursorX);
			//		m_srcRect.w = m_textW > contentW ? contentW : m_textW;
			//		m_cursorX = 0;
			//	}
			//
			//	//不能完全展示
			//	//if (m_srcRect.x > 0)
			//	//{
			//	//	//光标在最左的的时候需要校正位置
			//	//	if (m_cursorX < 0)
			//	//	{			
			//	//		m_srcRect.x -= SDL_abs(m_cursorX);
			//	//		m_cursorX = 0;
			//	//	}
			//	//}
			//	SDL_Log("-left--srcRect(%d,%d %d*%d)  m_cursorX:%d CW:%d", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h, m_cursorX, getTextWidth(m_text[m_cursor]));
			//}
			////否则光标移动到了第一个字符,重置一下
			//else
			//{
			//	m_cursorX = 0;
			//	m_srcRect.x = 0;
			//}
		}
		else if (ev->key.keysym.sym == SDLK_RIGHT)
		{
			if (m_cursor < m_text.size())
			{
				m_cursorX += getTextWidth(m_text[m_cursor]);
				m_cursor++;

				//如果光标移动之后越界了，更新一下
				if (m_cursorX > contentW && m_textW > contentW)
				{
					m_srcRect.x += SDL_abs(m_cursorX - (contentW));	//越界了多少个像素，加上即可
					//if (m_srcRect.x < 0)
					//	m_srcRect.x = 0;

					m_srcRect.w = contentW;
					m_cursorX = contentW;
				}
			}



			SDL_Log("-right--srcRect(%d,%d %d*%d) m_cursorX:%d ", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h, m_cursorX);

			////如果光标没有移动到最后一个字符，就可以向右移动
			//if (m_cursor < m_text.size())
			//{
			//
			//	int w = getTextWidth(m_text[m_cursor++]);
			//	m_cursorX += w;
			//
			//	//如果光标移动之后越界了，更新一下
			//	if (m_cursorX > contentW)
			//		//if(m_srcRect.x == m_textW - (d->w - m_leftMargin*2))
			//	{
			//		m_srcRect.x += SDL_abs(m_cursorX - (contentW));
			//		m_cursorX = contentW;
			//	}
			//	SDL_Log("-right--srcRect(%d,%d %d*%d) m_cursorX:%d CW:%d", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h, m_cursorX, getTextWidth(m_text[m_cursor - 1]));
			//}
			////否则不需要处理
			//else
			//{
			//	if (m_textW > contentW)
			//	{
			//		m_srcRect.x = m_textW - (contentW);
			//		m_srcRect.w = contentW;
			//	}
			//	else
			//	{
			//
			//	}
			//
			//
			//}

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

