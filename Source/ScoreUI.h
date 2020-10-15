//-----------------------------------------------------------------------+
// スコアUIクラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"

class ScoreUI : public UIScreen
{


public:

	ScoreUI(class RuleScore* in_rule);                   // コンストラクタ
	~ScoreUI();                                          // デストラクタ

	void Update(float in_deltaTime) override;            // 更新処理

	void Draw(class Shader* in_shader) override;                  // 描画処理


private:


	class RuleScore* m_ruleScore;

	class Texture* m_scoreTex;
	class Texture* m_scoreTexShadow;
	class Texture* m_scoreRateTex[2];

	Vector2 m_scoreTexPos;

	int m_prevScore;     // 更新前のスコア

	int m_nowScore;      // 毎フレームのスコア

	int m_fontSize;


};