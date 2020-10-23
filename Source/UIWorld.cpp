#include "UIWorld.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"

Matrix4 UIWorld::m_staticBillboardMat;
Vector3 UIWorld::m_cameraWorldPos;

UIWorld::UIWorld(const Vector3& in_pos, const std::string& in_filePath, float in_scale)
	:m_position(in_pos)
	,m_scale(in_scale)
	,m_texture(nullptr)
	,m_isVisible(false)
{

}

UIWorld::~UIWorld()
{
}

// 描画処理
void UIWorld::Draw(Shader* in_shader)
{
	// ワールドUI用のシェーダアクティブ化
	in_shader->SetActive();

	// ワールド行列、スケール行列を作成する
	Matrix4 mat, scale;
	scale = Matrix4::CreateScale(m_scale);
	mat = Matrix4::CreateTranslation(m_position);

	// シェーダのユニフォームへワールド合成行列・アルファ値をセット

}
