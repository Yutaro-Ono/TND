#include "LandMarkIndicator.h"
#include "PlayerManager.h"
#include "PlayerHuman.h"
#include "PlayerCar.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "WorldSpaceUI.h"
#include "CameraComponent.h"

LandMarkIndicator::LandMarkIndicator(PlayerManager* in_player)
	:m_player(in_player)
{
	SetScale(750.0f);
	// マップ矢印のモデル読み込み
	Mesh* mesh = RENDERER->GetMesh("Data/Meshes/TND/Interface/Arrow/MapHUD_Arrow.OBJ");
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);
	m_meshComp->SetVisible(false);
	m_meshComp->SetMapColor(Vector3(1.0f, 0.1f, 0.6f));
}


LandMarkIndicator::~LandMarkIndicator()
{
}

void LandMarkIndicator::UpdateActor(float in_deltaTime)
{
	// プレイヤー追跡
	if (m_player->GetPlayerMode() == PlayerManager::MODE_HUMAN)
	{
		SetPosition(m_player->GetPlayerHuman()->GetPosition());
		SetRotation(m_player->GetPlayerHuman()->GetRotation());
	}
	else
	{
		SetPosition(m_player->GetPlayerCar()->GetPosition());
		SetRotation(m_player->GetPlayerCar()->GetRotation());
	}
	
}
