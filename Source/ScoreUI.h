//-----------------------------------------------------------------------+
// �X�R�AUI�N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"

class ScoreUI : public UIScreen
{


public:

	ScoreUI();                   // �R���X�g���N�^
	~ScoreUI();                                          // �f�X�g���N�^

	void Update(float in_deltaTime) override;            // �X�V����

	void Draw(class Shader* in_shader) override;                  // �`�揈��

	//-------------------------------------------------+
	// Setter/Getter
	//-------------------------------------------------+
	void AddScore(int in_score) { m_nowScore += in_score; }     // �X�R�A�̃Z�b�^�[

private:

	class Texture* m_scoreTex;
	class Texture* m_scoreTexShadow;


	Vector2 m_scoreTexPos;

	int m_prevScore;     // �X�V�O�̃X�R�A

	int m_nowScore;      // ���t���[���̃X�R�A

	int m_fontSize;


};