//-----------------------------------------------------------------------+
// �Q�[���V�[���N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

#include "SceneBase.h"
#include <unordered_map>


class GameScene : public SceneBase
{


public:


	enum SceneState
	{
		STATE_FADE_IN,
		STATE_FADE_OUT,
		STATE_FREE,
		STATE_START,
		STATE_IN_GAME,
		STATE_FINISH
	};



	GameScene(int in_stageNum);                               // �R���X�g���N�^
	~GameScene();                                             // �f�X�g���N�^


	void Initialize() override;                               // ����������

	SceneBase* Update() override;                             // �X�V����

	void Draw() override;                                     // �`�揈��

	SceneState GetState() { return m_state; }                 // �X�e�[�g�̃Q�b�^�[

	void Wait(int in_millisec);                               // �w�肵���b�����E�F�C�g

	void Loading();                                           // ���[�h��ʉ��o


private:

	int m_stageNum;                                           // �X�e�[�W�ԍ�

	class GameWorld* m_world;                                 // �Q�[�����[���h�N���X

	class RuleTime* m_time;
	class RuleScore* m_score;                                 // �X�R�A

	int m_prevScore;                                          // �X�V�O�̃X�R�A
	int m_nowScore;                                           // ���t���[���̃X�R�A

	float m_bestSpeed;                                        // �v���C���[���B�������ō����x

	std::unordered_map<std::string, std::string> m_sound;     // ���ʉ�

	SceneState m_state;                                       // �X�e�[�g

	float m_cameraMove;                                       // �J�����̈ړ���

};