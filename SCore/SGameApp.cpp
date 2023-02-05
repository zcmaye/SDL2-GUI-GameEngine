#include"SGameApp.h"
#include"STextureManager.h"
#include"STTF_FontManager.h"

#include"SButton.h"
#include"SSwitchButton.h"
#include"SSlider.h"
#include"SLineEdit.h"

SGameApp* gInstance = nullptr;

SGameApp::SGameApp(int argc, char* argv[])
{
	gInstance = this;
	if (!init("SDL2", 640, 480))
	{
		SDL_Log("Create an application failed!");
	}
}

SGameApp::~SGameApp()
{
	gInstance = nullptr;
}

SGameApp* SGameApp::instance()
{
	if (!gInstance)
	{
		SDL_Log("Create an application instance first!");
		SDL_assert(gInstance != nullptr);
	}
	return gInstance;
}



bool SGameApp::init(const std::string& title, int w, int h)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)
		&& TTF_Init() == 0)
	{
		SDL_Log("game init!\n");
		//创建窗口
		window_ = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,w, h, SDL_WINDOW_SHOWN);
		if (window_)
		{
			SDL_Log("window created!\n");
		}

		//创建渲染器
		renderer_ = SDL_CreateRenderer(window_, -1, 0);
		if (renderer_)
		{
			SDL_Log("renderer created!\n");
		}
		isRunning_ = true;
	}
	else
	{
		isRunning_ = false;
	}
	
	for (int i = 0; i < 5; i++)
	{
		auto btn = sApp->GUIManager()->addWidget(new SButton);
		btn->move(0, i * (35 + 5))
		   ->setObjectname("button"+std::to_string(i));

		btn->clicked.connect([=]() 
			{
				
			});
		btn->pressed.connect([=] {SDL_Log("button released %d", i); });
		btn->setText(u8"顽石" + std::to_string(i));	
	}
	auto slider1 = sApp->GUIManager()->addWidget(new SSlider);
	auto slider2 = sApp->GUIManager()->addWidget(new SSlider);
	slider1->move(300,100/*300, 100*/)->setObjectname("slider1");
	slider2->move(300,120/*300, 200*/)->setObjectname("slider2");
	//slider1->setOrientation(SGUI::Vertical);
	slider2->setOrientation(SGUI::Vertical);
	slider2->setRange(10, 300);
	
	//slider1->valueChanged.connect(Bind_Args_1(slider2, &SSlider::setValue));

	auto imgBtn = sApp->GUIManager()
		->addWidget(new SButton("assets/images/play.png", "assets/images/play-hover.png"))
		->move(0,400)
		->setFixedSize(32,32);

	auto sbtn = sApp->GUIManager()->addWidget(new SSwitchButton);
	sbtn->move(100, 350);
	sbtn->setText(u8"声音");
	//sbtn->onSwitchChanged = [=](bool state) {slider1->setVisible(state); };
	//sbtn->onSwitchChanged = std::bind(&SSlider::setVisible,slider1,std::placeholders::_1);
	//sbtn->switchChanged.connect(Bind_Args_1(slider1, &SSlider::setVisible));
	sbtn->switchChanged.connect(Bind_Args_1(slider1, &SSlider::setVisible));

	auto btn = sApp->GUIManager()->addWidget(new SButton("assets/images/play.png", "assets/images/play-hover.png"));
	btn->move(100, 100);
	//btn->setText("play");

	auto edit = (SLineEdit*)sApp->GUIManager()->addWidget(new SLineEdit)->move(200,50);
	slider2->setValue(edit->width());
	slider2->valueChanged.connect([=](int val)
		{
			SDL_Log("val:%d val*2 = %d", val, val * 2);
			edit->setWidth(val);
		});
	auto btn1 = (SButton*)sApp->GUIManager()->widget("button1");
	btn1->released.connect([=] 
		{
		});
	return isRunning_;
}

void SGameApp::clean()
{
	delete STTF_FontManager::instance();
	delete STextureManager::instance();
	delete SGUIManager::instance();

	SDL_DestroyWindow(window_);
	SDL_DestroyRenderer(renderer_);
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
	if (SDL_PollEvent(&events_))
	{
		if (events_.type == SDL_QUIT)
		{
			quit();
		}
		sApp->GUIManager()->event(&events_);
	}
}


int SGameApp::exec()
{
	Uint32 startTime = 0;
	while (running())
	{
		startTime = SDL_GetTicks();
		SDL_SetRenderDrawColor(renderer_, 250, 250, 250, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer_);

		update();
		render();
		handleEvents();

		SDL_RenderPresent(renderer_);
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
