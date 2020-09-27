//-----------------------------------------------------------------------+
// �`���[�g���A���pUI(�Q�[���V�[��).
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "TutorialUI.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "InputController.h"


// �R���X�g���N�^
TutorialUI::TutorialUI()
{

	// �R���g���[���p�e�N�X�`��
	m_controllerTex = RENDERER->GetTexture("Data/Interface/Tutorial/Game/TutorialUI_GameScene_Controller.png");
	// �L�[�{�[�h�p�e�N�X�`��
	m_keyboardTex = RENDERER->GetTexture("Data/Interface/Tutorial/Game/TutorialUI_GameScene_Keyboard.png");

	// �^�X�N�p�e�N�X�`��
	m_taskTex = RENDERER->GetTexture("Data/Interface/Tutorial/Game/Task.png");

}


// �f�X�g���N�^
TutorialUI::~TutorialUI()
{
	// �����Ȃ�
}


// �X�V����
void TutorialUI::Update(float in_deltaTime)
{
	// �����Ȃ�
}


// �`�揈��
void TutorialUI::Draw(Shader * in_shader)
{

	// �R���g���[���ڑ����Ƃ����łȂ����Ƃŕ`���ς���
	if (m_controllerTex && CONTROLLER_INSTANCE.IsAvailable() == true)
	{
		DrawTexture(in_shader, m_controllerTex, Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.7f), 0.75f);
	}
	else if (m_keyboardTex)
	{
		DrawTexture(in_shader, m_keyboardTex, Vector2(0.0f, -RENDERER->GetScreenHeight() / 2.7f), 0.75f);
	}

	// �ڕW�̕\��
	DrawTexture(in_shader, m_taskTex, Vector2(-RENDERER->GetScreenWidth() / 2.7f, RENDERER->GetScreenHeight() / 2.3f), 0.75f);

}
