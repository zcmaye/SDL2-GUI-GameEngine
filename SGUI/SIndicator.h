#ifndef _SINDICATOR_H_
#define _SINDICATOR_H_
#include "SAbstractSlider.h"

class SIndicator : public SWidget
{
public:
	enum ShapeType { Rect, Elipse };
	SIndicator(SWidget*parent ,SIndicator::ShapeType type = Rect);
	inline bool isDown()const { return m_isDown; };
protected:
	void paintEvent()override;
	void mousePressEvent(SDL_MouseButtonEvent* ev)override;
	void mouseReleaseEvent(SDL_MouseButtonEvent* ev)override;
private:
	ShapeType m_type;
	SWidget* m_parent;
	bool m_isDown = false;
};

#endif