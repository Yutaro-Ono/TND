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

// �`�揈��
void UIWorld::Draw(Shader* in_shader)
{
	// ���[���hUI�p�̃V�F�[�_�A�N�e�B�u��
	in_shader->SetActive();

	// ���[���h�s��A�X�P�[���s����쐬����
	Matrix4 mat, scale;
	scale = Matrix4::CreateScale(m_scale);
	mat = Matrix4::CreateTranslation(m_position);

	// �V�F�[�_�̃��j�t�H�[���փ��[���h�����s��E�A���t�@�l���Z�b�g

}
