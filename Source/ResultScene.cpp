//-----------------------------------------------------------------------+
// ���U���g�V�[���N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "ResultScene.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Input.h"
#include "InputController.h"
#include "TitleScene.h"
#include "Actor.h"
#include "LevelObject.h"
#include "Camera.h"
#include "ResultScreen.h"
#include "LoadScreen.h"
#include "RecordScore.h"
#include "Environment.h"
#include "ResultCar.h"
#include "BridgeObject.h"
#include "RenderBloom.h"

// �R���X�g���N�^
ResultScene::ResultScene(int in_score, float in_bestSpeed)
	:m_state(STATE_WHITE_IN)
	,m_score(in_score)
	,m_environment(nullptr)
	,m_bestSpeed(in_bestSpeed)
{

}

// �f�X�g���N�^
ResultScene::~ResultScene()
{
	GAME_INSTANCE.DeadAllActor();
	// �S�Ă�UI��Close�ɐݒ�
	for (auto iter : GAME_INSTANCE.GetUIStack())
	{
		iter->Close();
	}
}


// ����������
void ResultScene::Initialize()
{
	// ������
	m_environment = new Environment(Environment::GAME_TIME::MORNING, Vector3(0.0f, -65.0f, 0.0f));

	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
	}

	// �v���C���[
	m_car = new ResultCar();
	m_car->SetPosition(Vector3(6500.0f, -65.0f, 0.0f));
	m_car->SetScale(0.4f);


	for (int i = 0; i < 8; i++)
	{
		m_bridge[i] = new BridgeObject(1, Vector3(i * 6500.0f, -2000.0f, 0.0f), false);

		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}

	// ���y
	m_sound["BGM"] = "Data/Music/BGM/TND/Result/cyrf_lu_metropolis.wav";
	m_sound["Enter"] = "Data/Music/SE/TND/System/Enter/decide13.wav";
	m_sound["Select"] = "Data/Music/SE/TND/System/Select/decide14.wav";
	AUDIO->GetMusic(m_sound["BGM"]);
	AUDIO->GetSound(m_sound["Enter"]);
	AUDIO->GetSound(m_sound["Select"]);

	// ���[�h����
	for (int i = 0; i < 64; i++)
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}

	// ���[�h��ʂ̖�����
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();

	// ���R�[�h
	m_record = new RecordScore(m_score);

	// UI
	m_resultUI = new ResultScreen(this, m_score, m_bestSpeed);
	m_resultUI->SetRank(m_record->GetRank());
	// UI�ւ̃X�R�A���̎󂯓n��
	for (int i = 0; i < 10; i++)
	{
		m_resultUI->SetScore(i, m_record->GetRankingScore(i));
	}
}


// �V�[���̍X�V����
SceneBase * ResultScene::Update()
{

	m_environment->SetDirectionalLightPos(m_car->GetPosition());

	switch (m_state)
	{


	//----------------------------------------------------------------------+
    // "WHITE IN"
    //----------------------------------------------------------------------+
	case STATE_WHITE_IN:


		if (RENDERER->GetBloom()->WhiteIn(70.3f, GAME_INSTANCE.GetDeltaTime()))
		{
			m_state = STATE_SCORE_DISPLAY;
		}

		break;


	//----------------------------------------------------------------------+
	// "WHITE OUT"
	//----------------------------------------------------------------------+
	case STATE_WHITE_OUT:

		if (RENDERER->GetBloom()->WhiteOut(70.3f, GAME_INSTANCE.GetDeltaTime()))
		{


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

			// ���R�[�h���폜
			delete m_record;

			// ���̃V�[����Ԃ�
			return new TitleScene;
		}

		break;


	case STATE_SCORE_DISPLAY:

		// BGM�Đ�
		if (AUDIO->IsPlayingMusic() == false)
		{
			AUDIO->PlayMusic(m_sound["BGM"]);
		}

		if (m_resultUI->GetDispScore() == m_score)
		{
			m_state = STATE_ONE_MORE;
		}


		break;

	case STATE_ONE_MORE:



		// ���L�[�E���R���g���[���[��DPAD�E�Ŏ��̍��ڂ�
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RIGHT) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
		{
			// ���艹�Đ�
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = STATE_GAME_QUIT;
		}

		// SPACE��ENTER�AA�{�^�����������烊�v���C (���̃V�[����Ԃ�)
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// ���艹�Đ�
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			AUDIO->FadeOutMusic(3000.0f);

			m_state = STATE_WHITE_OUT;

			break;
		}

		break;

	case STATE_GAME_QUIT:

		// ���L�[�����R���g���[���[��DPAD���őO�̍��ڂ�
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LEFT) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_LEFT))
		{
			// ���艹�Đ�
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = STATE_ONE_MORE;
		}

		// SPACE��ENTER�AA�{�^������������V���b�g�_�E��
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// ���艹�Đ�
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			GAME_INSTANCE.SetShutDown();
			break;
		}

		break;
	}

	return this;
}


// �`�揈��
void ResultScene::Draw()
{
	RENDERER->Draw();
}
