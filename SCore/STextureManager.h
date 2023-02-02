#ifndef STEXTUREMANAGER_H_
#define STEXTUREMANAGER_H_

#include"Configer.h"
#include"SColor.h"
#include"SFont.h"

class STextureManager
{
public:
	static STextureManager* instance();
	~STextureManager();

	SDL_Texture* loadTexture(const std::string& filename);
	SDL_Texture* cacheTexture(const std::string& filename);

	SDL_Texture* loadText(const std::string& text,const SFont& font,const SColor&color = SGUI::GlobalColor::Black);
	SDL_Texture* cacheText(const std::string& text,const SFont& font, const SColor& color = SGUI::GlobalColor::Black);

	static void drawTexture(SDL_Texture* tex, SDL_Rect* dstRect);
	static void drawTexture(SDL_Texture* tex, const SDL_Rect& dstRect);
	static void drawTexture(SDL_Texture* tex, int x,int y);
	static void drawTexture(SDL_Texture* tex, const SDL_Rect* srcRect, const SDL_Rect* dstRect);
	static int width(SDL_Texture* tex);
	static int height(SDL_Texture* tex);

	bool precacheTexture(const std::string& tag, const std::string& filename);	//预缓存
	bool precacheTexture(const std::string& tag, const SDL_Texture* tex);		//预缓存
	SDL_Texture* getTexture(const std::string& tag)const;
	static SDL_Texture* texture(const std::string& tag);
private:
	STextureManager();
	std::map<std::string, SDL_Texture*> textures;
};

#endif //STEXTUREMANAGER_H_
