#include "ResultCarDoor.h"
#include "EnvironmentMapComponent.h"
#include "CarMeshComponent.h"
#include "Mesh.h"

const std::string ResultCarDoor::CAR_DOOR_LEFT_FRAME_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Door/DoorLeft/DoorFrameLeft_Internal.OBJ";
const std::string ResultCarDoor::CAR_DOOR_LEFT_GLASS_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Door/DoorLeft/DoorGlassLeftTransparency_Internal.OBJ";
const std::string ResultCarDoor::CAR_DOOR_RIGHT_FRAME_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Door/DoorRight/DoorFrameRight_Internal.OBJ";
const std::string ResultCarDoor::CAR_DOOR_RIGHT_GLASS_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Door/DoorRight/DoorGlassRightTransparency_Internal.OBJ";

// in_leftRight = 右側のドアか左側のドアかで位置補正ベクトルを変える
// in_leftRight = true → 左側     false → 右側
ResultCarDoor::ResultCarDoor(Actor* in_owner, DOOR_POS in_pos)
	:m_owner(in_owner)
{

	// 座標オフセットとメッシュのセット (左右で分岐)
	if (in_pos == DOOR_POS::LEFT)
	{
		m_adjustPos = Vector3(80.0f, -100.0f, 92.8f);
		// 左側
		Mesh* frame = RENDERER->GetMesh(CAR_DOOR_LEFT_FRAME_MESH_PATH);
		CarMeshComponent* frameMeshComp = new CarMeshComponent(this);
		frameMeshComp->SetMesh(frame);
		Mesh* glass = RENDERER->GetMesh(CAR_DOOR_LEFT_GLASS_MESH_PATH);
		//MeshComponent* glassMeshComp = new MeshComponent(this);
		EnvironmentMapComponent* glassMeshComp = new EnvironmentMapComponent(this);
		glassMeshComp->SetMesh(glass);
	}
	else if(in_pos == DOOR_POS::RIGHT)
	{
		m_adjustPos = Vector3(80.0f, 100.0f, 96.8f);
		// 右側
		Mesh* frame = RENDERER->GetMesh(CAR_DOOR_RIGHT_FRAME_MESH_PATH);
		CarMeshComponent* frameMeshComp = new CarMeshComponent(this);
		frameMeshComp->SetMesh(frame);
		Mesh* glass = RENDERER->GetMesh(CAR_DOOR_RIGHT_GLASS_MESH_PATH);
		//MeshComponent* glassMeshComp = new MeshComponent(this);
		EnvironmentMapComponent* glassMeshComp = new EnvironmentMapComponent(this);
		glassMeshComp->SetMesh(glass);
		glassMeshComp->SetMesh(glass);
	}

	//// 拡大率もオーナーに合わせる
	m_scale = m_owner->GetScale();
	// オーナーの座標をセット
	m_position = m_owner->GetPosition() + m_adjustPos;

	m_rotation = m_owner->GetRotation();


}

ResultCarDoor::~ResultCarDoor()
{
}

void ResultCarDoor::UpdateActor(float in_deltaTime)
{
	// オーナーの拡大率・座標・回転行列に合わせておく
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition() + m_adjustPos;
	m_rotation = m_owner->GetRotation();

	// オーナーに合わせるためワールド座標を取得し続ける (補正ベクトルで位置を補正)
	m_worldTransform = Matrix4::CreateTranslation(m_adjustPos) * m_owner->GetWorldTransform();

}
