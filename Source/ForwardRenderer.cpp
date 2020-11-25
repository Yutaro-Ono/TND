#include "ForwardRenderer.h"
#include "Renderer.h"

// �R���X�g���N�^
ForwardRenderer::ForwardRenderer(Renderer* in_renderer)
	:m_renderer(in_renderer)
{
}

// �f�X�g���N�^
ForwardRenderer::~ForwardRenderer()
{
}

// �`�揈��
//void ForwardRenderer::Draw()
//{
//
//	// �[�x�e�X�g��ON�A�u�����h���I�t
//	glEnable(GL_DEPTH_TEST);
//	glDisable(GL_BLEND);
//	// ��ʂ��N���A
//	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//	// �J���[�o�b�t�@�̃N���A
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//	//----------------------------------------------+
//	// ���b�V���V�F�[�_�[(phong)
//	//----------------------------------------------+
//	if (m_switchShader == 2)
//	{
//		// �t���[���o�b�t�@�������ݏ���
//		m_frameBuffer->WriteFrameBuffer();
//
//		//���b�V���V�F�[�_�[�ŕ`�悷��Ώۂ̕ϐ����Z�b�g
//		m_meshShader->SetActive();
//		m_meshShader->SetMatrixUniform("u_viewProj", m_view * m_projection);
//		// ���C�e�B���O�ϐ����Z�b�g
//		SetLightUniforms(m_meshShader);
//		// �S�Ẵ��b�V���R���|�[�l���g��`��
//		for (auto mc : m_meshComponents)
//		{
//			if (mc->GetVisible())
//			{
//				mc->Draw(m_meshShader);
//			}
//		}
//
//		//-----------------------------------------------------------+
//		// �{�[�����胁�b�V���̕`��
//		//-----------------------------------------------------------+
//		m_skinnedShader->SetActive();
//		// �r���[�E�v���W�F�N�V�����̍����s����Z�b�g
//		m_skinnedShader->SetMatrixUniform("u_viewProj", m_view * m_projection);
//		// ���C�e�B���O�ϐ����Z�b�g
//		SetLightUniforms(m_skinnedShader);
//
//		for (auto sk : m_skeletalMeshComponents)
//		{
//			if (sk->GetVisible())
//			{
//				sk->Draw(m_skinnedShader);
//			}
//		}
//	}
//
//	//-----------------------------------------------+
//	// ���b�V���V�F�[�_�[(normal)
//	//-----------------------------------------------+
//	if (m_switchShader == 1)
//	{
//		// �t���[���o�b�t�@�������ݏ���
//		m_frameBuffer->WriteFrameBuffer();
//
//		//���b�V���V�F�[�_�[�ŕ`�悷��Ώۂ̕ϐ����Z�b�g
//		m_meshNormalShader->SetActive();
//		m_meshNormalShader->SetMatrixUniform("u_viewProj", m_view * m_projection);
//		// ���C�e�B���O�ϐ����Z�b�g
//		SetLightUniforms(m_meshNormalShader);
//		m_meshNormalShader->SetVectorUniform("u_lightPos", m_directionalLight.position);
//		m_meshNormalShader->SetVectorUniform("u_viewPos", m_view.GetTranslation());
//		// �S�Ẵ��b�V���R���|�[�l���g��`��
//		for (auto mc : m_meshComponents)
//		{
//			if (mc->GetVisible())
//			{
//				mc->Draw(m_meshNormalShader);
//			}
//		}
//
//		//-----------------------------------------------------------+
//		// �{�[�����胁�b�V���̕`��
//		//-----------------------------------------------------------+
//		m_skinnedShader->SetActive();
//		// �r���[�E�v���W�F�N�V�����̍����s����Z�b�g
//		m_skinnedShader->SetMatrixUniform("u_viewProj", m_view * m_projection);
//		// ���C�e�B���O�ϐ����Z�b�g
//		SetLightUniforms(m_skinnedShader);
//
//		for (auto sk : m_skeletalMeshComponents)
//		{
//			if (sk->GetVisible())
//			{
//				sk->Draw(m_skinnedShader);
//			}
//		}
//	}
//
//	//-----------------------------------------------+
//	// ���b�V���V�F�[�_�[(shadow)
//	//-----------------------------------------------+
//	//if (m_switchShader == 0)
//	//{
//	//	m_shadowMap->RenderDepthMapFromLightView(m_meshComponents, m_skeletalMeshComponents);
//
//	//	// �t���[���o�b�t�@�������ݏ���
//	//	m_frameBuffer->WriteFrameBuffer();
//
//	//	m_shadowMap->DrawShadowMesh(m_meshComponents, m_skeletalMeshComponents);
//	//}
//
//
//
//	//-----------------------------------------------+
//	// ���}�b�v�I�u�W�F�N�g�̕`��
//	//-----------------------------------------------+
//	// �V�F�[�_��L�����Euniform�Ɋe�s����Z�b�g
//	m_environmentMapShader->SetActive();
//	m_environmentMapShader->SetMatrixUniform("u_viewMat", m_view);
//	m_environmentMapShader->SetMatrixUniform("u_projMat", m_projection);
//	m_environmentMapShader->SetVectorUniform("u_viewPos", m_view.GetTranslation());
//	m_environmentMapShader->SetInt("u_skybox", 0);
//	for (auto env : m_envMeshComponents)
//	{
//		env->DrawEnvironmentMap(m_environmentMapShader);
//	}
//
//
//
//	if (m_switchShader == 0)
//	{
//		m_shadowMap->RenderDepthMapFromLightView(m_meshComponents, m_skeletalMeshComponents);
//
//		// �t���[���o�b�t�@�������ݏ���
//		m_frameBuffer->WriteFrameBuffer();
//
//		m_bloom->WriteBuffer(m_meshComponents, m_skeletalMeshComponents, m_activeSkyBox, m_envMeshComponents);
//		m_bloom->WriteBuffer(m_particleManager);
//
//		m_bloom->DrawDownSampling();
//		m_bloom->DrawGaussBlur();
//		m_bloom->DrawBlendBloom();
//
//	}
//
//
//	//---------------------------------------------------------------+
//	// �X�J�C�{�b�N�X�̕`��
//	//---------------------------------------------------------------+
//	// �L���[�u�}�b�v�V�F�[�_���A�N�e�B�u���E�L���[�uVAO���o�C���h
//	//m_activeSkyBox->Draw(m_skyboxShader);
//
//	//----------------------------------------------------------------+
//	// �p�[�e�B�N���`��
//	//----------------------------------------------------------------+
//	//glEnable(GL_DEPTH_TEST);
//	//m_particleManager->Draw();
//	// ���[���h��ԏ�̃X�v���C�g�`��
//	m_worldSpaceSpriteShader->SetActive();
//	m_worldSpaceSpriteShader->SetMatrixUniform("u_View", m_view);
//	m_worldSpaceSpriteShader->SetMatrixUniform("u_Projection", m_projection);
//	for (auto spr : m_worldSprites)
//	{
//		spr->Draw(m_worldSpaceSpriteShader);
//	}
//	// �t���[���o�b�t�@�`��
//	m_frameBuffer->DrawFrameBuffer();
//
//	// Sprite�̕`��
//	// �u�����h�̃A�N�e�B�u��
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	// �[�x�e�X�g�̒�~
//	glDisable(GL_DEPTH_TEST);
//	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
//	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
//
//	// sprite�V�F�[�_�[�̃A�N�e�B�u��
//	m_spriteVerts->SetActive();
//	m_spriteShader->SetActive();
//
//	for (auto sprite : m_spriteComponents)
//	{
//		if (sprite->GetVisible())
//		{
//			sprite->Draw(m_spriteShader);
//		}
//	}
//
//	// �S�Ă�UI���X�V
//	for (auto ui : GAME_INSTANCE.GetUIStack())
//	{
//		ui->Draw(m_spriteShader);
//	}
//
//
//
//
//}
