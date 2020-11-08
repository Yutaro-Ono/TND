//-----------------------------------------------------------------------+
// インターフェース基底クラス(スクリーン用のUI)
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Math.h"
#include <cstdint>
#include <string>
#include <functional>
#include <vector>


class UIScreen
{


public:

	UIScreen();                                                // コンストラクタ
	virtual ~UIScreen();                                       // デストラクタ

	virtual void Update(float in_deltaTime);                   // 更新処理
	virtual void Draw(class Shader* in_shader);                // 描画処理
	virtual void ProcessInput();
	virtual void HandleKeyPress(int in_key);
	
	// ステート(CLOSEで消去される)
	enum UIState
	{
		ACTIVE,
		CLOSE
	};
	
	void Close();
	
	UIState GetState() const { return m_state; }

	// 指定した文字列のテクスチャ化
	void SetTitle(const std::string& in_text, const Vector3& in_color = Color::White,
		          int in_pointSize = 40);



protected:

	
	void DrawTexture(class Shader* in_shader, class Texture* in_texture,
		const Vector2& offset = Vector2::Zero,
		float scale = 1.0f);
	
	void SetRelativeMouseMode(bool in_relative);

	class Font* m_font;
	class Texture* m_texture;

	// State
	UIState m_state;

	// スクリーンサイズ
	int SCREEN_W;
	int SCREEN_H;
	int SCREEN_HALF_W;
	int SCREEN_HALF_H;
};