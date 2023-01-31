#ifndef SFONTMANAGER_H_
#define SFONTMANAGER_H_
#include"SFont.h"

class STTF_FontManager
{
public:
	static STTF_FontManager* instance();
	~STTF_FontManager();
	/**
	 * @brief 打开字体并缓存.
	 * @param file 字体名称
	 * @param ptsize 字体大小
	 * @return 打开成功返回字体指针，失败返回NULL
	 */
	TTF_Font* openFont(const std::string& file,int ptsize);
	TTF_Font* cacheFont(const std::string& file, int ptsize);

public:
	static std::vector<std::string> families();

private:
	STTF_FontManager();
	std::map<std::string, TTF_Font*> m_fonts;
	std::map<std::string, std::string> m_families;
	std::map<std::string, std::string> m_familysMap;
};

#endif // !SFONTMANAGER_H_

