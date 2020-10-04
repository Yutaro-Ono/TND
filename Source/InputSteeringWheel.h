//-----------------------------------------------------------------------+
// �X�e�A�����O�R���g���[���[(���n���R��)�N���X
// 
// �����F�V���O���g��
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
//#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
//#define INITGUID
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <map>

// ������̑���
enum CALIBRATE_CONTROL
{
	WHEEL = 0,
	ACCELERATOR = 1,
	BRAKE = 2,
	CLUTCH = 3,

	X_AXIS = 1,
	Y_AXIS = 2,
	Z_AXIS = 3,
	SLIDER_0 = 4,
	SLIDER_1 = 5,
	X_ROT = 6,
	Y_ROT = 7,
	Z_ROT = 8,
};

class InputSteeringWheel
{
public:

	// �C���X�^���X����
	static InputSteeringWheel& SteeringControllerInstance()
	{
		static InputSteeringWheel ControllerInstance;
		return ControllerInstance;
	}

	~InputSteeringWheel();                 // �f�X�g���N�^

	bool Initialize();                     // ����������

	void Delete();                         // �폜����

	void Update();                         // �X�V����


	// �w�肵���R���g���[�����ʂ̓��͒l���擾
	long GetControlValue(int in_device, int in_control);

	void RenderDebugImGui();

private:

	InputSteeringWheel();                                 // �R���X�g���N�^
	
	LPDIRECTINPUT8 m_dinput;                              // DirectInput
	LPDIRECTINPUTDEVICE8 m_dinputDevice;                  // dinput�f�o�C�X

	DIDEVCAPS m_devCaps;                                  // �ڑ��f�o�C�X�̋@�\

	HINSTANCE m_hInstance;                                // �C���X�^���X�n���h��

	DIJOYSTATE2 m_joyPadData;                             // �W���C�p�b�h�f�[�^�\����

	bool m_isWheelActive;                                 // �n���R�����ڑ�����Ă��邩


	float m_accelParam;                                   // �A�N�Z�����͒l(float)
	float m_brakeParam;                                   // �u���[�L���͒l(float)

	static const int MODEL_NAME;                          // �g�p����X�e�A�����O�R���g���[���̃��f���ԍ�

};

// �C���X�^���X�̎擾
#define STEERING_CONTROLLER_INSTANCE InputSteeringWheel::SteeringControllerInstance()