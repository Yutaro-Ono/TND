#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Skeleton.h"
#include "ShadowMap.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* in_owner)
	:MeshComponent(in_owner, true)
	, m_skeleton(nullptr)
{
}

void SkeletalMeshComponent::Draw(Shader* in_shader)                         // �`��
{
	if (m_mesh && m_visible)
	{
		// ���[���h�ϊ��s��Z�b�g
		in_shader->SetMatrixUniform("u_worldTransform",
			m_owner->GetWorldTransform());
		// �s��p���b�g���Z�b�g    
		in_shader->SetMatrixUniforms("u_matrixPalette", &m_palette.mEntry[0],
			MAX_SKELETON_BONES);
		// �X�y�L�����[���x���Z�b�g
		in_shader->SetFloatUniform("u_specPower", 21);

		// �e��e�N�X�`�����V�F�[�_�ɃZ�b�g����
		// �e�N�X�`�����ǂݍ��܂�Ă��Ȃ��ꍇ�͖����Ȑ��� "0" ���Ԃ����
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::DIFFUSE_MAP));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::SPECULAR_MAP));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::NORMAL_MAP));
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(TEXTURE_TYPE::EMISSIVE_MAP));
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, RENDERER->GetShadowMap()->GetDepthMap());


		// ���b�V���̒��_�z����A�N�e�B�u��
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// �`��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::DrawShadow(Shader* in_shader)
{
	if (m_mesh)
	{
		// ���[���h�ϊ��s��Z�b�g
		in_shader->SetMatrixUniform("u_worldTransform",
			m_owner->GetWorldTransform());
		// �s��p���b�g���Z�b�g    
		in_shader->SetMatrixUniforms("u_matrixPalette", &m_palette.mEntry[0],
			MAX_SKELETON_BONES);

		// ���b�V���̒��_�z����A�N�e�B�u��
		VertexArray* va = m_mesh->GetVertexArray();
		va->SetActive();
		// �`��
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::Update(float in_deltaTime)
{
	if (m_animation && m_skeleton)
	{
		m_animTime += in_deltaTime * m_animPlayRate;

		// �A�j���[�V���������[�v�A�j���[�V�����Ȃ犪���߂�����
		if (m_animation->IsLoopAnimation())
		{
			// �A�j���������߂��čĐ�
			while (m_animTime > m_animation->GetDuration())
			{
				m_animTime -= m_animation->GetDuration();
			}
		}
		// ���[�v���Ȃ��A�j���ōĐ����Ԓ�������ŏI���Ԃ܂łƂ���
		else if (m_animTime > m_animation->GetDuration())
		{
			m_animTime = m_animation->GetDuration();
		}
		// �s��p���b�g�̍Čv�Z
		ComputeMatrixPalette();
	}
}

float SkeletalMeshComponent::PlayAnimation(const Animation* anim, float playRate)  // �A�j���[�V�����̍Đ�
{
	m_animation = anim;
	m_animTime = 0.0f;
	m_animPlayRate = playRate;

	if (!m_animation) { return 0.0f; }

	ComputeMatrixPalette();

	return m_animation->GetDuration();
}

// ���݃A�j���[�V�����Đ������H true : �Đ��� / false : �Đ��I��
bool SkeletalMeshComponent::IsPlaying()
{
	if (!m_animation->IsLoopAnimation())
	{
		if (m_animTime >= m_animation->GetDuration())
		{
			return false;
		}
	}
	return true;
}

// �s��p���b�g�̌v�Z
void SkeletalMeshComponent::ComputeMatrixPalette()
{
	const std::vector<Matrix4>& globalInvBindPoses = m_skeleton->GetGlobalInvBindPoses();   // �O���[�o���t�o�C���h�s��z��̎擾
	std::vector<Matrix4> currentPoses;                                                      // ���݂̃|�[�Y�s��
	m_animation->GetGlobalPoseAtTime(currentPoses, m_skeleton, m_animTime);                 // �A�j���������_�̃O���[�o���|�[�Y�̎擾

	// ���ꂼ��̃{�[���̍s��p���b�g�̃Z�b�g
	for (size_t i = 0; i < m_skeleton->GetNumBones(); i++)
	{
		// �s��p���b�g[i] = �O���[�o���t�o�C���h�s��[i]�@�~�@���݂̃|�[�Y�s��[i]  (i�̓{�[��ID)         
		m_palette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}
