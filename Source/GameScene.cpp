//---------------------------------------------------------------------------+
// �Q�[���V�[���N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//---------------------------------------------------------------------------+
#include "GameScene.h"
#include "GameMain.h"
#include "Input.h"
#include "InputController.h"
#include "AudioManager.h"
#include "Renderer.h"
#include "ParticleManager.h"
#include "LevelManager.h"
#include "SkyBox.h"
#include "RuleScore.h"
#include "RuleTime.h"
#include "HUD.h"
#include "CountDownUI.h"
#include "SpeedMeterUI.h"
#include "ScoreUI.h"
#include "TutorialUI.h"
#include "LoadScreen.h"
#include "ResultScene.h"
#include "Math.h"
#include "PlayerCar.h"
#include "PlayerManager.h"
#include "GameWorld.h"
#include "TitleScene.h"
#include "Canvas.h"
#include "RenderBloom.h"
#include <Windows.h>
#include <iostream>

// �R���X�g���N�^
GameScene::GameScene(int in_stageNum)
	:m_time(nullptr)
	,m_stageNum(in_stageNum)
	,m_state(STATE_FADE_IN)
	,m_score(nullptr)
	,m_prevScore(0)
	,m_nowScore(0)
	,m_bestSpeed(0.0f)
	,m_cameraMove(-10.0f)
{


}


// �f�X�g���N�^
GameScene::~GameScene()
{
	delete m_world;
	// �S�ẴA�N�^�[���폜
	GAME_INSTANCE.DeadAllActor();
	// �S�Ă�UI��Close�ɐݒ�
	for (auto iter : GAME_INSTANCE.GetUIStack())
	{
		iter->Close();
	}
	// �p�[�e�B�N����S�č폜
	RENDERER->GetParticleManager()->AllDeadParticle();
	// ���y���~
	AUDIO->StopMusic();
}


// ����������
// ��}���`�X���b�h���Ń��[�h�����𓱓����Ă��邽�ߎ��F��������
void GameScene::Initialize()
{
	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
		GAME_INSTANCE.GetLoadScreen()->SetIsGame();
	}

	//---------------------------------------------------------------------------------------------+
    // �A�N�^�[
    //---------------------------------------------------------------------------------------------+
	// ���[���h����
	m_world = new GameWorld();

	//---------------------------------------------------------------------------------------------+
    // ���y
    //---------------------------------------------------------------------------------------------+
	m_sound["BGM"] = "Data/Music/BGM/TND/Game/cyrf_cloudbase (mp3cut.net).wav";
	AUDIO->GetMusic(m_sound["BGM"]);

	// ���[�h��ʂ̖�����
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();

	// �^�C�����[��
	RuleTime* ruleTime = new RuleTime(this);
	m_time = ruleTime;
	// �������Ԃ̃Z�b�g
	m_time->SetStartTime();

	CountDownUI* countUI = new CountDownUI(ruleTime);

	// BGM�Đ��J�n
	AUDIO->PlayMusic(m_sound["BGM"]);
}

// �X�V����
SceneBase * GameScene::Update()
{

	m_world->Update(GAME_INSTANCE.GetDeltaTime());

	// �V�[���̏�Ԃɂ�菈���𕪊�
	switch (m_state)
	{

	//----------------------------------------------------------------------+
    // "FADE IN"
    //----------------------------------------------------------------------+
	case STATE_FADE_IN:


		if (RENDERER->GetBloom()->FadeIn(0.8f, GAME_INSTANCE.GetDeltaTime()))
		{
			m_state = STATE_FREE;
		}

		break;


	//----------------------------------------------------------------------+
	// "FADE OUT"
	//----------------------------------------------------------------------+
	case STATE_FADE_OUT:

		if (RENDERER->GetBloom()->WhiteOut(70.3f, GAME_INSTANCE.GetDeltaTime()))
		{
			// ���̃V�[����Ԃ�
			return new ResultScene(m_world->GetCanvas()->GetScoreUI()->GetScore(), m_bestSpeed);
		}

		break;


	case STATE_FREE:   // �t���[���[�h

		// �J�E���g�_�E���J�n��
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_SPACE) || CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_A))
		{
			// �������Ԃ̃Z�b�g
			m_time->SetStartTime();

			// �J�E���g�_�E����
			m_state = STATE_START;
		}

		// �������Ԃ̃Z�b�g
		m_time->SetStartTime();

		// �J�E���g�_�E����
		m_state = STATE_START;

		// �f�o�b�O�p�^�C�g���V�[��
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_P) || CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_A))
		{
			//return new TitleScene();
		}



		break;

	case STATE_START:   // �X�^�[�g������

		// �v���C���[���|�[�Y����
		//m_player->SetState(m_player->STATE_PAUSED);

		// �J�������v���C���[�Ɋ񂹂�
		if (m_time->GetNowCount() < 3)
		{
			//m_camera->SetBestCameraDist();
		}
		else
		{
			//m_cameraMove -= 3.0f;
			//m_camera->SetHorizonDist(m_cameraMove);
		}

		// �J�E���g�I���ŃA�N�e�B�u��
		if (m_time->GetNowCount() < 0)
		{
			//m_player->SetState(m_player->STATE_ACTIVE);

			// �������Ԃ̃Z�b�g
			m_time->SetLimitTime();

			m_state = STATE_IN_GAME;
		}

		break;


	case STATE_IN_GAME:

		// ���݂̃X�R�A���L�^
		//m_nowScore = m_score->GetScore();
		//
		//// �X�R�A���ϓ�������J�E���g�A�b�v
		//if (m_nowScore != m_prevScore)
		//{
		//	m_time->AddCount(1.0f);
		//	m_prevScore = m_nowScore;
		//}

		// �v���C���[�̑��x���Ď����A�ō����x���X�V������L�^����������
		//if (m_bestSpeed < m_player->GetAccel())
		//{
		//	m_bestSpeed = m_player->GetAccel();
		//}

		// �X�e�[�g�ύX
		if (m_time->GetCountState() == m_time->STATE_FINISH)
		{
			m_state = STATE_FINISH;
		}

		break;


	case STATE_FINISH:

		// �������ԃN���X����Q�[���I���̍��}���o����I��
		if (m_time->GetGameEnd() == true)
		{
			//m_player->AllStopSound();

			// �}�b�vHUD�̃I�t


			m_state = STATE_FADE_OUT;
		}

		break;

	default:

		break;

	}

	// �Q�[���t�B�j�b�V�����A�v���C���[���X�g�b�v���J�����𗣂�
	if (m_time->GetCountState() == m_time->STATE_FINISH)
	{
		//m_player->SetState(m_player->STATE_PAUSED);

		//m_camera->SetHorizonDist(m_camera->GetPosition().x * 0.2f);
		//m_camera->SetVerticalDist(500.0f);
	}

	return this;
}

// �`�揈��
void GameScene::Draw()
{
	// �`��
	RENDERER->Draw();
}

// �E�F�C�g����
void GameScene::Wait(int in_millisec)
{
	Sleep(in_millisec);
}

// ���[�f�B���O����
void GameScene::Loading()
{
	GAME_INSTANCE.GetLoadScreen()->AddGauge();
	RENDERER->Draw();
}
