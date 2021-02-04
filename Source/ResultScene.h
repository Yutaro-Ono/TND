//-----------------------------------------------------------------------+
// ���U���g�V�[���N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

// �C���N���[�h�t�@�C��
#include "SceneBase.h"
#include <unordered_map>
#include "AudioManager.h"

class ResultScene : public SceneBase
{


public:
	
	enum ResultState
	{
		STATE_WHITE_OUT,
		STATE_WHITE_IN,
		STATE_SCORE_DISPLAY,
		STATE_ONE_MORE,
		STATE_GAME_QUIT
	};

	ResultScene(int in_score, float in_bestSpeed);                               // �R���X�g���N�^
	~ResultScene();                                          // �f�X�g���N�^

	void Initialize() override;                              // ����������

	SceneBase* Update() override;                            // �V�[���̍X�V����

	void Draw() override;                                    // �`�揈��

	ResultState GetResultState() { return m_state; }


private:

	int m_score;                                              // �X�R�A

	float m_bestSpeed;                                         // �Q�[���V�[�����ɒB�������ō����x

	ResultState m_state;                                      // ���U���g��ʂ̃X�e�[�g

	class Environment* m_environment;                         // ���N���X

	class ResultCar* m_car;
	class BridgeObject* m_bridge[8];

	class ResultScreen* m_resultUI;                           // ���U���g�V�[��UI

	class RecordScore* m_record;

	std::unordered_map<std::string, std::string> m_sound;     // ���y


};