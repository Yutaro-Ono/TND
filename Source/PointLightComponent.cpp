#include "PointLightComponent.h"
#include "PointLight.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Shader.h"
#include "Mesh.h"

PointLightComponent::PointLightComponent(PointLight* in_light)
	:Component(in_light)
	,m_light(in_light)
	,m_mesh(nullptr)
{
	// �����_���[�ɂ��̃|�C���g���C�g��ǉ�
	RENDERER->AddPointLightComponent(this);
	// ���b�V���̃��[�h
	m_mesh = RENDERER->GetMesh("Data/Meshes/TND/Light/PointLight/PointLight_Sphere.OBJ");
}

PointLightComponent::~PointLightComponent()
{
	RENDERER->RemovePointLightComponent(this);
}

// ���C�g�̕`��
void PointLightComponent::Draw(Shader* in_shader)
{
	// �|�C���g���C�g�p�����[�^���Z�b�g
	in_shader->SetVectorUniform("u_pl.position", m_light->GetPosition());
	in_shader->SetFloat("u_pl.constant", m_light->GetConstant());
	in_shader->SetFloat("u_pl.linear", m_light->GetLinear());
	in_shader->SetFloat("u_pl.quadratic", m_light->GetQuadratic());
	// �J���[�̃Z�b�g
	in_shader->SetVectorUniform("u_pl.diffuse", m_light->GetDiffuseColor());
	in_shader->SetVectorUniform("u_pl.ambient", m_light->GetAmbientColor());
	in_shader->SetVectorUniform("u_pl.specular", m_light->GetSpecularColor());

	Matrix4 mat = Matrix4::CreateScale(m_light->GetLightRadius());
	mat = mat * m_light->GetWorldTransform();
	// ���W�̃Z�b�g
	in_shader->SetMatrixUniform("u_worldTransform", mat);

	// ���_�z����A�N�e�B�u��
	VertexArray* va = m_mesh->GetVertexArray();
	va->SetActive();
	// �`�悷��
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, 0);
}
