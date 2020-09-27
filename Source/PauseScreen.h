#pragma once
#include "UIScreen.h"

class PauseScreen : public UIScreen
{


public:

	enum PauseMode
	{
		MODE_ON,
		MODE_OFF
	};


	PauseScreen();
	~PauseScreen();


	void Draw(class Shader* in_shader) override;



	// ポーズモード時表示のオンオフ
	void SetModeON() { m_pause = MODE_ON; }
	void SetModeOFF() { m_pause = MODE_OFF; }



private:


	class Texture* m_pauseTex;

	class Texture* m_backGroundTex;


	PauseMode m_pause;

};