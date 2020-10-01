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
	:m_title(nullptr)
	,m_backGround(nullptr)
	,m_titlePos(0.0f, 300.0f)
	,m_bgPos(100.0f, 80.0f)
	,m_nextButtonPos(0.0f, 250.0f)
	,m_state(ACTIVE)
{

	// ゲームクラスのUIスタックに自身を追加
	GAME_INSTANCE.AddUI(this);

	// フォントのロード
	m_font = GAME_INSTANCE.GetFont("Data/Fonts/The 2K12.ttf");

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

void UIScreen::Close()
{
	m_state = CLOSE;
}

void UIScreen::SetTitle(const std::string & in_text, const Vector3 & in_color, int in_pointSize)
{
	// Clear out previous title texture if it exists
	if (m_title)
	{
		m_title->Delete();
		delete m_title;
		m_title = nullptr;
	}
	m_title = m_font->RenderText(in_text, in_color, in_pointSize);
}

void UIScreen::AddButton(const std::string & in_name, std::function<void()> onClick)
{
}

void UIScreen::DrawTexture(Shader * in_shader, Texture * in_texture, const Vector2 & offset, float scale)
{
	// Scale the quad by the width/height of texture
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(in_texture->GetWidth()) * scale,
		static_cast<float>(in_texture->GetHeight()) * scale,
		1.0f);
	// Translate to position on screen
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(offset.x, offset.y, 0.0f));
	// Set world transform
	Matrix4 world = scaleMat * transMat;
	in_shader->SetMatrixUniform("uWorldTransform", world);
	// Set current texture
	in_texture->SetActive();
	// Draw quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void UIScreen::SetRelativeMouseMode(bool in_relative)
{
}
