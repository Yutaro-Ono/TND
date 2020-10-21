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

	ScoreUI();                   // コンストラクタ
	~ScoreUI();                                          // デストラクタ

	void Update(float in_deltaTime) override;            // 更新処理

	void Draw(class Shader* in_shader) override;                  // 描画処理

	//-------------------------------------------------+
	// Setter/Getter
	//-------------------------------------------------+
	void AddScore(int in_score) { m_nowScore += in_score; }     // スコアのセッター

private:

	class Texture* m_scoreTex;
	class Texture* m_scoreTexShadow;


	Vector2 m_scoreTexPos;

	int m_prevScore;     // 更新前のスコア

	int m_nowScore;      // 毎フレームのスコア

	int m_fontSize;


};