#include "LightGlassComponent.h"
#include "Mesh.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Actor.h"
#include "Renderer.h"

LightGlassComponent::LightGlassComponent(Actor* in_owner)
	:Component(in_owner)
	,m_mesh(nullptr)
	,m_lightColor(Vector3(0.5f, 0.5f, 0.5f))
	,m_luminance(1.0f)
	,m_isVisible(false)
{
	RENDERER->AddLightGlassComponent(this);
}

LightGlassComponent::~LightGlassComponent()
{
	RENDERER->RemoveLightGlassComponent(this);
}

void LightGlassComponent::Draw(Shader* in_shader)
{

	if (!m_isVisible)
	{
		// GL�̃A���t�@�u�����h�E���e�X�g��ON
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// uniform�ւ̃Z�b�g
		in_shader->SetMatrixUniform("u_worldTransform", m_owner->GetWorldTransform());
		in_shader->SetVectorUniform("u_lightColor", m_lightColor);
		in_shader->SetFloat("u_luminance", m_luminance);

		// ���_�z��̃o�C���h
		VertexArray* vao = m_mesh->GetVertexArray();
		vao->SetActive();

		// �`��
		glDrawElements(GL_TRIANGLES, vao->GetNumIndices(), GL_UNSIGNED_INT, nullptr);

		// �A���t�@�e�X�g�̃I�t
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
	}

}

