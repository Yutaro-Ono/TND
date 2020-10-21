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
#include "ChainUI.h"
#include "TutorialUI.h"
#include "LoadScreen.h"
#include "ResultScene.h"
#include "Math.h"
#include "PlayerCar.h"
#include "PlayerManager.h"
#include "GameWorld.h"

#include <Windows.h>
#include <iostream>

// �R���X�g���N�^
GameScene::GameScene(int in_stageNum)
	:m_time(nullptr)
	,m_stageNum(in_stageNum)
	,m_state(STATE_FREE)
	,m_score(nullptr)
	,m_prevScore(0)
	,m_nowScore(0)
	,m_bestSpeed(0.0f)
	,m_cameraMove(-10.0f)
{

	// �v���W�F�N�V�����s��Z�b�g
	Matrix4 projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(75.0f),
		static_cast<float>(GAME_INSTANCE.GetRenderer()->GetScreenWidth()),
		static_cast<float>(GAME_INSTANCE.GetRenderer()->GetScreenHeight()),
		1.0f, 100000.0f);
	RENDERER->SetProjectionMatrix(projection);
	
	// ���C�e�B���O
	GAME_INSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.0f, 0.04f, 0.15f));
	DirectionalLight& dir = GAME_INSTANCE.GetRenderer()->GetDirectionalLight();
	dir.m_position = Vector3(0.0f, 0.0f, 1000.0f);
	dir.m_direction = Vector3(0.0f, 0.0f, -1.0f);
	dir.m_direction.Normalize();
	dir.m_diffuseColor = Vector3(0.07f, 0.05f, 0.3f);
	dir.m_specColor = Vector3(0.01f, 0.0f, 0.1f);
}


// �f�X�g���N�^
GameScene::~GameScene()
{
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
		RENDERER->Draw();
	}

	//---------------------------------------------------------------------------------------------+
    // �A�N�^�[
    //---------------------------------------------------------------------------------------------+
	// ���[���h����
	m_world = new GameWorld();

	// ���[�h��ʏ���
	Loading();

	// �E�F�C�g
	Wait(300);
	// ���[�h��ʏ���
	Loading();


	// �E�F�C�g
	Wait(300);
	// ���[�h��ʏ���
	Loading();

	// ���[�h��ʏ���
	Loading();
	// �E�F�C�g
	Wait(300);
	// ���[�h��ʏ���
	Loading();
	// �E�F�C�g
	Wait(300);
	// ���[�h��ʏ���
	Loading();
	// �E�F�C�g
	Wait(300);
	// ���[�h��ʏ���
	Loading();

	// SkyBox
	SkyBox* sky = new SkyBox();

	// �E�F�C�g
	Wait(300);
	// ���[�h��ʏ���
	Loading();


	// �E�F�C�g
	Wait(300);
	// ���[�h��ʏ���
	Loading();

	//---------------------------------------------------------------------------------------------+
    // ���y
    //---------------------------------------------------------------------------------------------+
	m_sound["BGM"] = "Data/Music/BGM/FC/GameScene/disent.wav";

	AUDIO->GetMusic(m_sound["BGM"]);

	// �E�F�C�g
	Wait(300);
	// ���[�h��ʏ���
	Loading();

	// �E�F�C�g
	Wait(300);
	// ���[�h��ʏ���
	Loading();

	// �E�F�C�g
	Wait(300);

	// ���[�h��ʂ̖�����
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();

	//---------------------------------------------------------------------------------------------+
	// �C���^�[�t�F�[�X
	//---------------------------------------------------------------------------------------------+
	// HUD(�v���C���[)
	//HUD* hud = new HUD(m_player);
	//SpeedMeterUI* speedUI = new SpeedMeterUI(m_player);

	// �^�C�����[��
	RuleTime* ruleTime = new RuleTime(this);
	m_time = ruleTime;
	CountDownUI* countUI = new CountDownUI(ruleTime);

}

// �X�V����
SceneBase * GameScene::Update()
{

	m_world->Update(GAME_INSTANCE.GetDeltaTime());

	// �V�[���̏�Ԃɂ�菈���𕪊�
	switch (m_state)
	{

	case STATE_FREE:   // �t���[���[�h

		// �J�E���g�_�E���J�n��
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_SPACE) || CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_A))
		{


			// �X�R�A
			//RuleScore* ruleScore = new RuleScore(m_player);
			//m_score = ruleScore;
			//ScoreUI* scoreUI = new ScoreUI(ruleScore);
			// �`�F�C��UI
			//ChainUI* chainUI = new ChainUI(ruleScore);
			// �������Ԃ̃Z�b�g
			m_time->SetStartTime();

			// �J�����ʒu������
			//m_camera->SetBestCameraDist();


			// UI�����̔��]
			//GAME_INSTANCE.SwapPauseUI();

			// BGM�Đ�
			//AUDIO->PlayMusic(m_sound["BGM"]);

			// �J�E���g�_�E����
			m_state = STATE_START;
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
			//m_state = STATE_FINISH;
		}

		break;


	case STATE_FINISH:

		// �������ԃN���X����Q�[���I���̍��}���o����I��
		if (m_time->GetGameEnd() == true)
		{
			//m_player->AllStopSound();

			// �S�Ă�UI��Close�ɐݒ�
			for (auto iter : GAME_INSTANCE.GetUIStack())
			{
				iter->Close();
			}

			// �S�ẴA�N�^�[���폜
			for (auto actor : GAME_INSTANCE.GetActorStack())
			{
				actor->SetState(Actor::STATE_DEAD);
			}

			// ���̃V�[����Ԃ�
			return new ResultScene(m_score->GetScore(), m_bestSpeed);
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
