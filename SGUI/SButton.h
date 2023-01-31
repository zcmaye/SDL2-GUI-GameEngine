#ifndef SBUTTON_H_
#define SBUTTON_H_
#include"SAbstractButton.h"

class SButton : public SAbstractButton
{
public:
	SButton(const std::string& text);
	SButton(const std::string& normalTexture, const std::string& hoverTexture);
	SButton();

protected:
	void paintEvent()override;
private:
	std::string m_normalTexture;
	std::string m_hoverTexture;
};

#endif // !SBUTTON_H_
