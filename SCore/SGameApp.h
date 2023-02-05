#pragma once
#include"Configer.h"
#include"SGUIManager.h"
#include"STextureManager.h"

#define sApp SGameApp::instance()
class SGameApp
{
public:
	SGameApp(int argc, char* argv[]);
	~SGameApp();
	static SGameApp* instance();

	/*@game*/
	bool init(const std::string& title,int w,int h);
	void clean();
	void update();
	void render();
	void handleEvents();
	inline void quit() { isRunning_ = false; };
	inline bool running() { return isRunning_; }
	int exec();



	/*@Managers*/
	SGUIManager* GUIManager() { return SGUIManager::instance(); };
	STextureManager* TextureManager() { return STextureManager::instance(); }

public:
	 bool isRunning_;
	 SDL_Window* window_;
	 SDL_Renderer* renderer_;
	 SDL_Event events_;
};



