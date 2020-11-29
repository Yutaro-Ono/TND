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
	in_shader->SetFloat("u_pointLight.constant", m_light->GetConstant());
	in_shader->SetFloat("u_pointLight.linear", m_light->GetLinear());
	in_shader->SetFloat("u_pointLight.quadratic", m_light->GetQuadratic());
	// �J���[�̃Z�b�g
	in_shader->SetVectorUniform("u_pointLight.diffuse", m_light->GetDiffuseColor());
	in_shader->SetVectorUniform("u_pointLight.ambient", m_light->GetAmbientColor());
	in_shader->SetVectorUniform("u_pointLight.specular", m_light->GetSpecularColor());

	// ���C�g�{�����[�����a����s����X�V
	Matrix4 lightVolume = Matrix4::CreateScale(m_light->GetLightRadius() * m_light->GetScale());
	lightVolume = lightVolume * Matrix4::CreateTranslation(m_light->GetPosition());
	// ���W�̃Z�b�g
	in_shader->SetVectorUniform("u_pointLight.position", m_light->GetPosition());
	in_shader->SetMatrixUniform("u_worldTransform", lightVolume);

	// ���_�z����A�N�e�B�u��
	VertexArray* va = m_mesh->GetVertexArray();
	va->SetActive();
	// �`�悷��
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}
