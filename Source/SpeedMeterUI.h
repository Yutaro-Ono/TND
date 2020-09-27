//-----------------------------------------------------------------------+
// スピードメーターUIクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"

class SpeedMeterUI : public UIScreen
{


public:

	SpeedMeterUI(class Player* in_target);
	~SpeedMeterUI();

	void Update(float in_deltaTime) override;

	void Draw(class Shader* in_shader) override;


private:

	class Player* m_player;

	class Texture* m_speedTex;          // スピードの数値

	class Texture* m_gaugeTex[20];          // スピードメーターゲージ
	class Texture* m_gaugeShadowTex;    // スピードメーターゲージの影
	int m_currentSpeedRate;                 // 現在の速度と最大速度との比率(ゲージの描画に用いる)


	Vector2 m_speedTexPos;

	int m_fontSize;

};