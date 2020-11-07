//-----------------------------------------------------------------------+
// �^�C�g���V�[���N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "TitleScene.h"
#include "GameMain.h"
#include "Input.h"
#include "InputController.h"
#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include "TitleScreen.h"
#include "Font.h"
#include "GameScene.h"
#include "PlayerCar.h"
#include "LoadScreen.h"
#include "Camera.h"
#include "SkyBox.h"
#include "Environment.h"
#include "WorldSpaceUI.h"
#include "ClientActor.h"
const int TitleScene::STAGE_ALL_NUM = 1;


// �R���X�g���N�^
TitleScene::TitleScene()
	:m_state(PRESS_ANY_KEY)
	,m_selectedStage(0)
{
}


// �f�X�g���N�^
TitleScene::~TitleScene()
{
	GAME_INSTANCE.DeadAllActor();
	
	delete m_environment;

}


// ����������
void TitleScene::Initialize()
{
	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
	}

	// �^�C�g���pUI
	TitleScreen* hud = new TitleScreen(this);

	// �v���C���[
	m_car = new PlayerCar();
	m_car->SetPosition(Vector3::Zero);
	m_car->SetScale(0.4f);
	m_car->SetState(Actor::STATE_PAUSED);

	m_client = new ClientActor(Vector3::Zero, 5);
	m_client->SetPosition(Vector3(0.0f, -55.0f, 0.0f));
	m_client->SetScale(0.4f);

	// ������
	m_environment = new Environment(Environment::GAME_TIME::NIGHT);
	//m_environment = new Environment(Environment::GAME_TIME::MORNING);

	// ���y
	m_sound["BGM"] = "Data/Music/BGM/FC/TitleScene/neighofwar.wav";
	m_sound["Enter"] = "Data/Music/SE/FC/System/Enter/switch01.wav";

	// SE
	AUDIO->GetSound(m_sound["Enter"]);                                       // ���艹

	// BGM
	//AUDIO->GetMusic(m_sound["BGM"]);
	//AUDIO->PlayMusic(m_sound["BGM"]);


	// ���[�h��ʂ̖�����
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();
}


// �X�V����
SceneBase * TitleScene::Update()
{


	// �^�C�g�����j���[
	switch (m_state)
	{



	//----------------------------------------------------------------------+
	// "PRESS ANY KEY"
	//----------------------------------------------------------------------+
	case PRESS_ANY_KEY:


		// �����ꂩ�̃{�^���Ŏ���
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN)
			|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A)|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B)
			|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_X)|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_Y)
			|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_START))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);
			m_state = GAME_START;
		}

		break;




	//----------------------------------------------------------------------+
	// "GAME START"
	//----------------------------------------------------------------------+
	case GAME_START:

		// TAB��B�{�^�����������烁�j���[��ʖ߂�
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_TAB) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			m_state = PRESS_ANY_KEY;
			break;
		}


		// ���L�[��DPAD���őI��
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			m_state = GAME_QUIT;
		}

		//---------------------------------------------------------------------------+
        // �V�[���J�ڏ���
		//---------------------------------------------------------------------------+
        // SPACE��ENTER�AA�{�^�����������玟�̃V�[����
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);
			// ���y���t�F�[�h�A�E�g
			AUDIO->FadeOutMusic(1.0f);

			// �v���C���[�̃T�E���h���~
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
			return new GameScene(m_selectedStage);
		}

		//// �X�e�[�W�Z���N�g��( KEYBOARD : SPACE or ENTER | XINPUT : A )
		//if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		//{
		//	// ���艹
		//	AUDIO->PlaySoundTND(m_sound["Enter"]);

		//	// �X�e�[�W�Z���N�g��
		//	m_state = STAGE_SELECT;
		//}

		break;




	//-----------------------------------------------------------------------------------+
	// "STAGE SELECT"
	//-----------------------------------------------------------------------------------+
	case STAGE_SELECT:

		//---------------------------------------------------------------------+
		// �u�߂�v����
		//---------------------------------------------------------------------+
		// TAB��B�{�^�����������烁�j���[��ʖ߂�
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_TAB) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			// "GAME_START"��
			m_state = GAME_START;


			break;
		}


		//-----------------------------------------------------------------------+
		// �X�e�[�W���ڂ̈ړ�������
		//-----------------------------------------------------------------------+
		// ���L�[��DPAD���őI��
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_UP))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			// �X�e�[�W1�łȂ���ΑI�𒆂̃X�e�[�W���J�E���g�_�E��
			if (m_selectedStage != 0)
			{
				m_selectedStage--;
			}

			break;
		}

		// ���L�[��DPAD���őI��
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			// �X�e�[�W���̏���łȂ���ΑI�𒆂̃X�e�[�W�ԍ����J�E���g�A�b�v
			if (m_selectedStage < STAGE_ALL_NUM - 1)
			{
				m_selectedStage++;
			}

			break;
		}


		//---------------------------------------------------------------------------+
		// �V�[���J�ڏ���
		//---------------------------------------------------------------------------+
		// SPACE��ENTER�AA�{�^�����������玟�̃V�[����
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);
			// ���y���t�F�[�h�A�E�g
			AUDIO->FadeOutMusic(1.0f);

			// �v���C���[�̃T�E���h���~
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
			return new GameScene(m_selectedStage);
		}

		break;




	//----------------------------------------------------------------------+
	// "QUIT TO DESKTOP"
	//----------------------------------------------------------------------+
	case GAME_QUIT:


		// SPACE��ENTER�AA�{�^������������V���b�g�_�E��
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			GAME_INSTANCE.SetShutDown();
			break;
		}

		// TAB��B�{�^�����������烁�j���[��ʖ߂�
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_TAB) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			m_state = PRESS_ANY_KEY;
			break;
		}


		// ��L�[��DPAD��őI��
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_UP))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			m_state = GAME_START;
		}

		break;




	default:

		break;


	}

	
	return this;

}


// �`�揈��
void TitleScene::Draw()
{
	RENDERER->Draw();
}
