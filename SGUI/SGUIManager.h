#ifndef SGUIMANAGER_H_
#define SGUIMANAGER_H_
#include"Configer.h"

class SGUIManager
{
public:
	static SGUIManager* instance();
	~SGUIManager();
	template<typename Ty>
	Ty* addWidget(Ty* w);							//添加控件
	SWidget* widget(const std::string& objname);	//获取控件
	SWidgetList widgets(const std::string& objname);	//获取控件
	void removeWidget(SWidget* w);					//移除控件
	void removeWidget(const std::string& objname);

	void clearAllFocus();	//清楚所有控件的焦点

	void event(SDL_Event* ev);
	void draw();
private:
	SGUIManager();
	SWidgetList m_widgets;
};

template<typename Ty>
Ty* SGUIManager::addWidget(Ty* w)
{
	if (w)
	{
		m_widgets.push_back(w);
	}
	return w;
}

#endif	//SGUIMANAGER_H_
