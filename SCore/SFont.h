#ifndef FONT_H_
#define FONT_H_

#include"Configer.h"

class SFontPrivate
{
public:
	Uint8 bold : 1;			//粗体
	Uint8 italic : 1;		//斜体
	Uint8 underline : 1;	//下划线
	Uint8 strikeOut : 1;	//删除线
	std::string family;		//字体名
	int	pixelSize;			//像素大小
	int pointSize;			//点大小
	int weight;				//字体质量
	TTF_Font* font;			

	SFontPrivate(const std::string& family, int pointSize = -1, int weight = -1, bool italic = false);
	SFontPrivate();
};

class SFont
{
public:
	SFont(const std::string& family, int pointSize = -1, int weight = -1, bool italic = false);
	SFont();
	SFont(const SFont& font);

	SFont& operator=(SFont&& other)noexcept;
	SFont& operator=(const SFont& font);

	bool bold()const;
	void setBold(bool enable);

	std::string family()const;
	void setFamily(const std::string& family);

	bool italic()const;
	void setItalic(bool enable);

	int pixelSize()const;
	void setPixelSize(int pixelSize);

	int pointSize()const;
	void setPointSize(int pointSize);
	//字体质量
	int weight()const;
	void setWeight(int weight);
	//下划线
	bool underline()const;
	void setUnderline(bool enable);
	//删除线
	bool strikeOut()const;
	void setStrikeOut(bool enable);
	//返回字体描述
	std::string toString()const;

	inline operator TTF_Font& ()const { return *d->font; }
	inline operator TTF_Font* ()const { return d->font; }
private:
	std::unique_ptr<SFontPrivate> d;
};


#endif // !FONT_H_


