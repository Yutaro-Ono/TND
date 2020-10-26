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

// �e��p�[�c�̃��b�V���p�X
const std::string HELI_BODY_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Body_Internal.OBJ";
const std::string HELI_ROTOR_MAIN_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Main_Rotor_Internal.OBJ";
const std::string HELI_ROTOR_BACK_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Tail_Rotor_Internal.OBJ";
const std::string HELI_SPOTLIGHT_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Spotlight_Internal.OBJ";

// �R���X�g���N�^
Helicopter::Helicopter(GameWorld* in_world, const Vector3& in_pos)
	:m_patrolComp(nullptr)
	,m_searchSphere(Vector3(in_pos.x, in_pos.y, 0.0f), 100.0f)
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
	AddComponent(m_patrolComp);


	
}

// �f�X�g���N�^
Helicopter::~Helicopter()
{
}

// �X�V����
void Helicopter::UpdateActor(float in_deltaTime)
{
	// �T�[�`�͈͂��X�V
	m_searchSphere.m_center.x = m_position.x;
	m_searchSphere.m_center.y = m_position.y;


	if (m_searchSphere.Contains(m_world->GetPlayer()->GetPosition()))
	{
		printf("�w���ƃv���C���[���������I\n");
	}

}
