#ifndef _SSLIDER_H_
#define _SSLIDER_H_
#include "SAbstractSlider.h"

class SSlider : public SAbstractSlider
{
public:
	SSlider();
protected:
	void paintEvent()override;
};

#endif