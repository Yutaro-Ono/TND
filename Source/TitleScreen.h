//-----------------------------------------------------------------------+
// タイトルシーンUIクラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

// インクルードファイル
#include "UIScreen.h"
#include <vector>

class TitleScreen : public UIScreen
{


public:

	TitleScreen(class TitleScene* in_target);
	~TitleScreen();

	void Update(float in_deltaTime) override;

	void Draw(class Shader* in_shader) override;


private:

	
	

	class Texture* m_logo;                        // タイトルロゴ用 テクスチャ
	class Texture* m_cursol;                      // カーソルテクスチャ
	class Texture* m_cross;                       // 十字キーテクスチャ

	class Texture* m_anyKey[2];                      // "PUSH ANY KEY" 用フォントテクスチャ
	class Texture* m_startButton[2];                 // "GAME START" 用フォントテクスチャ
	class Texture* m_quitButton[2];                  // "QUIT TO DESKTOP"用 フォントテクスチャ

	std::vector<class Texture*> m_stage;         // ステージ用フォントテクスチャ

	class TitleScene* m_targetScene;

	class Texture* m_xpadTutorialTex;          // コントローラー用チュートリアルUI
	class Texture* m_keyboardTutorialTex;      // キーボード用チュートリアルUI

	Vector2 m_logoPos;
	Vector2 m_logoJpPos;
	Vector2 m_anyKeyPos;
	Vector2 m_startPos;
	Vector2 m_quitPos;

	float m_counter;
};