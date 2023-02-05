#include "SLineEdit.h"
#include "SPainter.h"
#include "SGameApp.h"
#include "SIndicator.h"
#include "SEvent.h"



SLineEdit::SLineEdit()
{
	setFixedSize(150, 35);

	textChanged.connect([=](const std::string& text)
		{


		});

	setText(u8"hello world 顽石老师好帅呀");
}

SLineEdit::~SLineEdit()
{

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
		for (auto c : m_text)
		{
			auto tex = sApp->TextureManager()->loadText(c.toString(), d->font, d->fColor);
			int w;
			SDL_QueryTexture(tex, nullptr, nullptr, &w, nullptr);
			m_texs.push_back(Texture(tex ,w ));
		}
		emit textChanged(text);

		if (isFullDisplay())
		{
			m_xOffset = 0;
			m_cursor.x = textWidth();
		}
		else
		{
			m_xOffset = textWidth() - contentW;
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
	emit textChanged(m_text.toString());
}

void SLineEdit::paintEvent()
{
	SPainter painter(sApp->renderer_);
	//设置字体，必须设置
	painter.setFont(d->font);
	//绘制边框

	painter.setColor((d->isHovered || d->kFocus) ? d->hColor : d->bColor);
	painter.drawRect(geometry());

	//绘制文字
	if (!m_text.empty())
	{
		//updateTextRenderSize();

		int curW = 0;
		int x = d->x + m_leftMargin;
		int y = d->y + (d->h - textHeight()) / 2;
		for (size_t i = 0; i < m_texs.size(); i++)
		{
			if (curW - m_xOffset >= 0 - m_leftMargin && curW - m_xOffset < contentW)
			{
				//左边不能完全渲染的字符，裁剪一下
				if (curW - m_xOffset < 0 && curW - m_xOffset + m_texs[i].w > 0)
				{
					SDL_Rect srcRect = { SDL_abs(curW - m_xOffset) ,0, m_texs[i].w - SDL_abs(curW - m_xOffset) ,textHeight()};
					SDL_Rect dstRect = { d->x + m_leftMargin ,y,srcRect.w,srcRect.h };
					painter.drawTexture(dstRect, m_texs[i].texture, srcRect);
				}
				//左边不能完全渲染的字符，裁剪一下
				else if (curW - m_xOffset < contentW && curW - m_xOffset + m_texs[i].w > contentW)
				{
					SDL_Rect srcRect = { 0 ,0, SDL_abs(curW - m_xOffset - contentW) ,textHeight() };
					SDL_Rect dstRect = { x - m_xOffset ,y,srcRect.w,srcRect.h };
					painter.drawTexture(dstRect, m_texs[i].texture, srcRect);
					//SDL_Log("curW %d m_xOffset:%d ooo:%d cw:%d", curW, m_xOffset,curW - m_xOffset -contentW,m_texs[i].w);
				}
				//在输入框中的字符能完全渲染
				else
				{
					painter.drawTexture(x - m_xOffset, y, m_texs[i].texture);
				}

			}
			x += m_texs[i].w;
			curW += m_texs[i].w;
		}
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
	int pressX = (ev->x - d->x - m_leftMargin) + m_xOffset;

	bool cursorChanged = false;
	int w = 0;
	for (int i = 0; i < m_texs.size(); i++)
	{
		//点击了某个文字
		if (pressX >= w && pressX <= w + m_texs[i].w)
		{
			m_cursor.cursor = i;
			m_cursor.x = w - m_xOffset;
			cursorChanged = true;
			break;
		}
		w += m_texs[i].w;
	}
	//点击了除文本之外的区域，并且文本能完全显示
	if (!cursorChanged && isFullDisplay())
	{
		if (pressX >= w)
		{
			m_cursor.cursor = m_text.size();
			m_cursor.x = w;
		}
		else if (pressX <= m_texs[0].w)
		{
			m_cursor.cursor = 0;
			m_cursor.x = 0;
		}
	}
}

void SLineEdit::keyPressEvent(SDL_KeyboardEvent* ev)
{
	switch (ev->keysym.sym)
	{
	case SDLK_HOME:
		m_cursor.cursor = 0;
		m_cursor.x = 0;
		m_xOffset = 0;
		break;
	case SDLK_END:
		m_cursor.cursor = m_text.size();
		m_cursor.x = (textWidth() >= contentW ? contentW : textWidth());
		m_xOffset = (textWidth() >= contentW ? textWidth() - contentW : 0);
		break;
	case SDLK_RETURN:
		emit returnPressed();
		break;
	default:

		break;
	}
}

void SLineEdit::resizeEvent(SResizeEvent* ev)
{
	SDL_Log("%d %d %d", m_xOffset, ev->oldw, ev->w);
	if (textWidth() < ev->w - m_leftMargin*2)
	{
		m_xOffset = 0;
	}
	else
	{
		if (m_xOffset > 0)
		{
			m_xOffset -= ev->w - ev->oldw;
		}

		
		//m_xOffset = 0;
		//m_cursor.x = textWidth();
	}
	SDL_Log("%d", m_xOffset);
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

		std::vector<Texture> texs;
		int size = 0;
		for (auto c : str)
		{
			auto tex = sApp->TextureManager()->loadText(c.toString(), d->font, d->fColor);
			int w;
			SDL_QueryTexture(tex, nullptr, nullptr, &w, nullptr);
			size += w;
			texs.push_back(Texture(tex, w));
		}
		m_texs.insert(m_texs.begin() + m_cursor.cursor, texs.begin(), texs.end());

		m_cursor.x += size;
		m_cursor.cursor += str.size();
		emit textChanged(m_text.toString());


		//校正光标位置
		if (m_cursor.x <= contentW)
		{
			//如果把刚输入的数据加入之后，宽度大于了内容宽度
			if (isFullDisplay())
			{
				m_xOffset = 0;
			}
		}
		//不能用esle if，因为上面的判断里面修改了m_cursorX,有可能会越界，如果越界在这里重新调整一下
		if (m_cursor.x > contentW)
		{
			if (isFullDisplay())
			{
				m_xOffset = 0;
			}
			else
			{
				m_xOffset += (m_cursor.x - contentW);
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
				int charW = m_texs[--m_cursor.cursor].w;
				m_text.erase(m_cursor.cursor);

				SDL_DestroyTexture(m_texs[m_cursor.cursor].texture);
				m_texs.erase(m_texs.begin() + m_cursor.cursor);

				emit textChanged(m_text.toString());

				//如果文本能完全展示
				if (isFullDisplay())
				{
					m_cursor.x = m_cursor.x - charW + m_xOffset;	//从不能完全展示到能完全展示时，要把最后的偏移加上
					m_xOffset = 0;
				}
				else
				{
					if (m_xOffset == 0)
					{
						m_cursor.x -= charW;
					}
					else if (m_xOffset > 0)
					{
						m_xOffset -= charW;
					}
				}

				//SDL_Log("srcRect(%d,%d %d*%d)", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h);
			}
		}
		//移动光标
		if (ev->key.keysym.sym == SDLK_LEFT)
		{
			if (m_cursor.cursor > 0)
			{
				m_cursor.x -= m_texs[--m_cursor.cursor].w;//getTextWidth(m_text[--m_cursor.cursor]);
				if (m_cursor.x <= 0)
				{
					m_xOffset -= SDL_abs(m_cursor.x);
					m_cursor.x = 0;
				}
			}

			if (m_cursor.cursor == 0)
			{
				m_xOffset = 0;
				m_cursor.x = 0;
			}
			//SDL_Log("-left--srcRect(%d,%d %d*%d)  m_cursorX:%d ", m_srcRect.x, m_srcRect.y, m_srcRect.w, m_srcRect.h, m_cursorX);			
		}
		else if (ev->key.keysym.sym == SDLK_RIGHT)
		{
			if (m_cursor.cursor < m_text.size())
			{
				m_cursor.x += m_texs[m_cursor.cursor++].w;

				//如果光标移动之后越界了，更新一下
				if (m_cursor.x >= contentW)
				{
					m_xOffset += SDL_abs(m_cursor.x - (contentW));	//越界了多少个像素，加上即可
					m_cursor.x = contentW;
				}

			}
			if (m_cursor.cursor == m_text.size())
			{
				if (isFullDisplay())
				{
					m_xOffset = 0;
					m_cursor.x = textWidth();
				}
				else
				{
					m_xOffset = textWidth() - contentW;
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
		//m_srcRect.w = m_textW;
	}
	else
	{
		m_srcRect.w = contentW;
		//如果光标已经在最后面了，那么挑战一下(这里是为了解决文本不能完全展示，但是又调整了编辑框的宽度时，文本显示不正确的情况)
		if (m_cursor.cursor == m_text.size())
		{
			//m_srcRect.x = m_textW - contentW;
			m_cursor.x = contentW;
		}
	}
	//m_srcRect.y = (m_textH < d->h) ? 0 : (d->h - m_textH) / 2;
	//m_srcRect.h = (m_textH > d->h) ? d->h : m_textH;


}

int SLineEdit::textHeight() const
{
	return TTF_FontHeight(d->font);
}

int SLineEdit::textWidth()const
{
	int totalw = 0;
	for (auto& t : m_texs)
	{
		totalw += t.w;
	}
	return totalw;
}

