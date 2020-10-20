#include "ClientActor.h"
#include "SpriteComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "Texture.h"

ClientActor::ClientActor(const Vector3& in_pos)
	:m_isSelected(false)
	,m_landMark(nullptr)
{
	m_position = in_pos;

	Texture* texture = new Texture();
	texture->Load("Data/Interface/landmark.png");
	m_landMark = new SpriteComponent(this);
	m_landMark->SetTexture(texture);


	Mesh* mesh = RENDERER->GetMesh("Data/Meshes/TND/Actors/Player/rp_nathan_rigged_003_ue4.gpmesh");
	SkeletalMeshComponent* skel = new SkeletalMeshComponent(this);
	skel->SetMesh(mesh);
	skel->SetSkeleton(RENDERER->GetSkeleton("Data/Meshes/TND/Actors/Player/rp_nathan_rigged_003_ue4.gpskel"));
}

ClientActor::~ClientActor()
{
}

void ClientActor::UpdateActor(float in_deltaTime)
{
	m_recomputeWorldTransform = true;
}
