//-----------------------------------------------------------------------+
// �`���[�g���A���pUI(�Q�[���V�[��).
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "UIScreen.h"


class TutorialUI : public UIScreen
{


public:


	TutorialUI();                                     // �R���X�g���N�^
	~TutorialUI();                                    // �f�X�g���N�^


	void Update(float in_deltaTime) override;         // �X�V����

	void Draw(class Shader* in_shader);               // �`�揈��


private:


	class Texture* m_controllerTex;                   // ������� ���R���g���[���p

	class Texture* m_keyboardTex;                     // ������� ���L�[�{�[�h�p

	class Texture* m_taskTex;                         // �^�X�N(�ڕW�̕\��)


};