#include "TutorialScene.h"
#include "GameMain.h"
#include "Renderer.h"
#include "RenderBloom.h"
#include "PlayerManager.h"
#include "BridgeObject.h"
#include "LoadScreen.h"
#include "GameScene.h"
#include "Environment.h"
#include "ParticleManager.h"

TutorialScene::TutorialScene()
	:m_state(STATE_FADE_IN)
	,m_player(nullptr)
{

}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Initialize()
{

	// ������
	m_environment = new Environment(Environment::GAME_TIME::NIGHT, Vector3(0.0f, 0.0f, 0.0f));

	// ���[�h��ʏ���
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
	}

	// �v���C���[(�l����)����
	m_player = new PlayerManager();
	m_player->SetPosition(Vector3(0.0f, -1000.0f, 10.0f));
	m_player->SetScale(0.3f);
	// ���[�h����
	GAME_INSTANCE.GetLoadScreen()->AddGauge();

	// ���I�u�W�F�N�g�̐���
	for (int i = 0; i < 12; i++)
	{
		m_bridge[i] = new BridgeObject(1, Vector3(i * 6515.0f, 0.0f, 0.0f));

		// ���[�h����
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}

	// ���[�h��ʂ̖�����
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();
}

/// <summary>
/// �X�V����
/// </summary>
/// <returns> �ʏ�͎��g�̃|�C���^���A�V�[���J�ڂ̏�����B�������ꍇ�͎��V�[���ւ̃|�C���^��Ԃ� </returns>
SceneBase* TutorialScene::Update()
{

	// ��苗���i�񂾂玟�̃V�[����
	float toNextX = 30000.0f;                  // ���̍��W�𒴂����玟�V�[����
	if (m_player->GetPosition().x > toNextX)
	{
		m_state = STATE_FADE_OUT;
	}


	switch (m_state)
	{
	case STATE_FADE_IN:

		if (RENDERER->GetBloom()->FadeIn(0.5f, GAME_INSTANCE.GetDeltaTime()))
		{
			m_state = STATE_0_MOVING;
		}

		break;

	case STATE_FADE_OUT:

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

			// �p�[�e�B�N����S�č폜
			RENDERER->GetParticleManager()->AllDeadParticle();

			// ���̃V�[����Ԃ�
			return new GameScene(0);
		}

		break;

	case STATE_0_MOVING:

		break;

	case STATE_1_RIDING:

		break;

	case STATE_2_DRIVING:


		break;





	}


	return this;
}

void TutorialScene::Draw()
{
	RENDERER->Draw();
}
