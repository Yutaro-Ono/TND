#include "WorldSpaceUI.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "CameraComponent.h"

Matrix4 WorldSpaceUI::m_staticBillboardMat;
const Vector3 adjustPos = Vector3(0.0f, 0.0f, 100.0f);

// コンストラクタ
WorldSpaceUI::WorldSpaceUI(const Vector3& in_pos, const std::string& in_filePath, float in_scale)
	:m_position(in_pos + adjustPos)
	, m_scale(in_scale)
	, m_texture(nullptr)
	, m_isVisible(true)
	,m_worldTransform(Matrix4::Identity)
{
	// テクスチャの取得・生成
	m_texture = RENDERER->GetTexture(in_filePath);
	RENDERER->AddSpriteWorld(this);
}

WorldSpaceUI::WorldSpaceUI(const Vector3& in_pos, Texture* in_texture, float in_scale)
	:m_position(in_pos + adjustPos)
	, m_scale(in_scale)
	, m_texture(nullptr)
	, m_isVisible(true)
{
	m_texture = in_texture;
	RENDERER->AddSpriteWorld(this);
}

// デストラクタ
WorldSpaceUI::~WorldSpaceUI()
{
	RENDERER->RemoveSpriteWorld(this);
}

// 描画処理
void WorldSpaceUI::Draw(Shader* in_shader)
{
	if (m_isVisible)
	{
		// ブレンドのアクティブ化
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// 深度テストの停止
		glDisable(GL_DEPTH_TEST);

		// ビルボード行列のセット
		m_staticBillboardMat = GetBillboardMatrix();

		// ワールド行列、スケール行列を作成する
		Matrix4 mat, scale, projection, view;
		scale = Matrix4::CreateScale(m_scale);
		mat = Matrix4::CreateTranslation(m_position);
		projection = RENDERER->GetProjectionMatrix();
		view = RENDERER->GetViewMatrix();

		Matrix4 simpleViewProj = Matrix4::CreateSimpleViewProj(1920, 1080);
		in_shader->SetMatrixUniform("u_viewProj", simpleViewProj);

		// シェーダのユニフォームへワールド合成行列・アルファ値をセット
		in_shader->SetMatrixUniform("u_worldTransform", scale * m_staticBillboardMat * mat);
		in_shader->SetInt("u_texture", 0);
		//in_shader->SetMatrixUniform("u_View", view);
		//in_shader->SetMatrixUniform("u_Projection", projection);

		Matrix4 gousei = scale * m_staticBillboardMat * mat * view * projection;
		printf("x : %f, y : %f, z : %f\n", gousei.GetXAxis().x, gousei.GetYAxis().y, gousei.GetZAxis().z);

		// テクスチャのバインド
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureID());
		//m_texture->SetActive();
		// スプライト用のVAOをバインド
		RENDERER->SetWorldSpriteVertex();
		// キューブ描画
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

}

void WorldSpaceUI::SetUIPosition(const Vector3& in_pos)
{
	m_position = in_pos + adjustPos;
}

// ビルボード行列の計算・取得
Matrix4 WorldSpaceUI::GetBillboardMatrix()
{
	Matrix4 ret;
	ret = RENDERER->GetViewMatrix();
	ret.mat[3][0] = ret.mat[3][1] = ret.mat[3][2] = 0.0f;
	ret.Transpose();
	//ret.mat[1][1] *= -1;
	ret.mat[2][2] *= -1;


	Matrix4 Rot;
	Rot = Matrix4::CreateRotationX(-0.5f * Math::Pi);
	ret = Rot * ret;

	return Matrix4(ret);

}
