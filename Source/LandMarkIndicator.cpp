#include "LandMarkIndicator.h"
#include "PlayerCar.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "WorldSpaceUI.h"

LandMarkIndicator::LandMarkIndicator(PlayerCar* in_car)
	:m_car(in_car)
	,m_ui(nullptr)
{
	//Mesh* mesh = RENDERER->GetMesh("Data/Meshes/TND/Interface/Arrow/UI_Arrow_Internal.OBJ");
	//m_meshComp = new MeshComponent(this);
	//m_meshComp->SetMesh(mesh);

	//m_ui = new WorldSpaceUI(m_car->GetPosition(), "Data/Meshes/TND/Interface/Arrow/UI_Arrow.png", 50.0f);
	//m_ui->SetVisible(true);

	//SetRotation(Quaternion::Quaternion(Vector3::UnitZ, Math::ToRadians(90.0f)));
	//SetScale(50.0f);
}


LandMarkIndicator::~LandMarkIndicator()
{
}

void LandMarkIndicator::UpdateActor(float in_deltaTime)
{
	// ƒ|ƒWƒVƒ‡ƒ“’ÇÕ
	//m_ui->SetPosition(m_car->GetPosition() + Vector3(0.0f, 0.0f, 50.0f));

}
