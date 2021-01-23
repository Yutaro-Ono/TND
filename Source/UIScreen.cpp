//-----------------------------------------------------------------------+
// �C���^�[�t�F�[�X���N���X
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "UIScreen.h"
#include "Texture.h"
#include "Shader.h"
#include "GameMain.h"
#include "GameConfig.h"
#include "Renderer.h"
#include "Font.h"

// �R���X�g���N�^
UIScreen::UIScreen()
	:m_state(ACTIVE)
{

	// �Q�[���N���X��UI�X�^�b�N�Ɏ��g��ǉ�
	GAME_INSTANCE.AddUI(this);

	// �t�H���g�̃��[�h
	m_font = GAME_INSTANCE.GetFont(GAME_INSTANCE.GetFontPath());

	// �X�N���[���T�C�Y�i�[
	SCREEN_W = GAME_CONFIG->GetScreenWidth();
	SCREEN_H = GAME_CONFIG->GetScreenHeight();
	SCREEN_HALF_W = GAME_CONFIG->GetScreenWidth() / 2;
	SCREEN_HALF_H = GAME_CONFIG->GetScreenHeight() / 2;
}


// �f�X�g���N�^
UIScreen::~UIScreen()
{

}

void UIScreen::Update(float in_deltaTime)
{
}

void UIScreen::Draw(Shader * in_shader)
{

}

void UIScreen::ProcessInput()
{
}

void UIScreen::HandleKeyPress(int in_key)
{
}

// UI����� (���̌�GameMain���ŃX�^�b�N����폜�����)
void UIScreen::Close()
{
	m_state = CLOSE;
}

// �^�C�g���̃Z�b�g
// �w�肵����������w�肵���J���[�ŁA�w�肵���T�C�Y�ɍ쐬����
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


// �w�肵���e�N�X�`������ʏ�̃I�t�Z�b�g�ɕ`�悷��
// ���[���h�s����쐬���A�V�F�[�_�֑��M
void UIScreen::DrawTexture(Shader * in_shader, Texture * in_texture, const Vector2 & offset, float scale)
{
	// �e�N�X�`���̏c���T�C�Y�ɃX�P�[�����|�����킹���l���X�P�[���s��Ƃ��Ē�`
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(in_texture->GetWidth()) * scale,
		static_cast<float>(in_texture->GetHeight()) * scale,
		1.0f);
	// �w�肵����ʈʒu�ւ̃X�N���[���ϊ��s����쐬
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(offset.x, offset.y, 0.0f));
	// �X�P�[���ƕϊ��s������[���h�s��֕ϊ�
	Matrix4 world = scaleMat * transMat;
	
	// �V�F�[�_�Ƀ��[���h�ϊ��s��𑗐M
	in_shader->SetMatrixUniform("u_worldTransform", world);
	in_shader->SetInt("u_texture", 0);
	//in_texture->SetActive();

	// �e�N�X�`�����A�N�e�B�u��
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, in_texture->GetTextureID());

	// �`�悷��
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void UIScreen::SetRelativeMouseMode(bool in_relative)
{
}
