#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "Math.h"

class ShadowMap
{
public:


	ShadowMap();
	~ShadowMap();

	// ���̎��_����V�[���������_�����O���f�v�X�o�b�t�@�𓾂�
	void RenderDepthMapFromLightView(const std::vector<class MeshComponent*>& in_mesh, 
		                             const std::vector<class SkeletalMeshComponent*> in_skelMesh,
		                             const std::vector<class CarMeshComponent*> in_carMesh);          // �ʏ탁�b�V���E�X�L�����b�V���E�ԃ��b�V��



	void DrawShadowMesh(const std::vector<class MeshComponent*>& in_mesh);
	void DrawShadowMesh(const std::vector<class MeshComponent*>& in_mesh, const std::vector<class SkeletalMeshComponent*>& in_skelMesh);

	// Getter/Setter
	class Shader* GetDepthShader() { return m_depthShader; }
	class Shader* GetShadowShader() { return m_shadowShader; }
	unsigned int GetDepthMap() { return m_depthMap; }

private:


	unsigned int m_depthMapFBO;              // �f�v�X�o�b�t�@�I�u�W�F�N�g
	unsigned int m_depthMap;                 // �[�x���������ނ��߂�2D�e�N�X�`��

	class Shader* m_depthShader;          // �f�v�X�V�F�[�_ (�e�`��̑O�����Ɏg�p����)
	class Shader* m_depthSkinShader;      // �f�v�X�V�F�[�_ (�X�L�����b�V���p)
	class Shader* m_shadowShader;         // �V���h�E�}�b�s���O�V�F�[�_
	class Shader* m_skinShadowShader;     // �V���h�E�K�p�̃X�L���V�F�[�_

	// ���C�g��ԗp�s��
	Matrix4 m_lightView, m_lightProj, m_lightSpace;


	// �V���h�E�}�b�v�T�C�Y
	static const int SHADOW_WIDTH;
	static const int SHADOW_HEIGHT;

};