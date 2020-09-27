//-----------------------------------------------------------------------+
// カウントダウンUIクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"

class CountDownUI : public UIScreen
{


public:


	CountDownUI(class RuleTime* in_time);                   // コンストラクタ
	~CountDownUI();                                         // デストラクタ

	void Update(float in_deltaTime) override;               // 更新処理

	void Draw(class Shader* in_shader) override;            // 描画処理

	void SetTimeAdding() { m_isAdding = true; }             // ともし火取得時、時間が追加されたかどうかのセッター


private:

	class RuleTime* m_time;                                 // タイマークラス取得用

	class Texture* m_timeTex;                               // 制限時間表示用(本体)
	class Texture* m_timeTexShadow;                         // 制限時間表示用(影)

	class Texture* m_timeLimitTex[2];

	class Texture* m_addTimeTex;
	class Texture* m_addTimeTexShadow;

	class Texture* m_skipXpad;
	class Texture* m_skipKey;

	Vector2 m_timeTexPos;

	bool m_isAdding;

	float m_timer;

	int m_fontSize;

};