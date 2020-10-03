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
#include "PlayerCamera.h"
#include "Skydome.h"
#include "LoadScreen.h"

const int TitleScene::STAGE_ALL_NUM = 1;


// �R���X�g���N�^
TitleScene::TitleScene()
	:m_state(PRESS_ANY_KEY)
	,m_selectedStage(0)
{
	//���C�e�B���O
	GAME_INSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.5f, 0.56f, 0.6f));
	DirectionalLight& dir = GAME_INSTANCE.GetRenderer()->GetDirectionalLight();
	dir.m_position = Vector3(0.0f, 0.0f, 1000.0f);
	dir.m_direction = Vector3(0.0f, 0.0f, 1.0f);
	dir.m_direction.Normalize();
	dir.m_diffuseColor = Vector3(0.0f, 1.0f, 0.6f);
	dir.m_specColor = Vector3(0.0f, 1.0f, 0.0f);
}


// �f�X�g���N�^
TitleScene::~TitleScene()
{
	// �v���C���[���폜
	m_player->SetState(Actor::STATE_DEAD);
	// �X�J�C�h�[�����폜
	//m_skydome->SetState(Actor::STATE_DEAD);

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

	//m_skydome = new Skydome();
	//m_skydome->SetMesh(RENDERER->GetMesh("Data/Meshes/FC/Skydome/Skydome_Sunny.gpmesh"));
	//m_skydome->SetPosition(Vector3::Zero);
	//m_skydome->SetScale(1.0f);

	// �v���C���[
	m_player = new PlayerCar();
	m_player->SetPosition(Vector3::Zero);
	m_player->SetScale(0.5f);
	m_player->SetState(Actor::STATE_PAUSED);

	// �J����
	Camera* camera = new Camera(m_player);
	camera->Initialize(Vector3(0.0f, -250.0f, 0.0f), m_player->GetPosition(), Vector3::Zero);



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


		// �X�e�[�W�Z���N�g��( KEYBOARD : SPACE or ENTER | XINPUT : A )
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// ���艹
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			// �X�e�[�W�Z���N�g��
			m_state = STAGE_SELECT;
		}

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
