#include"SGameApp.h"
#include"STextureManager.h"
#include"STTF_FontManager.h"

#include"SButton.h"
#include"SSwitchButton.h"
#include"SSlider.h"
#include"SLineEdit.h"

bool SGameApp::isRunning = false;
SDL_Window* SGameApp::window;
SDL_Renderer* SGameApp::renderer;
SDL_Event SGameApp::events;
SGameApp* gIns = nullptr;

SGameApp::SGameApp(int argc, char* argv[])
{
	gIns = this;

}

SGameApp::~SGameApp()
{
	gIns = nullptr;
}

SGameApp* SGameApp::instance()
{
	if (!gIns)
	{
		SDL_Log("Create an application example first!");
		SDL_assert(gIns != nullptr);
	}
	return gIns;
}

int SGameApp::exec()
{
	Uint32 startTime = 0;
	while (isRunning)
	{
		startTime = SDL_GetTicks();
		SDL_SetRenderDrawColor(renderer, 250, 250, 250, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		update();
		render();
		handleEvents();

		SDL_RenderPresent(renderer);
		//SDL_Log("times %u", SDL_GetTicks() - startTime);

	}
	clean();

	bool ret = _CrtDumpMemoryLeaks();
	if (ret)
	{
		_STL_ASSERT(ret == false, "Have a memory leak");
	}
	return 0;
}

bool SGameApp::init(const std::string& title, int w, int h)
{
	if (isRunning)
	{
		SDL_Log("game already init!\n");
		return true;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)
		&& TTF_Init() == 0)
	{
		SDL_Log("game init!\n");
		//创建窗口
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
		if (window)
		{
			SDL_Log("window created!\n");
		}

		//创建渲染器
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_Log("renderer created!\n");
		}
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	
	for (int i = 0; i < 5; i++)
	{
		auto btn = sApp->GUIManager()->addWidget(new SButton);
		btn->move(0, i * (35 + 5))
		   ->setObjectname("button"+std::to_string(i));

		btn->clicked.connect([=]() 
			{
				SDL_Log("button clicked %d", i); 
			});
		btn->pressed.connect([=] {SDL_Log("button released %d", i); });
		btn->setText(u8"顽石" + std::to_string(i));	
	}
	auto slider1 = sApp->GUIManager()->addWidget(new SSlider);
	auto slider2 = sApp->GUIManager()->addWidget(new SSlider);
	slider1->move(300,100/*300, 100*/)->setObjectname("slider1");
	slider2->move(350,100/*300, 200*/)->setObjectname("slider2");
	slider1->setOrientation(SGUI::Vertical);
	slider2->setOrientation(SGUI::Vertical);
	
	slider1->valueChanged.connect(Bind_Args_1(slider2, &SSlider::setValue));

	auto imgBtn = sApp->GUIManager()
		->addWidget(new SButton("assets/images/play.png", "assets/images/play-hover.png"))
		->move(0,400)
		->setFixedSize(32,32);

	//auto sbtn = sApp->GUIManager()->addWidget(new SSwitchButton);
	//sbtn->move(100, 350);
	//sbtn->setText(u8"声音");
	////sbtn->onSwitchChanged = [=](bool state) {slider1->setVisible(state); };
	////sbtn->onSwitchChanged = std::bind(&SSlider::setVisible,slider1,std::placeholders::_1);
	//sbtn->switchChanged.connect(Bind_Args_1(slider1, &SSlider::setVisible));

	auto btn = sApp->GUIManager()->addWidget(new SButton("assets/images/play.png", "assets/images/play-hover.png"));
	btn->move(100, 100);
	//btn->setText("play");

	auto edit = (SLineEdit*)sApp->GUIManager()->addWidget(new SLineEdit)->move(200,50);
	slider2->valueChanged.connect([=](int val)
		{
			SDL_Log("val:%d val*2 = %d", val, val * 2);
			edit->setWidth(val*2);
		});
	auto btn1 = (SButton*)sApp->GUIManager()->widget("button1");
	btn1->released.connect([=] 
		{
		});
	return isRunning;
}

void SGameApp::clean()
{
	delete STTF_FontManager::instance();
	delete STextureManager::instance();
	delete SGUIManager::instance();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	SDL_Log("game clean!\n");
}

void SGameApp::update()
{

}


void SGameApp::render()
{
	sApp->GUIManager()->draw();
}

void SGameApp::handleEvents()
{
	if (SDL_PollEvent(&events))
	{
		if (events.type == SDL_QUIT)
		{
			quit();
		}
		sApp->GUIManager()->event(&events);
	}
}



