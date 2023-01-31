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
	static bool init(const std::string& title,int w,int h);
	static void clean();
	static void update();
	static void render();
	static void handleEvents();
	static int exec();

	/*@quit*/
	inline static void quit() { isRunning = false; };

	/*@Managers*/
	SGUIManager* GUIManager() { return SGUIManager::instance(); };
	STextureManager* TextureManager() { return STextureManager::instance(); }

public:
	static bool isRunning;
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static SDL_Event events;
};



