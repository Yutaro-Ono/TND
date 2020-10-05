#include "InputController.h"
#include <limits.h>
#include <iostream>

// XINPUT�̂������l
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE 7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD 30
// ���͂̍ő�l
const float InputController::PAD_MAX_VALUE = 32767.0f;

// �R���X�g���N�^
InputController::InputController()
	:m_gameController(nullptr)
	,m_whichController(-1)
{
	// �{�^���Ǝ���0�ŏ�����
	// memset -> �z���0�ŏ������ł���(�킴�킴for���ŏ���������K�v���Ȃ�)
	memset(m_buttonStates, 0, sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);
	memset(m_prevButtonStates, 0, sizeof(Uint8) * SDL_CONTROLLER_BUTTON_MAX);
	memset(m_axisValues, 0, sizeof(float) * SDL_CONTROLLER_AXIS_MAX);
	printf("InputSystem < CONTROLLER_XINPUT > : Create Success.\n");
}

// �f�X�g���N�^
InputController::~InputController()
{
}

// ����������
bool InputController::Initialize()
{
	// �p�b�h�̐ݒ�t�@�C�� gamecontrollerdb.txt �̓ǂݍ��݂Ɩ�肪�������̃`�F�b�N
    // �l�X�ȃR���g���[���[�̓��̓^�C�v�ɑΉ�
	int iNumOfControllers = SDL_GameControllerAddMappingsFromFile("Data/Strings/gamecontrollerdb.txt");
	if (iNumOfControllers == -1)
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "Error loading database [%s]", SDL_GetError());
		return false;
	}

	// �R���g���[���J��
	m_gameController = SDL_GameControllerOpen(0);

	if (SDL_IsGameController(0) && m_gameController)
	{
		std::cout << SDL_GameControllerMapping(m_gameController) << std::endl;
	}

	// �R���g���[���C�x���g�̖����i�����炩��t���[�����ɏ�Ԃ��擾���邽�߁j
	SDL_GameControllerEventState(SDL_IGNORE);
	return true;
}

// �X�V����
void InputController::Update()
{
	// �R���g���[���������ꍇ�� early exit����
	if (m_gameController == NULL)
	{
		return;
	}

	// �O�̃t���[���̃R���g���[���̏�Ԃ��R�s�[����
	memcpy(&m_prevButtonStates, &m_buttonStates, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);

	// �R���g���[���̏�Ԃ��X�V����
	SDL_GameControllerUpdate();

	// ���݂̃R���g���[���̃{�^����Ԃ�ۑ�
	for (int b = 0; b < SDL_CONTROLLER_BUTTON_MAX; ++b)
	{
		m_buttonStates[b] = SDL_GameControllerGetButton(m_gameController, (SDL_GameControllerButton)b);
	}

	// ���݂̃R���g���[���̎�����ۑ�
	for (int a = 0; a < SDL_CONTROLLER_AXIS_MAX; ++a)
	{
		m_axisValues[a] = SDL_GameControllerGetAxis(m_gameController, (SDL_GameControllerAxis)a);
	}

	// ���X�e�B�b�N���͂��x�N�g��������
	m_axisL.x = (float)m_axisValues[SDL_CONTROLLER_AXIS_LEFTX];
	m_axisL.y = (float)m_axisValues[SDL_CONTROLLER_AXIS_LEFTY];

	// �E�X�e�B�b�N���͂��x�N�g��������
	m_axisR.x = (float)m_axisValues[SDL_CONTROLLER_AXIS_RIGHTX];
	m_axisR.y = (float)m_axisValues[SDL_CONTROLLER_AXIS_RIGHTY];

	// �������l�ȉ���؂�
	m_axisL.x = (fabs(m_axisL.x) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? 0.0f : m_axisL.x / PAD_MAX_VALUE;
	m_axisL.y = (fabs(m_axisL.y) < (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? 0.0f : m_axisL.y / PAD_MAX_VALUE;
	m_axisR.x = (fabs(m_axisR.x) < (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? 0.0f : m_axisR.x / PAD_MAX_VALUE;
	m_axisR.y = (fabs(m_axisR.y) < (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? 0.0f : m_axisR.y / PAD_MAX_VALUE;
}

// �������
void InputController::Delete()
{
	if (!m_gameController)
	{
		SDL_GameControllerClose(m_gameController);
	}

	m_gameController = nullptr;
}


void InputController::ReceiveEvent(const SDL_Event & event)
{
	switch (event.type)
	{

	// �R���g���[���[�ǉ�
	case SDL_CONTROLLERDEVICEADDED:
	{
		// �R���g���[���[�����t�����Ă��Ȃ�����
		if (m_gameController != NULL)
		{
			// �R���g���[���[�̃I�[�v��
			m_whichController = event.cdevice.which;
			m_gameController = SDL_GameControllerOpen(m_whichController);
			// �O�Ɏ��t����ꂽ�R���g���[���̖���������邽�߃�������0�ɃZ�b�g�B
			memset(m_buttonStates, 0, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);
			memset(m_prevButtonStates, 0, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);
			memset(m_axisValues, 0, sizeof(float)*SDL_CONTROLLER_AXIS_MAX);
		}
		break;
	}

	// �R���g���[���[���O���C�x���g
	case SDL_CONTROLLERDEVICEREMOVED:
	{
		// ���݂̃R���g���[�������O���ꂽ���̃`�F�b�N
		if (m_whichController == event.cdevice.which)
		{
			m_whichController = -1;
			m_gameController = NULL;
		}
		break;
	}
	
	}

}


bool InputController::IsTriggered(const SDL_GameControllerButton in_button) const
{
	return (m_buttonStates[in_button] == 1 && m_prevButtonStates[in_button] == 0);
}

bool InputController::IsPressed(const SDL_GameControllerButton in_button) const
{
	return (m_buttonStates[in_button] == 1);
}

bool InputController::IsReleased(const SDL_GameControllerButton in_button) const
{
	return (m_buttonStates[in_button] == 0 && m_prevButtonStates[in_button] == 1);
}

float InputController::GetAxisValue(const SDL_GameControllerAxis in_axis) const
{

	return m_axisValues[in_axis] / PAD_MAX_VALUE;
}
