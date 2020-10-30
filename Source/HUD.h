//-----------------------------------------------------------------------+
// HUDクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"
#include <vector>

class HUD : public UIScreen
{


public:

	HUD(class GameWorld* in_world);
	~HUD();

	void Update(float in_deltaTime) override;

	void Draw(class Shader* in_shader);






private:

	class Texture* m_dangerCall;                             // ヘリに見つかった時のDANGER表示
	class Texture* m_speedTexture;

	class Texture* m_impactTexture[2];                       // 集中線

	class GameWorld* m_world;                         // ワールドへのポインタ

	std::vector<class Texture*> m_flameGauge;

	std::vector<Vector2> m_flameGaugePos;
	
	bool m_isImpact;                                  // 集中線を出すか出さないか

	Vector2 m_speedTexPos;

	float m_dispAccel;                                // アクセル表示用

};