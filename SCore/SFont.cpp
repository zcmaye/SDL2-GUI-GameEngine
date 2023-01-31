#include "SFont.h"
#include "STTF_FontManager.h"
SFont::SFont(const std::string& family, int pointSize, int weight, bool italic)
	:d(new SFontPrivate(family,pointSize,weight,italic))
{
	
}

SFont::SFont()
	:d(new SFontPrivate)
{
}

SFont::SFont(const SFont& font)
{
	*this->d = *font.d;
}

SFont& SFont::operator=(SFont&& other) noexcept
{
	d.release();
	d = std::move(other.d);	
	return *this;
}

SFont& SFont::operator=(const SFont& font)
{
	*this->d = *font.d;
	return *this;
}

bool SFont::bold() const
{
	
	return d->bold;
}

void SFont::setBold(bool enable)
{
	d->bold = enable;
	TTF_SetFontStyle(d->font, enable ? TTF_GetFontStyle(d->font) | TTF_STYLE_BOLD: TTF_GetFontStyle(d->font) & TTF_STYLE_BOLD);
}

std::string SFont::family() const
{
	return d->family;
}

void SFont::setFamily(const std::string& family)
{
	d->family = family;
}

bool SFont::italic() const
{
	return d->italic;
}

void SFont::setItalic(bool enable)
{
	d->italic = enable;
	TTF_SetFontStyle(d->font, enable ? TTF_GetFontStyle(d->font) | TTF_STYLE_ITALIC : TTF_GetFontStyle(d->font) & TTF_STYLE_ITALIC);
}

int SFont::pixelSize() const
{
	return d->pixelSize;
}

void SFont::setPixelSize(int pixelSize)
{
	d->pixelSize = pixelSize;
}

int SFont::pointSize() const
{
	return d->pointSize;
}

void SFont::setPointSize(int pointSize)
{
	d->pointSize = pointSize;
}

int SFont::weight() const
{
	return d->weight;
}

void SFont::setWeight(int weight)
{
	d->weight = weight;
	TTF_SetFontStyle(d->font, weight);
}

bool SFont::underline() const
{
	return d->underline;
}

void SFont::setUnderline(bool enable)
{
	d->underline = enable;
	TTF_SetFontStyle(d->font, enable ? TTF_GetFontStyle(d->font) | TTF_STYLE_UNDERLINE : TTF_GetFontStyle(d->font) & TTF_STYLE_UNDERLINE);
}

bool SFont::strikeOut() const
{
	return d->strikeOut;
}

void SFont::setStrikeOut(bool enable)
{
	d->strikeOut = enable;
	TTF_SetFontStyle(d->font, enable ? TTF_GetFontStyle(d->font) | TTF_STYLE_STRIKETHROUGH : TTF_GetFontStyle(d->font) & TTF_STYLE_STRIKETHROUGH);
}

std::string SFont::toString()const
{
	return d->family + "," + std::to_string(d->pointSize);
}

/*@SFontPrivate*/
SFontPrivate::SFontPrivate(const std::string& family, int pointSize, int weight, bool italic)
	:bold(false)
	,italic(italic)
	,underline(false)
	,strikeOut(false)
	,family(family)
	,pixelSize(pointSize)
	,pointSize(pointSize)
	,weight(weight)
{
	font = STTF_FontManager::instance()->cacheFont(family, pointSize);
	if (!font)
	{
		SDL_Log("%s font family not found", family.data());
	}
	else
	{
		int flags = 0;
		if (bold || weight == TTF_STYLE_BOLD)
		{
			flags |= TTF_STYLE_BOLD;
		}
		if (italic)
		{
			flags |= TTF_STYLE_ITALIC;
		}
		if (underline)
		{
			flags |= TTF_STYLE_UNDERLINE;
		}
		if (strikeOut)
		{
			flags |= TTF_STYLE_STRIKETHROUGH;
		}
		TTF_SetFontStyle(font, flags);
	}
}

SFontPrivate::SFontPrivate()
	:SFontPrivate(u8"msyh",12)
{
}
