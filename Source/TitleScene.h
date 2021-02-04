//-----------------------------------------------------------------------+
// �^�C�g���V�[���N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

// �C���N���[�h�t�@�C��
#include "SceneBase.h"
#include <string>
#include <unordered_map>
#include "AudioManager.h"

class TitleScene : public SceneBase
{


public:


	enum TitleState
	{
		FADE_IN,
		FADE_OUT,
		PRESS_ANY_KEY,
		GAME_START,
		STAGE_SELECT,
		GAME_QUIT
	};

	TitleScene();
	~TitleScene();

	void Initialize() override;

	SceneBase* Update() override;
	void Draw() override;

	TitleState GetState() { return m_state; }                           // �^�C�g���V�[���̏�ԃQ�b�^�[


	const int& GetNowSelectStage() const { return m_selectedStage; }    // ���ݑI�𒆂̃X�e�[�W�ԍ��̃Q�b�^�[

	const int& GetAllStageNum() const { return STAGE_ALL_NUM; }         // �X�e�[�W���擾


private:


	//class PlayerCar* m_car;                                             // ��
	class TitleCar* m_car;

	class BridgeObject* m_bridge[8];

	class ClientActor* m_client;

	class Environment* m_environment;                                   // ��

	int m_selectedStage;                                                // �I�𒆂̃X�e�[�W
	std::vector<int> m_stage;                                           // �X�e�[�W�ԍ�

	std::unordered_map<std::string, std::string> m_sound;               // ���ʉ�

	TitleState m_state;                                                 // �^�C�g���V�[���̃X�e�[�g


	static const int STAGE_ALL_NUM;                                     // �S�X�e�[�W


};