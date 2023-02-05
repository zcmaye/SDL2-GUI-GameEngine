#ifndef _SABSTRACTSLIDER_H_
#define _SABSTRACTSLIDER_H_
#include "SWidget.h"

class SIndicator;
class SAbstractSlider : public SWidget
{
public:
	SAbstractSlider();
	~SAbstractSlider();

	void setOrientation(SGUI::Orientation);
	SGUI::Orientation orientation() const;

	void setRange(int min, int max);
	void setValue(int val);
	void setMaximum(int max);
	void setMinimum(int min);
	int maximum() const;
	int minimum() const;
	int value()const;

	//void setGrooveWidth(int w);
	//inline void setIndicatorWidth(int w) { m_indicator->setWidth(w); };
	//inline void setIndicatorHeight(int h) { m_indicator->setHeight(h); };
	//inline void setIndicatorSize(int w, int h) { m_indicator->setFixedSize(w, h); };

public:
	SSignal<void(int)> valueChanged;
	SSignal<void(int)> sliderMoved;
	SSignal<void(int,int)> rangeChanged;

protected:
	void paintEvent() override;
	void mouseMoveEvent(SDL_MouseMotionEvent* ev)override;
	void mouseWheelEvent(SDL_MouseWheelEvent* ev)override;
	void showEvent(SDL_WindowEvent* ev);
	void moveEvent(SDL_Event* ev)override;

	SIndicator* m_indicator;
	void updateIndicator();

	void updateRatio();	//值和凹槽长度比率
	//int m_grooveWidth;
	//int m_grooveHeight;
private:
	SGUI::Orientation m_orientation;
	int m_max = 100;	
	int m_min = 0;		//最小值
	int m_value = 0;	//当前值
	int m_step = 1;		//步长
	int m_distance = 0;		//滑块距离左边/上边的距离
	double m_ratio = 0.0;			//值和凹槽长度比率
};

#endif