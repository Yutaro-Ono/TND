//--------------------------------------------------------------+
// �w���R�v�^�[�N���X
//--------------------------------------------------------------+
#include "Helicopter.h"
#include "HeliBody.h"
#include "HeliRotorMain.h"
#include "HeliRotorBack.h"
#include "HeliSpotLight.h"

// �e��p�[�c�̃��b�V���p�X
const std::string HELI_BODY_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Body_Internal.OBJ";
const std::string HELI_ROTOR_MAIN_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Main_Rotor_Internal.OBJ";
const std::string HELI_ROTOR_BACK_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Tail_Rotor_Internal.OBJ";
const std::string HELI_SPOTLIGHT_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Spotlight_Internal.OBJ";

Helicopter::Helicopter(const Vector3& in_pos)
{
	m_position = in_pos;
	// �X�P�[���l��45.0�ɐݒ�
	SetScale(45.0f);

	// �e��p�[�c�N���X�𐶐�
	m_body = new HeliBody(this, HELI_BODY_MESH_PATH);
	m_mainRotor = new HeliRotorMain(this, HELI_ROTOR_MAIN_MESH_PATH);
	m_backRotor = new HeliRotorBack(this, HELI_ROTOR_BACK_MESH_PATH);
	m_spotLight = new HeliSpotLight(this, HELI_SPOTLIGHT_MESH_PATH);


}

Helicopter::~Helicopter()
{
}

void Helicopter::UpdateActor(float in_deltaTime)
{
	
}
