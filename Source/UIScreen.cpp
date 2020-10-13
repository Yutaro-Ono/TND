//-----------------------------------------------------------------------+
// インターフェース基底クラス
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "UIScreen.h"
#include "Texture.h"
#include "Shader.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Font.h"


// コンストラクタ
UIScreen::UIScreen()
	:m_state(ACTIVE)
{

	// ゲームクラスのUIスタックに自身を追加
	GAME_INSTANCE.AddUI(this);

	// フォントのロード
	m_font = GAME_INSTANCE.GetFont(GAME_INSTANCE.GetFontPath());

}


// デストラクタ
UIScreen::~UIScreen()
{

}

void UIScreen::Update(float in_deltaTime)
{
}

void UIScreen::Draw(Shader * in_shader)
{

}

void UIScreen::ProcessInput(const uint8_t * in_keys)
{
}

void UIScreen::HandleKeyPress(int in_key)
{
}

// UIを閉じる (この後GameMain側でスタックから削除される)
void UIScreen::Close()
{
	m_state = CLOSE;
}

// タイトルのセット
// 指定した文字列を指定したカラーで、指定したサイズに作成する
void UIScreen::SetTitle(const std::string & in_text, const Vector3 & in_color, int in_pointSize)
{
	
	if (m_texture)
	{
		m_texture->Delete();
		delete m_texture;
		m_texture = nullptr;
	}
	m_texture = m_font->RenderText(in_text, in_color, in_pointSize);
}


// 指定したテクスチャを画面上のオフセットに描画する
// ワールド行列を作成し、シェーダへ送信
void UIScreen::DrawTexture(Shader * in_shader, Texture * in_texture, const Vector2 & offset, float scale)
{
	// テクスチャの縦横サイズにスケールを掛け合わせた値をスケール行列として定義
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(in_texture->GetWidth()) * scale,
		static_cast<float>(in_texture->GetHeight()) * scale,
		1.0f);
	// 指定した画面位置へのスクリーン変換行列を作成
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(offset.x, offset.y, 0.0f));
	// スケールと変換行列をワールド行列へ変換
	Matrix4 world = scaleMat * transMat;
	
	// シェーダにワールド変換行列を送信
	in_shader->SetMatrixUniform("uWorldTransform", world);
	// テクスチャをアクティブ化
	in_texture->SetActive();
	// 描画する
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void UIScreen::SetRelativeMouseMode(bool in_relative)
{
}
