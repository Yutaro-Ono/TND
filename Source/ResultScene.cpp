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
#include "Skydome.h"
#include "LevelObject.h"
#include "FlameStatue.h"
#include "AngelStatue.h"
#include "FlameActor.h"
#include "Camera.h"
#include "ResultScreen.h"
#include "LoadScreen.h"
#include "RecordScore.h"


// �R���X�g���N�^
ResultScene::ResultScene(int in_score, float in_bestSpeed)
	:m_state(STATE_SCORE_DISPLAY)
	,m_score(in_score)
	,m_bestSpeed(in_bestSpeed)
{
	//���C�e�B���O
	GAME_INSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.5f, 0.56f, 0.6f));
	DirectionalLight& dir = GAME_INSTANCE.GetRenderer()->GetDirectionalLight();
	dir.m_direction = Vector3(0.0f, 0.0f, 1.0f);
	dir.m_direction.Normalize();
	dir.m_diffuseColor = Vector3(1.0f, 1.0f, 1.0f);
	dir.m_specColor = Vector3(0.8f, 0.8f, 0.8f);
}


// �f�X�g���N�^
ResultScene::~ResultScene()
{

}


// ����������
void ResultScene::Initialize()
{
	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
		RENDERER->Draw();
	}

	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}



	// �X�J�C�h�[���̐����Ɛݒ�
	m_skydome = new Skydome();
	m_skydome->SetMesh(RENDERER->GetMesh("Data/Meshes/FC/Skydome/Skydome_Sunny.gpmesh"));
	m_skydome->SetPosition(Vector3(0.0f, 0.0f, -20.0f));
	m_skydome->SetScale(0.45f);

	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}

	// �C��̐���
	m_flameStatue = new FlameStatue();
	m_flameStatue->SetMesh(RENDERER->GetMesh("Data/Meshes/FC/Objects/FlameStatue/FlameStatue.gpmesh"));
	m_flameStatue->SetPosition(Vector3(0.0f, 0.0f, -120.0f));

	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}

	// �����N������
	for (int i = 0; i < 3; i++)
	{
		m_monkStatue[i] = new AngelStatue();
		m_monkStatue[i]->SetMesh(RENDERER->GetMesh("Data/Meshes/FC/Objects/Monk_Statue/SM_MonkStatue_01.gpmesh"));

		if (i == 0)
		{

			m_monkStatue[i]->SetPosition(Vector3(0.0f, 100.0f, -120.0f));
			m_monkStatue[i]->SetRotation(Quaternion(Vector3::UnitZ, 1.5f));

		}
		else if(i == 1)
		{
			m_monkStatue[i]->SetPosition(Vector3(-150.0f, 75.0f, -120.0f));
			m_monkStatue[i]->SetRotation(Quaternion(Vector3::UnitZ, 2.5f));
		}
		else if(i == 2)
		{
			m_monkStatue[i]->SetPosition(Vector3(150.0f, 75.0f, -120.0f));
			m_monkStatue[i]->SetRotation(Quaternion(Vector3::UnitZ, 0.5f));
		}

		// ���[�h��ʏ���
		{
			GAME_INSTANCE.GetLoadScreen()->AddGauge();
			RENDERER->Draw();
		}
	}

	//------------------------------------------------------------//
	// �I�u�W�F�N�g�̐���
	//-----------------------------------------------------------//
	// ��n
	LevelObject* m_platform = new LevelObject(RENDERER->GetMesh("Data/Meshes/FC/Objects/Result_Building/SM_Platform_A.gpmesh"));
	m_platform->SetPosition(Vector3(0.0f, 0.0f, -220.0f));
	m_platform->SetScale(0.6f);

	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}

	// ��
	LevelObject* m_pillar = new LevelObject(RENDERER->GetMesh("Data/Meshes/FC/Objects/Result_Building/SM_MapRoom_Pillars.gpmesh"));
	m_pillar->SetPosition(Vector3(0.0f, 0.0f, -220.0f));
	m_pillar->SetScale(0.6f);

	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}


	// �Ƃ����т̐���
	m_flame = new FlameActor();
	m_flame->SetPosition(Vector3::Zero);
	// �Ƃ����т̓X�R�A�ɉ������T�C�Y�ɃZ�b�g
	m_flame->SetScale(m_score / 100.0f);
	m_flame->SetFlameLife(0.2f);

	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}



	// �J�����̐����Ə�����
	Camera* camera = new Camera(m_flame);
	camera->Initialize(Vector3(0.0f, -250.0f, 0.0f), m_flame->GetPosition(), Vector3::Zero);

	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}


	// ���y
	m_sound["BGM"] = "Data/Music/BGM/FC/ResultScene/townofdeath.wav";
	m_sound["Enter"] = "Data/Music/SE/FC/System/Enter/switch01.wav";
	AUDIO->GetMusic(m_sound["BGM"]);
	AUDIO->GetSound(m_sound["Enter"]);


	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}





	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
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

	switch (m_state)
	{

	case STATE_SCORE_DISPLAY:

		if (m_resultUI->GetDispScore() == m_score)
		{
			m_state = STATE_ONE_MORE;
		}


		break;

	case STATE_ONE_MORE:

		// BGM�Đ�
		if (AUDIO->IsPlayingMusic() == false)
		{
			AUDIO->PlayMusic(m_sound["BGM"]);
		}


		// ���L�[�E���R���g���[���[��DPAD�E�Ŏ��̍��ڂ�
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RIGHT) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
		{
			// ���艹�Đ�
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			m_state = STATE_GAME_QUIT;
		}

		// SPACE��ENTER�AA�{�^�����������烊�v���C (���̃V�[����Ԃ�)
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// ���艹�Đ�
			AUDIO->PlaySoundTND(m_sound["Enter"]);

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

			// �^�C�g���V�[���̃|�C���^��Ԃ�
			return new TitleScene;

			break;
		}

		break;

	case STATE_GAME_QUIT:

		// ���L�[�����R���g���[���[��DPAD���őO�̍��ڂ�
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LEFT) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_LEFT))
		{
			// ���艹�Đ�
			AUDIO->PlaySoundTND(m_sound["Enter"]);

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
