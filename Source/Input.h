//-----------------------------------------------------------------------+
// ���̓N���X(�V���O���g��)
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "GameMain.h"

class Input
{
public:

	// �L�[�������
	enum KEY_STATE
	{
		KEY_OFF = 0,                        // �L�[��������Ă��Ȃ�
		KEY_PUSH_DOWN,                      // �L�[���������ꂽ
		KEY_PRESSED,                        // �L�[�������ꑱ���Ă���
		KEY_PULL_UP                         // ������Ă����̂������ꂽ
	};

	// �C���X�^���X
	static Input& InputInstance()
	{
		static Input InputInstance;
		return InputInstance;
	}

	~Input() {};

	void Update();                                     // ���̓V�X�e���X�V

	char GetInput(Uint8 SDL_SCANCODE_key);             // �w�肵���L�[�̓��͏�Ԃ̎擾

	bool IsKeyPressed(Uint8 SDL_SCANCODE_key);         // �w�肵���L�[�͉����ꑱ���Ă����Ԃ����擾
	bool IsKeyPullUp(Uint8 SDL_SCANCODE_key);          // �w�肵���L�[�̃{�^���������ꂽ���H
	bool IsKeyPushDown(Uint8 SDL_SCANCODE_key);        // �w�肵���L�[�͉����ꑱ���Ă��邩
	bool IsKeyOff(Uint8 SDL_SCANCODE_key);             // ����L�[�͉�����Ă��Ȃ���

private:

	Input();                                           // �V���O���g��

	char m_key[256];                                   // �S�ẴL�[��Ԏ擾�p

	char m_keyState[2][256];                           // �L�[��� [���� / �ߋ�] [�S�L�[�R�[�h]

	int m_nowKey, m_prevKey;                           // ���݂̃t���[���A�ߋ��̃t���[��

};

#define INPUT_INSTANCE Input::InputInstance()