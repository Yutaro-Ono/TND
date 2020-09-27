//-----------------------------------------------------------------------+
// �C���^�[�t�F�[�X���N���X
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
	virtual void ProcessInput(const uint8_t* in_keys);
	virtual void HandleKeyPress(int in_key);
	
	// �X�e�[�g(CLOSE�ŏ��������)
	enum UIState
	{
		ACTIVE,
		CLOSE
	};
	
	void Close();
	
	UIState GetState() const { return m_state; }

	void SetTitle(const std::string& in_text,
		const Vector3& in_color = Color::White,
		int in_pointSize = 40);
	
	void AddButton(const std::string& in_name, std::function<void()> onClick);


protected:

	
	void DrawTexture(class Shader* in_shader, class Texture* in_texture,
		const Vector2& offset = Vector2::Zero,
		float scale = 1.0f);
	
	void SetRelativeMouseMode(bool in_relative);

	class Font* m_font;
	class Texture* m_title;
	class Texture* m_backGround;
	class Texture* m_buttonOn;
	class Texture* m_buttonOff;

	
	Vector2 m_titlePos;
	Vector2 m_nextButtonPos;
	Vector2 m_bgPos;

	// State
	UIState m_state;


};