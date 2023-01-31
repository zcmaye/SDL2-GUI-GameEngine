#ifndef _SABSTRACTSLIDER_H_
#define _SABSTRACTSLIDER_H_
#include "SWidget.h"

class SAbstractSlider : public SWidget
{
public:
	SAbstractSlider();

	void setOrientation(SGUI::Orientation);
	SGUI::Orientation orientation() const;

	void setRange(int min, int max);
	void setValue(int val);
	void setMaximum(int max);
	void setMinimum(int min);
	int maximum() const;
	int minimum() const;
	int value()const;

public:
	std::function<void(int)> onValueChanged;

protected:
	void paintEvent() override;
	void mousePressEvent(SDL_MouseButtonEvent* ev)override;
	void mouseReleaseEvent(SDL_MouseButtonEvent* ev)override;
	void mouseMoveEvent(SDL_MouseMotionEvent* ev)override;
	void showEvent(SDL_WindowEvent* ev);

	SDL_Rect m_handleRect;	//手柄矩形区域
	SColor m_handColor = SColor(191, 191, 191);
	bool m_hoverHandle = false;
	void updateRatio();	//值和凹槽长度比率
private:
	bool m_isPressHandle = false;	//是否按下了手柄
	SGUI::Orientation m_orientation;
	int m_max = 100;	
	int m_min = 0;		//最小值
	int m_value = 0;	//当前值
	int m_step = 1;		//步长
	int m_distance = 0;		//滑块距离左边/上边的距离
	double m_ratio = 0.0;			//值和凹槽长度比率


};

#endif