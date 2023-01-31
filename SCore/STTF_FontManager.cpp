#include"STTF_FontManager.h"
#include<filesystem>
STTF_FontManager* STTF_FontManager::instance()
{
	static STTF_FontManager* ins = nullptr;
	if (!ins)
	{
		ins = new STTF_FontManager;
	}
	return ins;
}

TTF_Font* STTF_FontManager::openFont(const std::string& file, int ptsize)
{
	TTF_Font* font = nullptr;
	auto it = m_familysMap.find(file);
	if (it != m_familysMap.end())
	{
		font = TTF_OpenFont(m_families[it->second].data(), ptsize);
	}
	else
	{
		font = TTF_OpenFont(m_families[file].data(), ptsize);
	}

	if (!font)
	{
		SDL_Log("font open faild:%s", SDL_GetError());
		return nullptr;
	}
	return font;
}

TTF_Font* STTF_FontManager::cacheFont(const std::string& file, int ptsize)
{
	auto key = file + "," + std::to_string(ptsize);
	auto it = m_fonts.find(key);
	if (it != m_fonts.end())
	{
		return it->second;
	}

	auto font = openFont(file, ptsize);
	if (!font)
	{
		return nullptr;
	}
	m_fonts.insert({ key,font });
	//SDL_Log("font key:%s", key.data());
	return font;
}

std::vector<std::string> STTF_FontManager::families()
{
	std::vector<std::string> fonts;
	std::filesystem::directory_iterator dirit(std::filesystem::path("C:\\Windows\\Fonts"));
	for (auto dir_entry : dirit)
	{
		fonts.push_back(dir_entry.path().string());
	}
	return fonts;
}

STTF_FontManager::STTF_FontManager()
{
	auto fs = families();
	for (auto path : fs)
	{
		auto filename = std::filesystem::path(path).filename().string();
		auto family = filename.substr(0, filename.rfind('.'));
		m_families.insert({ family ,path});
		//std::cout << filename.substr(0, filename.rfind('.')) << "  " << path << std::endl;
	}
	
	m_familysMap.insert({ "微软雅黑"	,"msyh" });
	m_familysMap.insert({ "宋体"	,"simsun" });
	m_familysMap.insert({ "楷体"	,"simkai" });
	m_familysMap.insert({ "仿宋"	,"simfang" });
	m_familysMap.insert({ "黑体"	,"simhei" });
	m_familysMap.insert({ "等线"	,"Deng" });
}

STTF_FontManager::~STTF_FontManager()
{
	for (auto f : m_fonts)
	{
		TTF_CloseFont(f.second);
	}
}