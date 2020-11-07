//-----------------------------------------------------------------------+
// �^�C�g���V�[��UI�N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

// �C���N���[�h�t�@�C��
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

	
	

	class Texture* m_logo;                        // �^�C�g�����S�p �e�N�X�`��
	class Texture* m_cursol;                      // �J�[�\���e�N�X�`��
	class Texture* m_cross;                       // �\���L�[�e�N�X�`��

	class Texture* m_anyKey[2];                      // "PUSH ANY KEY" �p�t�H���g�e�N�X�`��
	class Texture* m_startButton[2];                 // "GAME START" �p�t�H���g�e�N�X�`��
	class Texture* m_quitButton[2];                  // "QUIT TO DESKTOP"�p �t�H���g�e�N�X�`��

	std::vector<class Texture*> m_stage;         // �X�e�[�W�p�t�H���g�e�N�X�`��

	class TitleScene* m_targetScene;

	class Texture* m_xpadTutorialTex;          // �R���g���[���[�p�`���[�g���A��UI
	class Texture* m_keyboardTutorialTex;      // �L�[�{�[�h�p�`���[�g���A��UI

	Vector2 m_logoPos;
	Vector2 m_logoJpPos;
	Vector2 m_anyKeyPos;
	Vector2 m_startPos;
	Vector2 m_quitPos;

	float m_counter;
};