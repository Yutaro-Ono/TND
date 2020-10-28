#include "CubeMapComponent.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "Math.h"
#include "SkyBox.h"
#include <vector>


CubeMapComponent::CubeMapComponent(Actor* in_owner)
	:Component(in_owner)
	,m_texture(nullptr)
	,m_isVisible(false)
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

// キューブマップの描画処理
void CubeMapComponent::Draw(Shader* in_shader)
{

	if (m_isVisible)
	{
		// ビュー行列、プロジェクション行列を取得
		Matrix4 view, projection;
		view = RENDERER->GetViewMatrix();
		projection = RENDERER->GetProjectionMatrix();
		view.Invert();

		// Uniformに行列をセット
		in_shader->SetMatrixUniform("u_View", view);
		in_shader->SetMatrixUniform("u_Proj", projection);

		glDepthFunc(GL_LEQUAL);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture->GetTextureID());
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}

}
