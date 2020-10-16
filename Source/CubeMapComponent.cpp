#include "CubeMapComponent.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include <vector>


CubeMapComponent::CubeMapComponent(Actor* in_owner)
	:Component(in_owner)
	,m_texture(nullptr)
{
	RENDERER->AddSkyBox(this);
}

CubeMapComponent::~CubeMapComponent()
{
	RENDERER->RemoveSkyBox(this);
}

// テクスチャの生成
void CubeMapComponent::CreateTexture(const std::string& in_filePath)
{
	m_texture = new Texture();

	m_texture->LoadSkyBox(in_filePath);

}

void CubeMapComponent::Draw(Shader* in_shader)
{
	// glBind ↓
	RENDERER->SetCubeMapVAO();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture->GetTextureID());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}
