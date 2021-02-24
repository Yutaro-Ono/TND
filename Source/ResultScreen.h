//-----------------------------------------------------------------------+
// リザルトシーン用UIクラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"


class ResultScreen : public UIScreen
{


public:

	ResultScreen(class ResultScene* in_targetScene, int in_score, float in_bestSpeed);
	~ResultScreen();


	void Update(float in_deltaTime) override;

	void Draw(class Shader* in_shader) override;

	int GetDispScore() { return m_dispScore; }


	void SetRank(const int& in_rank) { m_rank = in_rank; }

	void SetScore(const int& in_num, const int& in_score);                    // ランキング情報のセッター


private:

	class ResultScene* m_targetScene;

	class Texture* m_resultTex[2];
	class Texture* m_scoreTex[2];
	class Texture* m_restartButton[2];
	class Texture* m_quitButton[2];

	class Texture* m_rankingScore[10];          // ランキングごとのスコアテクスチャ

	// 操作表示用UI
	class Texture* m_xpadSkipTex;                   // Xboxコントローラ
	class Texture* m_keyboardSkipTex;               // キーボード

	class Texture* m_xpadTutorialTex;               // Xboxコントローラ
	class Texture* m_keyboardTutorialTex;           // キーボード

	int m_score;

	int m_rank;

	int m_dispScore;

	float m_counter;

	int m_fontSize;

	Vector2 m_resultTexPos;
};