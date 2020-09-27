//-----------------------------------------------------------------------+
// チェインUIクラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"


class ChainUI : public UIScreen
{


public:

	ChainUI(class RuleScore* in_rule);                       // コンストラクタ
	~ChainUI();                                              // デストラクタ

	void Update(float in_deltaTime) override;                // 更新処理

	void Draw(class Shader* in_shader) override;             // 描画処理


private:

	class Texture* m_chain[2];

	class Texture* m_rankTex[2];
	class Texture* m_rankNum[2];
	class Texture* m_chainRank[2];

	class Texture* m_timeGauge;

	class SpriteComponent* m_sprite;

	int m_gaugeWidth;

	class RuleScore* m_rule;

	int m_chainNum;

	int m_rank;


};