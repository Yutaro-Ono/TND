#include "Environment.h"
#include "GameMain.h"
#include "SkyBox.h"
#include "CubeMapComponent.h"
#include "GameWorld.h"
#include "PlayerManager.h"

static Vector3 playerPos = Vector3::Zero;
static const Vector3 ADJUST_POS = Vector3(2000.0f, 0.0f, 3000.0f);

Environment::Environment(GAME_TIME in_gameTime)
	:m_gameTime(in_gameTime)
	, m_world(nullptr)
{
	// �X�J�C�{�b�N�X����
	m_morningBox = new SkyBox(this, GAME_TIME::MORNING);
	m_eveningBox = new SkyBox(this, GAME_TIME::EVENING);
	m_nightBox = new SkyBox(this, GAME_TIME::NIGHT);
	// �ݒ肳�ꂽ���ԑт��ƂɃX�J�C�{�b�N�X���Z�b�g
	SetSkyBox(m_gameTime);
	SetDirectionalLight(m_gameTime);
}


Environment::Environment(GameWorld* in_world, GAME_TIME in_gameTime)
	:m_gameTime(in_gameTime)
	,m_world(in_world)
{
	// �X�J�C�{�b�N�X����
	m_morningBox = new SkyBox(this, GAME_TIME::MORNING);
	m_eveningBox = new SkyBox(this, GAME_TIME::EVENING);
	m_nightBox = new SkyBox(this, GAME_TIME::NIGHT);

	playerPos = m_world->GetPlayer()->GetPosition();
	
	// �ݒ肳�ꂽ���ԑт��ƂɃX�J�C�{�b�N�X���Z�b�g
	SetSkyBox(m_gameTime);

	// �����Z�b�g
	SetDirectionalLight(in_gameTime);

}

Environment::~Environment()
{

}

void Environment::Update()
{


	Vector3 framePlayerPos = m_world->GetPlayer()->GetPosition();

	// �e�̂������}���邽��4000�ȏ�̈ړ��ŕ��s���C�g�̈ʒu���v���C���[�ɃX�i�b�v����
	if (framePlayerPos.x >= playerPos.x + 2800 || framePlayerPos.x <= playerPos.x - 2800 ||
		framePlayerPos.y >= playerPos.y + 2800 || framePlayerPos.y <= playerPos.y - 2800)
	{
		// �v���C���[�̈ʒu�ɉ����ĕ��s���C�g�̍��W�E�������œK��
		RENDERER->GetDirectionalLight().position = Vector3((int)framePlayerPos.x, (int)framePlayerPos.y, 0.0f);
		RENDERER->GetDirectionalLight().position.x += (int)ADJUST_POS.x;
		RENDERER->GetDirectionalLight().position.y += (int)ADJUST_POS.y;
		RENDERER->GetDirectionalLight().position.z += (int)ADJUST_POS.z;

		RENDERER->GetDirectionalLight().target = framePlayerPos;

		RENDERER->GetDirectionalLight().direction = RENDERER->GetDirectionalLight().target - RENDERER->GetDirectionalLight().position;
		RENDERER->GetDirectionalLight().direction.Normalize();

		playerPos = framePlayerPos;
	}

}

void Environment::SetSkyBox(GAME_TIME in_gameTime)
{
	if (in_gameTime == GAME_TIME::MORNING || in_gameTime == GAME_TIME::AFTERNOON)
	{
		// �����_���[�ɓ����̃X�J�C�{�b�N�X���Z�b�g
		RENDERER->SetActiveSkyBox(m_morningBox->GetCubeMapComp());
		m_morningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (in_gameTime == GAME_TIME::EVENING)
	{
		// �����_���[�ɗ[���̃X�J�C�{�b�N�X���Z�b�g
		RENDERER->SetActiveSkyBox(m_eveningBox->GetCubeMapComp());
		m_eveningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (in_gameTime == GAME_TIME::NIGHT)
	{
		// �����_���[�ɖ�̃X�J�C�{�b�N�X���Z�b�g
		RENDERER->SetActiveSkyBox(m_nightBox->GetCubeMapComp());
		m_nightBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

}

// ���ԑт��Ƃɕ��s���C�g��ݒ肷��
void Environment::SetDirectionalLight(GAME_TIME in_gameTime)
{
	m_gameTime = in_gameTime;

	if (m_gameTime == GAME_TIME::MORNING)
	{
		// ���C�e�B���O
		RENDERER->SetAmbientLight(Vector3(0.8f, 0.8f, 0.9f));
		DirectionalLight& dir = RENDERER->GetDirectionalLight();
		//dir.position = Vector3(30000.0f, 15400.0f, 15000.0f);
		dir.position = playerPos + ADJUST_POS;
		dir.target = playerPos;
		dir.direction = dir.target - dir.position;
		dir.direction.Normalize();
		dir.ambient = Vector3(0.4f, 0.4f, 0.4f);
		dir.diffuse = Vector3(1.0f, 1.0f, 1.0f);
		dir.specular = Vector3(0.8f, 0.8f, 0.8f);

		m_morningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (m_gameTime == GAME_TIME::AFTERNOON)
	{
		// ���C�e�B���O
		RENDERER->SetAmbientLight(Vector3(0.1f, 0.1f, 0.15f));
		DirectionalLight& dir = RENDERER->GetDirectionalLight();
		dir.position = playerPos + ADJUST_POS;
		dir.target = playerPos;
		dir.direction = dir.target - dir.position;
		dir.direction.Normalize();
		dir.ambient = Vector3(0.5f, 0.5f, 0.5f);
		dir.diffuse = Vector3(0.07f, 0.05f, 0.3f);
		dir.specular = Vector3(0.5f, 0.5f, 0.6f);

		m_morningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (m_gameTime == GAME_TIME::EVENING)
	{
		// ���C�e�B���O
		RENDERER->SetAmbientLight(Vector3(0.1f, 0.1f, 0.15f));
		DirectionalLight& dir = RENDERER->GetDirectionalLight();
		dir.position = playerPos + ADJUST_POS;
		dir.target = playerPos;
		dir.direction = dir.target - dir.position;
		dir.direction.Normalize();
		dir.ambient = Vector3(0.4f, 0.4f, 0.4f);
		dir.diffuse = Vector3(0.07f, 0.05f, 0.3f);
		dir.specular = Vector3(0.01f, 0.0f, 0.1f);

		m_eveningBox->GetCubeMapComp()->SetIsVisible(true);
		return;

	}

	if (m_gameTime == GAME_TIME::NIGHT)
	{
		// ���C�e�B���O
		RENDERER->SetAmbientLight(Vector3(0.15f, 0.15f, 0.2f));
		DirectionalLight& dir = RENDERER->GetDirectionalLight();
		dir.position = playerPos + ADJUST_POS;
		dir.target = playerPos;
		dir.direction = dir.target - dir.position;
		dir.direction.Normalize();
		dir.ambient = Vector3(0.2f, 0.2f, 0.2f);
		dir.diffuse = Vector3(0.07f, 0.05f, 0.3f);
		dir.specular = Vector3(0.08f, 0.05f, 0.4f);

		m_nightBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

}
