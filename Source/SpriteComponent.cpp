//-----------------------------------------------------------------------+
// SpriteComponentクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "SpriteComponent.h"
#include "GameMain.h"
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"
#include "Renderer.h"


// コンストラクタ
SpriteComponent::SpriteComponent(Actor * in_owner, int in_drawOrder)
	:Component(in_owner)
	,m_texture(nullptr)
	,m_drawOrder(in_drawOrder)
	,m_textureWidth(0)
	,m_textureHeight(0)
	,m_visible(true)
{

	GAME_INSTANCE.GetRenderer()->AddSprite(this);

}


// デストラクタ
SpriteComponent::~SpriteComponent()
{

	GAME_INSTANCE.GetRenderer()->RemoveSprite(this);

}


// 描画処理
void SpriteComponent::Draw(Shader * in_shader)
{

	if (m_texture)
	{
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(m_textureWidth),
			static_cast<float>(m_textureHeight),
			1.0f);

		Matrix4 world = scaleMat * m_owner->GetWorldTransform();

		//
		//

		// WorldTransformをセット
		in_shader->SetMatrixUniform("uWorldTransform", world);
		// テクスチャをアクティブ
		m_texture->SetActive();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

}


// テクスチャのセット
void SpriteComponent::SetTexture(Texture * in_texture)
{

	m_texture = in_texture;
	// 横幅と縦幅のセット
	m_textureWidth = in_texture->GetWidth();
	m_textureHeight = in_texture->GetHeight();

}
