#include "WorldSpaceUI.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "CameraComponent.h"

Matrix4 WorldSpaceUI::m_staticBillboardMat;
const Vector3 adjustPos = Vector3(0.0f, 0.0f, 100.0f);

// �R���X�g���N�^
WorldSpaceUI::WorldSpaceUI(const Vector3& in_pos, const std::string& in_filePath, float in_scale)
	:m_position(in_pos + adjustPos)
	, m_scale(in_scale)
	, m_texture(nullptr)
	, m_isVisible(true)
	,m_worldTransform(Matrix4::Identity)
{
	// �e�N�X�`���̎擾�E����
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

// �f�X�g���N�^
WorldSpaceUI::~WorldSpaceUI()
{
	RENDERER->RemoveSpriteWorld(this);
}

// �`�揈��
void WorldSpaceUI::Draw(Shader* in_shader)
{
	if (m_isVisible)
	{
		// �u�����h�̃A�N�e�B�u��
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// �[�x�e�X�g�̒�~
		glDisable(GL_DEPTH_TEST);

		// �r���{�[�h�s��̃Z�b�g
		m_staticBillboardMat = GetBillboardMatrix();

		// ���[���h�s��A�X�P�[���s����쐬����
		Matrix4 mat, scale, projection, view;
		scale = Matrix4::CreateScale(m_scale);
		mat = Matrix4::CreateTranslation(m_position);
		projection = RENDERER->GetProjectionMatrix();
		view = RENDERER->GetViewMatrix();

		Matrix4 simpleViewProj = Matrix4::CreateSimpleViewProj(1920, 1080);
		in_shader->SetMatrixUniform("u_viewProj", simpleViewProj);

		// �V�F�[�_�̃��j�t�H�[���փ��[���h�����s��E�A���t�@�l���Z�b�g
		in_shader->SetMatrixUniform("u_worldTransform", scale * m_staticBillboardMat * mat);
		in_shader->SetInt("u_texture", 0);
		//in_shader->SetMatrixUniform("u_View", view);
		//in_shader->SetMatrixUniform("u_Projection", projection);

		Matrix4 gousei = scale * m_staticBillboardMat * mat * view * projection;
		printf("x : %f, y : %f, z : %f\n", gousei.GetXAxis().x, gousei.GetYAxis().y, gousei.GetZAxis().z);

		// �e�N�X�`���̃o�C���h
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureID());
		//m_texture->SetActive();
		// �X�v���C�g�p��VAO���o�C���h
		RENDERER->SetWorldSpriteVertex();
		// �L���[�u�`��
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

}

void WorldSpaceUI::SetUIPosition(const Vector3& in_pos)
{
	m_position = in_pos + adjustPos;
}

// �r���{�[�h�s��̌v�Z�E�擾
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
