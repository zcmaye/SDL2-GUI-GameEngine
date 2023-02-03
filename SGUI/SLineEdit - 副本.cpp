#include "SLineEdit.h"
#include "SPainter.h"
#include "SGameApp.h"
#include "SIndicator.h"




SLineEdit::SLineEdit()
{
	setFixedSize(150, 35);

	textChanged.connect([=](const std::string& text)
		{
			//文本改变时，释放掉旧的纹理，重新渲染文本
			SDL_DestroyTexture(m_texture);
			m_texture = nullptr;

			if (text.empty())
				return;

			//文本改变时，重新计算文本尺寸
			TTF_SizeUTF8(d->font, m_text.toString().c_str(), &m_textW, &m_textH);
			//渲染文本
			m_texture = sApp->TextureManager()->loadText(m_text.toString(), d->font, d->fColor);
		
		});

	setText("hello world");
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
		m_text.clear();
		m_text = text;
		emit textChanged(text);

		if (m_textW < contentW)
		{
			m_srcRect.x = 0;
			m_srcRect.w = m_textW;
			m_cursor.x = m_textW;
		}
		else
		{
			m_srcRect.x = m_textW - contentW;
			m_srcRect.w = contentW;
			m_cursor.x = contentW;
		}
		m_cursor.cursor = m_text.size();
	}

}

void SLineEdit::clear()
{
	m_text.clear();
	m_cursor.cursor = { 0 };
	m_cursor.x = { 0 };
	m_cursor.y = { 0 };
	m_srcRect = { 0 };
	m_textW = { 0 };		//文本总宽度
	m_textH = { 0 };
	emit textChanged(m_text.toString());
}

void SLineEdit::paintEvent()
{
	SPainter painter(SGameApp::renderer);
	//设置字体，必须设置
	painter.setFont(d->font);
	//绘制边框

	painter.setColor((d->isHovered || d->kFocus) ? d->hColor : d->bColor);
	painter.drawRect(geometry());

	//绘制文字
	if (!m_text.empty())
	{
		updateTextRenderSize();

		SDL_Rect drect
		{
			d->x + m_leftMargin,
			d->y + (d->h - m_textH) / 2,
			m_srcRect.w,
			m_srcRect.h
		};

		painter.drawTexture(drect, m_texture, m_srcRect);
	}


	//绘制游标
	if (m_cursor.visible && d->kFocus)
	{
		painter.setColor(d->fColor);
		m_cursor.y = (d->h - TTF_FontHeight(d->font)) / 2;
		painter.drawLine(
			m_leftMargin + d->x + m_cursor.x,
			d->y + m_cursor.y,
			m_leftMargin + d->x + m_cursor.x,
			d->y + m_cursor.y + TTF_FontHeight(d->font));

	}




}

void SLineEdit::mouseMoveEvent(SDL_MouseMotionEvent* ev)
{


	SWidget::mouseMoveEvent(ev);
}

void SLineEdit::mousePressEvent(SDL_MouseButtonEvent* ev)
{
	//if (ev->x - d->x > getTextWidth(m_text))
	//{
	//	m_cursor.cursor = m_text.size();
	//	m_cursor.x = getTextWidth(m_text);
	//}
	//else if (ev->x - d->x <= m_leftMargin + getTextWidth(m_text[0]))
	//{
	//	m_cursor.cursor = 0;
	//	m_cursor.x = 0;
	//}

	int realX = ev->x - d->x - m_leftMargin + m_srcRect.x;
	int totalw = 0;
	for(int i =0;i<m_widths.size();++i)
	{
		totalw += m_widths[i];
		SDL_Log(" totalw:%d cw:%d", totalw, m_widths[i]);
		if (realX <= totalw  && realX >= totalw - m_widths[i])
		{
			SDL_Log("----totalw:%d textx:%d cw:%d textW:%d", totalw, m_srcRect.x, m_widths[i],m_textW);
			m_cursor.cursor = i;
			m_cursor.x = totalw - m_widths[i];
			break;
		}
	}
	SDL_Log("realX:%d totalw:%d", realX, totalw);
}

void SLineEdit::keyPressEvent(SDL_KeyboardEvent* ev)
{
	switch (ev->keysym.sym)
	{
	case SDLK_HOME:
		m_cursor.cursor = 0;
		m_cursor.x = 0;
		m_srcRect.x = 0;
		m_srcRect.w = (m_textW > contentW ? contentW : m_textW);
		break;
	case SDLK_END:
		m_cursor.cursor = m_text.size();
		m_cursor.x = (m_textW > contentW ? contentW : m_textW);
		m_srcRect.x = (m_textW > contentW ? m_textW - contentW : 0);
		m_srcRect.w = (m_textW > contentW ? contentW : m_textW);
		break;
	case SDLK_RETURN:
		SDL_Log("return");
		break;
	default:

		break;
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
	}

	//保存输入的文本
	if (ev->type == SDL_TEXTINPUT)
	{
		//记录当前光标所在的字符
		SString str(ev->text.text);
		m_text.insert(m_text.begin() + m_cursor.cursor, str);
		m_cursor.cursor += str.size();

		//文本改变信号
		emit textChanged(m_text.toString());

		m_cursor.x += getTextWidth(str);
		for (auto c : str)
		{
			m_widths.push_back(getTextWidth(c));
		}

		//校正光标位置
		if (m_cursor.x <= contentW)
		{
			//如果把刚输入的数据加入之后，宽度大于了内容宽度
			if (isFullDisplay())
			{
				m_srcRect.x = 0;
			}
		}
		//不能用esle if，因为上面的判断里面修改了m_cursorX,有可能会越界，如果越界在这里重新调整一下
		if (m_cursor.x > contentW)
		{
			if (isFullDisplay())
			{
				m_srcRect.x = 0;
			}
			else
			{
				m_srcRect.x += (m_cursor.x - contentW);
			}

			m_cursor.x = contentW;
		}

		//SDL_Log(u8"srcRect(%d,%d %d*%d)  newTextW：%d  m_textW:%d  m_cursorX:%d", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h, getTextWidth(str), m_textW, m_cursor.x);
	}

	if (ev->type == SDL_KEYDOWN)
	{
		//删除文本
		if (ev->key.keysym.sym == SDLK_BACKSPACE)
		{
			if (!m_text.empty() && m_cursor.cursor > 0)
			{
				int charW = getTextWidth(m_text[m_cursor.cursor - 1]);
				m_text.erase(--m_cursor.cursor);
				emit textChanged(m_text.toString());

				//如果文本能完全展示
				if (isFullDisplay())
				{
					m_cursor.x = m_cursor.x - charW + m_srcRect.x;	//从不能完全展示到能完全展示时，要把最后的偏移加上
					m_srcRect.x = 0;
				}
				else
				{
					//m_cursorX -= charW;
					m_srcRect.x -= charW;
				}

				//SDL_Log("srcRect(%d,%d %d*%d)", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h);

			}
		}
		//移动光标
		if (ev->key.keysym.sym == SDLK_LEFT)
		{
			if (m_cursor.cursor > 0)
			{
				m_cursor.x -= getTextWidth(m_text[--m_cursor.cursor]);
				if (m_cursor.x < 0)
				{
					m_srcRect.x -= SDL_abs(m_cursor.x);
					m_cursor.x = 0;
				}
			}

			if (m_cursor.cursor == 0)
			{
				m_srcRect.x = 0;
				m_cursor.x = 0;
			}
			//SDL_Log("-left--srcRect(%d,%d %d*%d)  m_cursorX:%d ", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h, m_cursorX);			
		}
		else if (ev->key.keysym.sym == SDLK_RIGHT)
		{
			if (m_cursor.cursor < m_text.size())
			{
				m_cursor.x += getTextWidth(m_text[m_cursor.cursor++]);

				//如果光标移动之后越界了，更新一下
				if (m_cursor.x > contentW)
				{
					m_srcRect.x += SDL_abs(m_cursor.x - (contentW));	//越界了多少个像素，加上即可
					m_cursor.x = contentW;
				}

			}
			if (m_cursor.cursor == m_text.size())
			{
				if (isFullDisplay())
				{
					m_srcRect.x = 0;
					m_cursor.x = m_textW;
				}
				else
				{
					m_srcRect.x = m_textW - contentW;
					m_cursor.x = contentW;
				}
			}
			//SDL_Log("-right--srcRect(%d,%d %d*%d) m_cursorX:%d ", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h, m_cursorX);
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

void SLineEdit::updateTextRenderSize()
{
	if (isFullDisplay())
	{
		m_srcRect.w = m_textW;
	}
	else
	{
		m_srcRect.w = contentW;
		//如果光标已经在最后面了，那么挑战一下(这里是为了解决文本不能完全展示，但是又调整了编辑框的宽度时，文本显示不正确的情况)
		if (m_cursor.cursor == m_text.size())
		{
			m_srcRect.x = m_textW - contentW;
			m_cursor.x = contentW;
		}
	}
	m_srcRect.y = (m_textH < d->h) ? 0 : (d->h - m_textH) / 2;
	m_srcRect.h = (m_textH > d->h) ? d->h : m_textH;


}

