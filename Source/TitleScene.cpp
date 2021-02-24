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
#include "PointLight.h"
#include "SpotLight.h"
#include "TitleCar.h"
#include "BridgeObject.h"
#include "RenderBloom.h"
#include "TutorialScene.h"
const int TitleScene::STAGE_ALL_NUM = 1;

// �R���X�g���N�^
TitleScene::TitleScene()
	:m_state(FADE_IN)
	,m_selectedStage(0)
	,m_car(nullptr)
	,m_client(nullptr)
	,m_environment(nullptr)
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
	// ������
	m_environment = new Environment(Environment::GAME_TIME::NIGHT, Vector3(0.0f, -65.0f, 0.0f));
	//DirectionalLight& light = RENDERER->GetDirectionalLight();
	//light.target = Vector3::Zero;
	//light.position = light.target + Vector3(2000.0f, 0.0f, 3000.0f);
	//light.direction = light.target - light.position;
	//light.direction.Normalize();
	//m_environment = new Environment(Environment::GAME_TIME::MORNING);
	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
	}

	// ���[�h����
	GAME_INSTANCE.GetLoadScreen()->AddGauge();

	// ���y
	m_sound["BGM"] = "Data/Music/BGM/TND/Title/cyrf_crashed_dimension (mp3cut.net).wav";
	// ���[�h����
	GAME_INSTANCE.GetLoadScreen()->AddGauge();
	m_sound["Enter"] = "Data/Music/SE/TND/System/Enter/decide13.wav";
	// ���[�h����
	GAME_INSTANCE.GetLoadScreen()->AddGauge();
	m_sound["Select"] = "Data/Music/SE/TND/System/Select/decide14.wav";

	// SE
	AUDIO->GetSound(m_sound["Enter"]);                                       // ���艹
	AUDIO->GetSound(m_sound["Select"]);                                      // �I����

	// ���[�h����
	GAME_INSTANCE.GetLoadScreen()->AddGauge();

	// BGM
	AUDIO->GetMusic(m_sound["BGM"]);

	// ���[�h����
	GAME_INSTANCE.GetLoadScreen()->AddGauge();

	// �v���C���[
	//m_car = new PlayerCar();
	m_car = new TitleCar();
	m_car->SetPosition(Vector3(6500.0f, -65.0f, 0.0f));
	m_car->SetScale(0.4f);
	//m_car->SetState(Actor::STATE_PAUSED);

	GAME_INSTANCE.GetLoadScreen()->AddGauge();

	// ���I�u�W�F�N�g�̐���
	for (int i = 0; i < 8; i++)
	{
		m_bridge[i] = new BridgeObject(1, Vector3(i * 6515.0f, -2000.0f, 0.0f));

		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}

	
	//m_client = new ClientActor(Vector3::Zero, 5);
	//m_client->SetPosition(Vector3(0.0f, -55.0f, 0.0f));
	//m_client->SetScale(0.4f);



	//m_pointLight = new PointLight();
	//m_pointLight->SetPosition(Vector3(0.0f, 0.0f, 40.0f));

	//m_spotLight = new SpotLight(Vector3::Zero, SpotLight::VL_BIG);

	for (int i = 0; i < 61; i++)
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}

	// ���[�h��ʂ̖�����
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();
	// �^�C�g���pUI
	TitleScreen* hud = new TitleScreen(this);

	// BGM�Đ��J�n
	AUDIO->PlayMusic(m_sound["BGM"]);
}

// �X�V����
SceneBase * TitleScene::Update()
{

	// �����X�V
	m_environment->SetDirectionalLightPos(m_car->GetPosition());


	// �^�C�g�����j���[
	switch (m_state)
	{

	//----------------------------------------------------------------------+
    // "FADE IN"
    //----------------------------------------------------------------------+
	case FADE_IN:


		if (RENDERER->GetBloom()->FadeIn(0.5f, GAME_INSTANCE.GetDeltaTime()))
		{
			m_state = PRESS_ANY_KEY;
		}

		break;


	//----------------------------------------------------------------------+
    // "FADE OUT"
    //----------------------------------------------------------------------+
	case FADE_OUT:

		if (RENDERER->GetBloom()->FadeOut(0.7f, GAME_INSTANCE.GetDeltaTime()))
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

			// ���̃V�[����Ԃ�
			return new GameScene(m_selectedStage);
			//return new TutorialScene();
		}
		
		break;


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
			AUDIO->PlaySoundTND(m_sound["Select"]);
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
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = PRESS_ANY_KEY;
			break;
		}

		// ���L�[��DPAD���őI��
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		{
			// �I����
			AUDIO->PlaySoundTND(m_sound["Select"]);

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
			AUDIO->FadeOutMusic(3000.0f);
			
			

			// �t�F�[�h�A�E�g�����Ɉڍs
			m_state = FADE_OUT;
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
			AUDIO->PlaySoundTND(m_sound["Select"]);

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
			// �I����
			AUDIO->PlaySoundTND(m_sound["Select"]);

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
			// �I����
			AUDIO->PlaySoundTND(m_sound["Select"]);

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

			// �t�F�[�h�A�E�g�����Ɉڍs
			m_state = FADE_OUT;
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
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = PRESS_ANY_KEY;
			break;
		}


		// ��L�[��DPAD��őI��
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_UP))
		{
			// �I����
			AUDIO->PlaySoundTND(m_sound["Select"]);

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
