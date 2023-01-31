#include"SGameApp.h"
#include"STextureManager.h"
#include"STTF_FontManager.h"

#include"SButton.h"
#include"SSwitchButton.h"
#include"SSlider.h"

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
		SDL_SetRenderDrawColor(renderer, 240, 240, 240, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		update();
		render();
		handleEvents();

		SDL_RenderPresent(renderer);
		SDL_Log("times %u", SDL_GetTicks() - startTime);

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
		   ->setObjectname("button");

		btn->onClicked = [=](){SDL_Log("button clicked %d", i);};
		//btn->onPressed = [=](){SDL_Log("button released %d", i);};
		btn->onPressed = BIND_LAMBDA({SDL_Log("button released %d",i); });
		btn->setText(u8"顽石" + std::to_string(i));	
	}
	auto slider1 = sApp->GUIManager()->addWidget(new SSlider);
	auto slider2 = sApp->GUIManager()->addWidget(new SSlider);
	slider1->move(300, 100)->setObjectname("slider1");
	slider2->move(300, 200)->setObjectname("slider2");
	
	//slider1->onValueChanged = [=](int value)
	//	{
	//		slider2->setValue(value);
	//	};
	slider1->onValueChanged = BIND_1(slider2, &SSlider::setValue);

	auto imgBtn = sApp->GUIManager()
		->addWidget(new SButton("assets/images/play.png", "assets/images/play-hover.png"))
		->move(0,400)
		->setFixedSize(32,32);

	auto sbtn = sApp->GUIManager()->addWidget(new SSwitchButton);
	sbtn->move(100, 350);
	sbtn->setText(u8"声音");
	//sbtn->onSwitchChanged = [=](bool state) {slider1->setVisible(state); };
	//sbtn->onSwitchChanged = std::bind(&SSlider::setVisible,slider1,std::placeholders::_1);
	sbtn->onSwitchChanged = BIND_1(slider1 ,&SSlider::setVisible);


	auto btn = sApp->GUIManager()->addWidget(new SButton("assets/images/play.png", "assets/images/play-hover.png"));
	btn->move(100, 100);
	//btn->setText("play");

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

void SDL_RenderDrawEllipse(SDL_Renderer* render, SDL_Rect* rect)
{
	//半轴长
	int aHalf = rect->w / 2;
	int bHalf = rect->h / 2;

	int x, y;
	//求出圆上每个坐标点
	for (float angle = 0; angle < 360; angle += 0.1)
	{
		x = (rect->x + aHalf) + aHalf * SDL_cos(angle);
		y = (rect->y + bHalf) + bHalf * SDL_sin(angle);
		SDL_RenderDrawPoint(render, x, y);
	}
}

void drawArc(SDL_Renderer* renderer, SDL_Rect* rect, double startAngle, double endAngle);
void drawRountRect(SDL_Renderer* renderer, SDL_Rect* rect, int hRadius, int vRadius);

void drawArc(SDL_Renderer* renderer, SDL_Rect* rect, double startAngle, double endAngle)
{
	//半轴长
	int aHalf = rect->w / 2;
	int bHalf = rect->h / 2;

	int x, y;
	//求出圆上每个坐标点
	for (float angle = startAngle; angle < endAngle; angle += 0.2)
	{
		auto radian = 0.01745 * angle;
		x = (rect->x + aHalf) + aHalf * SDL_cos(-radian);
		y = (rect->y + bHalf) + bHalf * SDL_sin(-radian);
		SDL_RenderDrawPoint(renderer, x, y);
	}
}
void drawArc(SDL_Renderer* renderer, int x,int y,int hRadius,int vRadius, double startAngle, double endAngle)
{
	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//SDL_Rect r = { x,y,hRadius*2,vRadius*2 };
	//SDL_RenderFillRect(renderer, &r);
	//半轴长
	int aHalf = hRadius;
	int bHalf = vRadius;
	//SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	int tx,ty;
	//求出圆上每个坐标点
	for (float angle = startAngle; angle < endAngle; angle += 0.2)
	{
		auto radian = 0.01745 * angle;
		tx = (x+aHalf) + aHalf * SDL_cos(-radian);
		ty = (y+bHalf) + bHalf * SDL_sin(-radian);
		SDL_RenderDrawPoint(renderer, tx, ty);
	}
}

void drawRountRect(SDL_Renderer* renderer, SDL_Rect* rect, int hRadius, int vRadius)
{
	SDL_RenderDrawLine(renderer, rect->x + hRadius, rect->y, rect->x + rect->w - hRadius, rect->y);
	SDL_RenderDrawLine(renderer, rect->x+rect->w, rect->y + vRadius, rect->x+rect->w, rect->y + rect->h - vRadius);
	SDL_RenderDrawLine(renderer, rect->x + hRadius, rect->y+rect->h, rect->x + rect->w - hRadius, rect->y + rect->h);
	SDL_RenderDrawLine(renderer, rect->x, rect->y + vRadius, rect->x, rect->y + rect->h - vRadius);


	drawArc(renderer, rect->x,rect->y,hRadius,vRadius,90,180);
	drawArc(renderer, rect->x + rect->w - hRadius*2,rect->y, hRadius, vRadius,0,90);
	drawArc(renderer, rect->x + rect->w-hRadius*2,rect->y+rect->h-vRadius*2, hRadius, vRadius,270,360);
	drawArc(renderer, rect->x,rect->y + rect->h-vRadius*2, hRadius, vRadius,180,270);
}

void drawFillRountRect(SDL_Renderer* renderer, SDL_Rect* rect, int hRadius, int vRadius)
{
	SDL_Rect r = *rect;
	while (r.w >= 0 && r.h >= 0)
	{
		drawRountRect(renderer, &r, hRadius--, vRadius--);
		++r.x;
		++r.y;
		r.w -= 2;
		r.h -= 2;		
	}
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



