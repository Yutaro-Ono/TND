//-----------------------------------------------------------------------+
// �R���g���[���[�N���X(�V���O���g��)
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "GameMain.h"

class InputController
{


public:

	// �C���X�^���X����
	static InputController& ControllerInstance()
	{
		static InputController ControllerInstance;
		return ControllerInstance;
	}

	~InputController();          // �f�X�g���N�^

	bool Initialize();           // ����������
	void Update();               // �X�V����
	void Delete();               // �������

	// �R���g���[���[�𔲂�����������
	void ReceiveEvent(const SDL_Event& event);

	// �{�^���̃X�e�[�g�擾�֐�
	bool IsTriggered(const SDL_GameControllerButton in_button) const;
	bool IsPressed(const SDL_GameControllerButton in_button) const;
	bool IsReleased(const SDL_GameControllerButton in_button) const;

	bool IsAvailable() { return m_gameController != nullptr; }

	// ���݂̎��̒l���擾
	float GetAxisValue(const SDL_GameControllerAxis in_axis) const;
	Vector2& GetLAxisVec() { return m_axisL; }
	Vector2& GetRAxisVec() { return m_axisR; }


private:

	InputController();           // �R���X�g���N�^

	// �R���g���[���[
	SDL_GameController* m_gameController;

	// �R���g���[���[�̃X�e�[�g
	Uint8 m_buttonStates[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 m_prevButtonStates[SDL_CONTROLLER_BUTTON_MAX];
	float m_axisValues[SDL_CONTROLLER_AXIS_MAX];
	
	int m_whichController;

	Vector2 m_axisL;               // ���X�e�B�b�N���͎擾�p
	Vector2 m_axisR;               // �E�X�e�B�b�N���͎擾�p

	static const float PAD_MAX_VALUE;
};

#define CONTROLLER_INSTANCE InputController::ControllerInstance()