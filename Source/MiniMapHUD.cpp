#include "MiniMapHUD.h"
#include "GameMain.h"
#include "Renderer.h"
#include "GameWorld.h"
#include "PlayerManager.h"

MiniMapHUD::MiniMapHUD(GameWorld* in_world)
{
	Vector3 viewPos = RENDERER->GetViewMatrix().GetTranslation();
	Matrix4 view = Matrix4::CreateLookAt(Vector3(viewPos.x, viewPos.y, 500.0f), in_world->GetPlayer()->GetPosition(), Vector3::UnitX);
}

MiniMapHUD::~MiniMapHUD()
{
}

void MiniMapHUD::WriteBuffer(std::vector<class MeshComponent*> in_mesh)
{
}

// ミニマップ用のフレームバッファ作成
void MiniMapHUD::CreateFBO(unsigned int& in_fbo)
{
	glGenFramebuffers(1, &in_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, in_fbo);




}
