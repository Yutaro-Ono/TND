//--------------------------------------------------------------+
// �w���R�v�^�[�N���X
//--------------------------------------------------------------+
#include "Helicopter.h"
#include "HeliBody.h"
#include "HeliRotorMain.h"
#include "HeliRotorBack.h"
#include "HeliSpotLight.h"
#include "PatrolComponent.h"
#include "GameWorld.h"
#include "PlayerManager.h"
#include "PointLight.h"
#include "InputController.h"

// �e��p�[�c�̃��b�V���p�X
const std::string HELI_BODY_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Body_Internal.OBJ";
const std::string HELI_ROTOR_MAIN_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Main_Rotor_Internal.OBJ";
const std::string HELI_ROTOR_BACK_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Tail_Rotor_Internal.OBJ";
const std::string HELI_SPOTLIGHT_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Spotlight_Internal.OBJ";

// �R���X�g���N�^
Helicopter::Helicopter(GameWorld* in_world, const Vector3& in_pos, int in_num)
	:m_patrolComp(nullptr)
	,m_state(HELI_STATE::STOP)
	,m_searchSphere(Vector3(in_pos.x, in_pos.y, 0.0f), 1000.0f)
	,m_foundPlayer(false)
	,m_number(in_num)
{
	// ���[���h�̃|�C���^���Z�b�g
	m_world = in_world;

	// ���W��ݒ�
	m_position = in_pos;
	// �X�P�[���l��45.0�ɐݒ�
	SetScale(45.0f);

	// �e��p�[�c�N���X�𐶐�
	m_body = new HeliBody(this, HELI_BODY_MESH_PATH);
	m_mainRotor = new HeliRotorMain(this, HELI_ROTOR_MAIN_MESH_PATH);
	m_backRotor = new HeliRotorBack(this, HELI_ROTOR_BACK_MESH_PATH);
	m_spotLight = new HeliSpotLight(this, HELI_SPOTLIGHT_MESH_PATH);

	// ����R���|�[�l���g�𐶐��E�ǉ�
	m_patrolComp = new PatrolComponent(this, m_world);


	// �|�C���g���C�g
	m_pLight[0] = new PointLight();
	m_pLight[0]->SetPosition(Vector3(m_position.x, m_position.y, 0.0f));
	m_pLight[0]->SetLightParameter(0.01f, 0.21f, 0.01f);

	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	m_pLight[1] = new PointLight();
	m_pLight[1]->SetLightColor(color, color);
	m_pLight[1]->SetPosition(Vector3(m_position.x, m_position.y, m_position.z - 10.0f));
	m_pLight[1]->SetLightParameter(0.01f, 0.21f, 0.01f);
}

// �f�X�g���N�^
Helicopter::~Helicopter()
{

}

// �X�V����
void Helicopter::UpdateActor(float in_deltaTime)
{
	if (m_state == HELI_STATE::PATROL)
	{
		// �w���̍��G�͈͂Ƀv���C���[���ڐG�������ǂ����̌��o
		SearchPlayer(m_world->GetPlayer());
	}

	// �|�C���g���C�g�̒Ǐ]����
	Vector3 lightPos = Vector3(m_position.x, m_position.y, m_position.z);
	m_pLight[0]->SetPosition(lightPos * Vector3(1.0f, 1.0f, 0.0f));
	Vector3 trns = Vector3::Transform(Vector3(50.0f, 0.0f, -10.0f), m_rotation);
	m_pLight[1]->SetPosition(lightPos + trns);
}

// �w���̍��G�͈͂Ƀv���C���[���ڐG�������ǂ����̌��o
void Helicopter::SearchPlayer(PlayerManager* in_player)
{
	// �T�[�`�͈͂��X�V
	m_searchSphere.m_center.x = m_position.x;
	m_searchSphere.m_center.y = m_position.y;

	// �v���C���[�𔭌��������ǂ���
	if (m_searchSphere.Contains(in_player->GetPosition()))
	{
		// �v���C���[�����ȏ�̑��x�ő����Ă��鎞�̂ݔ�����Ԃ�
		if (in_player->GetPlayerCar()->GetAccelValue() - in_player->GetPlayerCar()->GetBrakeValue() >= 15.0f)
		{
			m_foundPlayer = true;
			printf("�w�����v���C���[�𔭌�����\n");
		}

	}
	else
	{
		m_foundPlayer = false;
	}
}
