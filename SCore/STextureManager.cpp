#include "STextureManager.h"
#include "SGameApp.h"

STextureManager* STextureManager::instance()
{
	static STextureManager* ins;
	if (!ins)
	{
		ins = new STextureManager;
	}
	return ins;
}

STextureManager::~STextureManager()
{
	for (auto p : textures)
	{
		SDL_DestroyTexture(p.second);
	}
}

SDL_Texture* STextureManager::loadTexture(const std::string& filename)
{
	auto sfc = IMG_Load(filename.c_str());
	if (!sfc)
	{
		SDL_Log("surface load faild:%s", SDL_GetError());
		return nullptr;
	}
	auto tex = SDL_CreateTextureFromSurface(sApp->renderer_, sfc);
	SDL_FreeSurface(sfc);
	return tex;
}

SDL_Texture* STextureManager::cacheTexture(const std::string& filename)
{
	auto it = textures.find(filename);
	if (it != textures.end())
	{
		return it->second;
	}
	auto tex = loadTexture(filename);
	if (!tex)
	{
		return nullptr;
	}
	textures.insert({ filename,tex });
	return tex;
}

SDL_Texture* STextureManager::loadText(const std::string& text, const SFont& font, const SColor& color)
{
	auto sfc = TTF_RenderUTF8_Blended(font, text.c_str(),color);
	//auto sfc = TTF_RenderUTF8(font, text.c_str(), color,SColor(125,160,25));
	if (!sfc)
	{
		SDL_Log("TTF_Render faild:%s", SDL_GetError());
		return nullptr;
	}
	auto tex = SDL_CreateTextureFromSurface(sApp->renderer_, sfc);
	SDL_FreeSurface(sfc);
	return tex;
}

SDL_Texture* STextureManager::cacheText(const std::string& text, const SFont& font, const SColor& color)
{
	auto key = font.toString() + "," + std::to_string(color) + text;
	auto it = textures.find(key);
	if (it != textures.end())
	{
		return it->second;
	}
	auto tex = loadText(text,font,color);
	if (tex)
	{
		textures.insert({ key,tex });
	}
	//SDL_Log("Key:%s", key.c_str());
	return tex;
}


void STextureManager::drawTexture(SDL_Texture* tex, SDL_Rect* dstRect)
{
	SDL_RenderCopy(sApp->renderer_, tex, NULL, dstRect);
}

void STextureManager::drawTexture(SDL_Texture* tex, const SDL_Rect& dstRect)
{
	drawTexture(tex, (SDL_Rect*) & dstRect);
}

void STextureManager::drawTexture(SDL_Texture* tex, int x, int y)
{
	SDL_Rect r = { x,y };
	if (SDL_QueryTexture(tex, nullptr, nullptr, &r.w, &r.h) != 0)
	{
		SDL_Log("%s QueryTexture failed", __FUNCTION__);
		return;
	}

	STextureManager::drawTexture(tex, &r);
}

void STextureManager::drawTexture(SDL_Texture* tex, const SDL_Rect* srcRect, const SDL_Rect* dstRect)
{
	SDL_RenderCopy(sApp->renderer_, tex, srcRect, dstRect);
}

int STextureManager::width(SDL_Texture* tex)
{
	int w;
	SDL_QueryTexture(tex, nullptr, nullptr, &w, nullptr);
	return w;
}

int STextureManager::height(SDL_Texture* tex)
{
	int h;
	SDL_QueryTexture(tex, nullptr, nullptr, nullptr,&h);
	return h;
}

bool STextureManager::precacheTexture(const std::string& tag, const std::string& filename)
{
	auto tex = loadTexture(filename);
	if (!tex)
	{
		return false;
	}
	if (precacheTexture(tag, tex))
	{
		return true;
	}
	SDL_DestroyTexture(tex);
	return false;
}

bool STextureManager::precacheTexture(const std::string& tag, const SDL_Texture* tex)
{
	auto it = textures.find(tag);
	if (it != textures.end())
	{
		return false;	//已经缓存了
	}

	if (tex)
	{
		textures.insert({ tag,(SDL_Texture*)tex });
		return true;
	}
	return false;
}

SDL_Texture* STextureManager::getTexture(const std::string& tag) const
{
	auto it = textures.find(tag);
	if (it != textures.end())
	{
		return it->second;	
	}
	SDL_Log("%s %s No cache",__FUNCTION__, tag.c_str());
	return nullptr;
}

SDL_Texture* STextureManager::texture(const std::string& tag)
{
	return instance()->getTexture(tag);
}

STextureManager::STextureManager()
{
	//预缓存需要使用的图片
	precacheTexture("switch/off","assets/images/switch-off.png");
	precacheTexture("switch/off-hover","assets/images/switch-off-hover.png");
	precacheTexture("switch/on","assets/images/switch-on.png");
	precacheTexture("switch/on-hover","assets/images/switch-on-hover.png");
}
