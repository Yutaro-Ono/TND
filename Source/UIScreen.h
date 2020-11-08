//-----------------------------------------------------------------------+
// �C���^�[�t�F�[�X���N���X(�X�N���[���p��UI)
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

	UIScreen();                                                // �R���X�g���N�^
	virtual ~UIScreen();                                       // �f�X�g���N�^

	virtual void Update(float in_deltaTime);                   // �X�V����
	virtual void Draw(class Shader* in_shader);                // �`�揈��
	virtual void ProcessInput();
	virtual void HandleKeyPress(int in_key);
	
	// �X�e�[�g(CLOSE�ŏ��������)
	enum UIState
	{
		ACTIVE,
		CLOSE
	};
	
	void Close();
	
	UIState GetState() const { return m_state; }

	// �w�肵��������̃e�N�X�`����
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

	// �X�N���[���T�C�Y
	int SCREEN_W;
	int SCREEN_H;
	int SCREEN_HALF_W;
	int SCREEN_HALF_H;
};