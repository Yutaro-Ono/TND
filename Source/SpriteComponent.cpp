//-----------------------------------------------------------------------+
// SpriteComponent�N���X
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


// �R���X�g���N�^
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


// �f�X�g���N�^
SpriteComponent::~SpriteComponent()
{

	GAME_INSTANCE.GetRenderer()->RemoveSprite(this);

}


// �`�揈��
void SpriteComponent::Draw(Shader * in_shader)
{

	if (m_texture)
	{
		// �X�P�[���s��̍쐬
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(m_textureWidth),
			static_cast<float>(m_textureHeight),
			1.0f);
		// �X�P�[�� * ��ʏ�̍��W �s�������
		Matrix4 world = scaleMat * m_owner->GetWorldTransform();

		// WorldTransform���Z�b�g
		in_shader->SetMatrixUniform("u_worldTransform", world);
		in_shader->SetInt("u_texture", 0);
		// �e�N�X�`�����A�N�e�B�u
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureID());
		//RENDERER->SetActiveSpriteVAO();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

}


// �e�N�X�`���̃Z�b�g
void SpriteComponent::SetTexture(Texture * in_texture)
{

	m_texture = in_texture;
	// �����Əc���̃Z�b�g
	m_textureWidth = in_texture->GetWidth();
	m_textureHeight = in_texture->GetHeight();

}
