#include"SGUIManager.h"
#include"SWidget.h"
SGUIManager::SGUIManager(){}
SGUIManager* SGUIManager::instance()
{
	static SGUIManager* ins = nullptr;
	if (!ins)
	{
		ins = new SGUIManager;
	}
	return ins;
}
SGUIManager::~SGUIManager()
{
	for (auto w : m_widgets)
	{
		delete w;
	}
}
				
SWidget* SGUIManager::widget(const std::string& objname)
{
	auto it = std::find_if(m_widgets.begin(), m_widgets.end(), [=](SWidget* w)
		{
			return w->objectname() == objname;
		});
	if (it != m_widgets.end())
	{
		return *it;
	}
	return nullptr;
}
SWidgetList SGUIManager::widgets(const std::string& objname)
{
	SWidgetList ws;
	for (auto w : m_widgets)
	{
		if (w->objectname() == objname)
		{
			ws.push_back(w);
		}
	}
	return ws;
}

void SGUIManager::removeWidget(SWidget* w)
{
	auto it = std::remove(m_widgets.begin(), m_widgets.end(), w);
	if(it!=m_widgets.end())
		m_widgets.erase(it, m_widgets.end());
}					
void SGUIManager::removeWidget(const std::string& objname)
{
	auto it = std::find_if(m_widgets.begin(), m_widgets.end(), [=](SWidget*w)
		{
			return w->objectname() == objname;
		});
	if (it != m_widgets.end())
	{
		delete *it;
		m_widgets.erase(it);
	}
}
void SGUIManager::clearAllFocus()
{
	for (auto w : m_widgets)
	{
		w->clearFocus();
	}
}
void SGUIManager::event(SDL_Event* ev)
{
	for (auto w : m_widgets)
	{
		w->event(ev);
	}
}

void SGUIManager::draw()
{
	for (int i = 0; i < m_widgets.size(); i++)
	{
		if(m_widgets[i]->visible())
			m_widgets[i]->update();
	}
}
