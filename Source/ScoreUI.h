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

	ScoreUI(class RuleScore* in_rule);                   // �R���X�g���N�^
	~ScoreUI();                                          // �f�X�g���N�^

	void Update(float in_deltaTime) override;            // �X�V����

	void Draw(class Shader* in_shader) override;                  // �`�揈��


private:


	class RuleScore* m_ruleScore;

	class Texture* m_scoreTex;
	class Texture* m_scoreTexShadow;
	class Texture* m_scoreRateTex[2];

	Vector2 m_scoreTexPos;

	int m_prevScore;     // �X�V�O�̃X�R�A

	int m_nowScore;      // ���t���[���̃X�R�A

	int m_fontSize;


};