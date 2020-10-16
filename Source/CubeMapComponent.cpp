#include "CubeMapComponent.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include <vector>
#include <string>

CubeMapComponent::CubeMapComponent(Actor* in_owner)
	:Component(in_owner)
	,m_texture(nullptr)
{
	RENDERER->AddSkyBox(this);
}

CubeMapComponent::~CubeMapComponent()
{
}

// テクスチャの生成
void CubeMapComponent::CreateTexture(const std::string& in_filePath)
{
	m_texture = new Texture();

	// 各スカイボックス用画像のパス
	std::vector<std::string> faces
	{
		in_filePath + "right.jpg",
		in_filePath + "left.jpg",
		in_filePath + "top.jpg",
		in_filePath + "bottom.jpg",
		in_filePath + "front.jpg",
		in_filePath + "back.jpg"
	};

	m_texture->LoadSkyBox(faces);

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
