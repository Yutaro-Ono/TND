//-----------------------------------------------------------------------+
// �J�E���g�_�E��UI�N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"

class CountDownUI : public UIScreen
{


public:


	CountDownUI(class RuleTime* in_time);                   // �R���X�g���N�^
	~CountDownUI();                                         // �f�X�g���N�^

	void Update(float in_deltaTime) override;               // �X�V����

	void Draw(class Shader* in_shader) override;            // �`�揈��

	void SetTimeAdding() { m_isAdding = true; }             // �Ƃ����Ύ擾���A���Ԃ��ǉ����ꂽ���ǂ����̃Z�b�^�[


private:

	class RuleTime* m_time;                                 // �^�C�}�[�N���X�擾�p

	class Texture* m_timeTex;                               // �������ԕ\���p(�{��)
	class Texture* m_timeTexShadow;                         // �������ԕ\���p(�e)

	class Texture* m_timeLimitTex[2];

	class Texture* m_addTimeTex;
	class Texture* m_addTimeTexShadow;

	class Texture* m_skipXpad;
	class Texture* m_skipKey;

	Vector2 m_timeTexPos;

	bool m_isAdding;

	float m_timer;

	int m_fontSize;

};