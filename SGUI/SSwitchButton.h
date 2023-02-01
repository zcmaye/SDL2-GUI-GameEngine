#ifndef SSWITCHBUTTON_H_
#define SSWITCHBUTTON_H_
#include "SAbstractButton.h"

class SIndicator;
class SSwitchButton : public SAbstractButton
{
public:
	SSwitchButton();
	void setOffTexture(const std::string& offTex, const std::string& offHoverTex = std::string());
	void setOnTexture(const std::string& onTex, const std::string& onHoverTex = std::string());
public:
	std::function<void(bool state)> onSwitchChanged;
protected:
	void paintEvent()override;
	void mousePressEvent(SDL_MouseButtonEvent* ev)override;
	void showEvent(SDL_WindowEvent* ev)override;
private:
	std::string m_offImgFile;
	std::string m_offHoverImgFile;
	std::string m_onImgFile;
	std::string m_onHoverImgFile;
	bool m_state;

	SIndicator* m_indicator;
};

#endif // !SBUTTON_H_
