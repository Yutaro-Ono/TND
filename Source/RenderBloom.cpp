//----------------------------------------------------------------------------------------+
//
// HDR(HighDynamicRange)�Ή��̃t���[���o�b�t�@�Ƃ���𗘗p����Bloom�������s��
//
//----------------------------------------------------------------------------------------+
#include "RenderBloom.h"
#include "GameMain.h"
#include "Renderer.h"
#include "GameConfig.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CubeMapComponent.h"
#include "EnvironmentMapComponent.h"
#include "ShadowMap.h"
#include "ParticleManager.h"
#include "VertexArray.h"
#include "Shader.h"
#include <iostream>

const unsigned int maxLevelNum = 5;

// �R���X�g���N�^
RenderBloom::RenderBloom()
	:m_multiRenderTargetShader(nullptr)
	, m_multiRenderSkinShader(nullptr)
	, m_multiRenderCubeMapShader(nullptr)
	, m_multiRenderEnvironmentShader(nullptr)
	,m_hdrBloomShader(nullptr)
	,m_downSamplingShader(nullptr)
	,m_gaussShader(nullptr)
	,m_exposure(1.0f)
	,m_gamma(0.085f)
{

	m_saveExposure = m_exposure;

	// Bloom�pFBO�ƃe�N�X�`���𐶐�
	if (!CreateHDRFBO())
	{
		std::cout << "HDR�o�b�t�@�̐����Ɏ��s" << std::endl;
	}
	CreateBlurFBO();


	// �V�F�[�_�̃��[�h
	m_multiRenderTargetShader = new Shader();
	m_multiRenderTargetShader->Load("Data/Shaders/MultiRender/MultiRenderNormShadow.vert", "Data/Shaders/MultiRender/MultiRenderNormShadow.frag");
	m_multiRenderSkinShader = new Shader();
	m_multiRenderSkinShader->Load("Data/Shaders/MultiRender/MultiRenderNormShadowSkinned.vert", "Data/Shaders/MultiRender/MultiRenderNormShadowSkinned.frag");
	m_multiRenderCubeMapShader = new Shader();
	m_multiRenderCubeMapShader->Load("Data/Shaders/MultiRender/MultiRenderSkyBox.vert", "Data/Shaders/MultiRender/MultiRenderSkyBox.frag");
	m_multiRenderEnvironmentShader = new Shader();
	m_multiRenderEnvironmentShader->Load("Data/Shaders/MultiRender/MultiRenderEnvironmentMap.vert", "Data/Shaders/MultiRender/MultiRenderEnvironmentMap.frag");
	// Bloom�V�F�[�_
	m_hdrBloomShader = new Shader();
	m_hdrBloomShader->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/MultiRender/Bloom.frag");
	// �_�E���T���v�����O�p�V�F�[�_
	m_downSamplingShader = new Shader();
	m_downSamplingShader->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/MultiRender/DownSampling.frag");
	// �K�E�X�ڂ����p�V�F�[�_
	m_gaussShader = new Shader();
	m_gaussShader->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/MultiRender/GaussBlur.frag");
}

// �f�X�g���N�^
RenderBloom::~RenderBloom()
{
	delete m_multiRenderTargetShader;
	delete m_multiRenderSkinShader;
	delete m_multiRenderCubeMapShader;
	delete m_multiRenderEnvironmentShader;
	delete m_hdrBloomShader;
	delete m_downSamplingShader;
	delete m_gaussShader;

	for (auto fbo : m_blurFBO)
	{
		glDeleteFramebuffers(1, &fbo);
	}
	m_blurFBO.clear();

	for (auto tex : m_blurBufferTex)
	{
		glDeleteTextures(1, &tex);
	}
	m_blurBufferTex.clear();

	glDeleteFramebuffers(1, &m_hdrFBO);
	glDeleteRenderbuffers(1, &m_rbo);
}

// �J���[�o�b�t�@�E���P�x�o�b�t�@�ւ̏�������(��p�̃V�F�[�_�Ń��b�V���̑S�`����s��)
void RenderBloom::WriteBuffer(std::vector<class MeshComponent*> in_meshComp, std::vector<class SkeletalMeshComponent*> in_skelComp, CubeMapComponent* in_cubeMapComp, std::vector<class EnvironmentMapComponent*> in_envComp)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
	glEnable(GL_DEPTH_TEST);
	// �r���[�|�[�g����ʃT�C�Y�ɖ߂�
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());

	// ���C�g��Ԃ̊e�s����`
	Matrix4 lightSpace, lightView, lightProj;
	lightProj = Matrix4::CreateOrtho(7000.0f, 7000.0f, 1.0f, 5000.0f);
	lightView = Matrix4::CreateLookAt(RENDERER->GetDirectionalLight().position, RENDERER->GetDirectionalLight().target, Vector3::UnitZ);
	lightSpace = lightView * lightProj;
	// �ϊ��p�s��̎擾
	Matrix4 view = RENDERER->GetViewMatrix();
	Matrix4 projection = RENDERER->GetProjectionMatrix();

	// �V���h�E�V�F�[�_�̃A�N�e�B�u���Euniform�ւ̃Z�b�g
	m_multiRenderTargetShader->SetActive();
	m_multiRenderTargetShader->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_multiRenderTargetShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_multiRenderTargetShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_multiRenderTargetShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);

	m_multiRenderTargetShader->SetMatrixUniform("u_view", view);
	m_multiRenderTargetShader->SetMatrixUniform("u_projection", projection);
	m_multiRenderTargetShader->SetVectorUniform("u_viewPos", RENDERER->GetViewMatrix().GetTranslation());
	m_multiRenderTargetShader->SetMatrixUniform("u_lightSpaceMatrix", lightSpace);
	m_multiRenderTargetShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);

	// �T���v�����O�p�e�N�X�`���Z�b�g
	m_multiRenderTargetShader->SetInt("u_mat.diffuseMap", 0);
	m_multiRenderTargetShader->SetInt("u_mat.specularMap", 1);
	m_multiRenderTargetShader->SetInt("u_mat.normalMap", 2);
	m_multiRenderTargetShader->SetInt("u_mat.depthMap", 3);

	// �V���h�E�V�F�[�_�ɂ�郁�b�V���`��
	for (auto mesh : in_meshComp)
	{
		mesh->Draw(m_multiRenderTargetShader);
	}


	// �V���h�E�V�F�[�_�̃A�N�e�B�u���Euniform�ւ̃Z�b�g
	m_multiRenderSkinShader->SetActive();
	m_multiRenderSkinShader->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_multiRenderSkinShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_multiRenderSkinShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_multiRenderSkinShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);

	m_multiRenderSkinShader->SetMatrixUniform("u_view", view);
	m_multiRenderSkinShader->SetMatrixUniform("u_projection", projection);
	m_multiRenderSkinShader->SetMatrixUniform("u_lightSpaceMatrix", lightSpace);
	m_multiRenderSkinShader->SetVectorUniform("u_viewPos", RENDERER->GetViewMatrix().GetTranslation());
	m_multiRenderSkinShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);

	// �T���v�����O�p�e�N�X�`���Z�b�g
	m_multiRenderSkinShader->SetInt("u_mat.diffuseMap", 0);
	m_multiRenderSkinShader->SetInt("u_mat.specularMap", 1);
	m_multiRenderSkinShader->SetInt("u_mat.normalMap", 2);
	m_multiRenderSkinShader->SetInt("u_mat.depthMap", 3);

	// �V���h�E�V�F�[�_�ɂ�郁�b�V���`��
	for (auto skel : in_skelComp)
	{
		skel->Draw(m_multiRenderSkinShader);
	}

	// ���}�b�v
	m_multiRenderEnvironmentShader->SetActive();
	m_multiRenderEnvironmentShader->SetMatrixUniform("u_view", view);
	m_multiRenderEnvironmentShader->SetMatrixUniform("u_projection", projection);
	m_multiRenderEnvironmentShader->SetVectorUniform("u_viewPos", RENDERER->GetViewMatrix().GetTranslation());
	m_multiRenderEnvironmentShader->SetInt("u_skybox", 0);
	for (auto env : in_envComp)
	{
		env->DrawEnvironmentMap(m_multiRenderEnvironmentShader);
	}

	// �X�J�C�{�b�N�X
	m_multiRenderCubeMapShader->SetActive();
	// Uniform�ɍs����Z�b�g
	Matrix4 InvView = view;
	InvView.Invert();
	m_multiRenderCubeMapShader->SetMatrixUniform("u_view", InvView);
	m_multiRenderCubeMapShader->SetMatrixUniform("u_projection", projection);
	m_multiRenderCubeMapShader->SetInt("u_skybox", 0);
	in_cubeMapComp->Draw(m_multiRenderCubeMapShader);


}

// �p�[�e�B�N���̕`��
void RenderBloom::WriteBuffer(ParticleManager* in_particle)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
	glEnable(GL_DEPTH_TEST);

	in_particle->Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


// ���P�x�o�b�t�@���_�E���T���v�����O�v�Z���ĕ`�悷��
void RenderBloom::DrawDownSampling(unsigned int in_brightBuffer)
{
	glDisable(GL_DEPTH_TEST);

	unsigned int renderSource = in_brightBuffer;   // �_�E���T���v�����O�^�[�Q�b�g (���P�x�o�b�t�@)

	int reduceX = GAME_CONFIG->GetScreenWidth();
	int reduceY = GAME_CONFIG->GetScreenHeight();

	// �k���o�b�t�@�̊�ԖڂɃ_�E���T���v�����O���ʂ��o�͂���
	for (int i = 0; i < maxLevelNum; i++)
	{
		reduceX /= 2;
		reduceY /= 2;
		// �`��FBO�Ɋ�Ԃ��w��
		glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO[i * 2 + 1]);
		// Viewport�̒���
		glViewport(0, 0, reduceX, reduceY);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// �e�N�X�`���ɍ��P�x�o�b�t�@���Z�b�g���A�V�F�[�_���Ń_�E���T���v�����O�������ʂ��o�͂���
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderSource);
		m_downSamplingShader->SetActive();
		m_downSamplingShader->SetInt("u_screenTex", 0);

		// �`�悷��
		RENDERER->GetScreenVAO()->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		renderSource = m_blurBufferTex[i * 2 + 1];
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

// ���P�x�ʂ̃K�E�X�ڂ�������
void RenderBloom::DrawGaussBlur()
{
	const unsigned int sampleCount = 15;
	Vector3 offset[sampleCount];

	int reduceX = GAME_CONFIG->GetScreenWidth();
	int reduceY = GAME_CONFIG->GetScreenHeight();;
	float deviation = 2.0f;
	unsigned int renderSource = m_blurBufferTex[1];

	// �K�E�X���x������
	for (int i = 0; i < maxLevelNum; i++)
	{
		reduceX /= 2;
		reduceY /= 2;
		bool h = false;
		// horizontal 0 : ���� 1: �������� �ɃK�E�X�ڂ���������
		for (int horizontal = 0; horizontal < 2; horizontal++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO[i * 2 + horizontal]);
			{
				glViewport(0, 0, reduceX, reduceY);
				Vector2 dir;
				if (horizontal)
				{
					dir = Vector2(0.0f, 1.0f);
				}
				else
				{
					dir = Vector2(1.0f, 0.0f);
				}
				CalcGaussBlurParam(reduceX, reduceY, dir, deviation, offset);

				// �J���[�o�b�t�@�̃N���A
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, renderSource);
				m_gaussShader->SetActive();
				m_gaussShader->SetInt("u_blurSource", 0);
				m_gaussShader->SetInt("u_param.sampleCount", 15);

				// �K�E�X�V�F�[�_�[��offset���Z�b�g
				for (int i = 0; i < sampleCount; i++)
				{
					std::string s = "u_param.offset[" + std::to_string(i) + "]";
					m_gaussShader->SetVectorUniform(s.c_str(), offset[i]);
				}

				RENDERER->GetScreenVAO();
				glDrawArrays(GL_TRIANGLES, 0, 6);
				renderSource = m_blurBufferTex[i * 2 + horizontal];
			}
		}
		// �񐔂��Ƃɕ΍����グ��
		deviation += 1.5f;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// �r���[�|�[�g����ʃT�C�Y�ɖ߂�
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
}

// Bloom�������{������ʂƒʏ�`�挋�ʂ��������ĕ`��
void RenderBloom::DrawBlendBloom(unsigned int in_colorBuffer)
{

	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_hdrBloomShader->SetActive();
	m_hdrBloomShader->SetFloat("u_exposure", m_exposure);
	m_hdrBloomShader->SetFloat("u_gamma", m_gamma);
	m_hdrBloomShader->SetInt("u_scene", 0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, in_colorBuffer);

	for (unsigned int i = 0; i < 5; i++)
	{
		int num = i + 1;
		std::string s = "u_bloom" + std::to_string(num);
		m_hdrBloomShader->SetInt(s, num);
		glActiveTexture(GL_TEXTURE0 + num);
		glBindTexture(GL_TEXTURE_2D, m_blurBufferTex[i * 2 + 1]);
	}

	RENDERER->GetScreenVAO()->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);

}


// �K�E�X�ڂ����̏d�݂��Z�o���� (in_rho = �΍�)
float RenderBloom::GaussianDistribution(const Vector2& in_pos, float in_rho)
{
	return exp(-(in_pos.x * in_pos.x + in_pos.y * in_pos.y) / (2.0f * in_rho * in_rho));
}

void RenderBloom::CalcGaussBlurParam(int in_w, int in_h, Vector2 in_dir, float in_deviation, Vector3* in_offset)
{
	auto tu = 1.0f / float(in_w);
	auto tv = 1.0f / float(in_h);
	in_offset[0].z = GaussianDistribution(Vector2(0.0f, 0.0f), in_deviation);
	auto total_weight = in_offset[0].z;
	in_offset[0].x = 0.0f;
	in_offset[0].y = 0.0f;
	for (auto i = 1; i < 8; ++i)
	{
		int nextpos = (i - 1) * 2 + 1;
		in_offset[i].x = in_dir.x * tu * nextpos;
		in_offset[i].y = in_dir.y * tv * nextpos;
		in_offset[i].z = GaussianDistribution(in_dir * float(nextpos), in_deviation);
		total_weight += in_offset[i].z * 2.0f;
	}
	for (auto i = 0; i < 8; ++i)
	{
		in_offset[i].z /= total_weight;
	}
	for (auto i = 8; i < 15; ++i)
	{
		in_offset[i].x = -in_offset[i - 7].x;
		in_offset[i].y = -in_offset[i - 7].y;
		in_offset[i].z = in_offset[i - 7].z;
	}
}

/// <summary>
/// �t�F�[�h�A�E�g (��ʈÓ]����)
/// </summary>
/// <param name="in_val"> �t���[�����̌����� </param>
/// <param name="in_deltaTime"> �f���^�^�C�� </param>
/// <returns> ���S�ɈÓ]������true��Ԃ� </returns>
bool RenderBloom::FadeOut(float in_val, float in_deltaTime)
{
	m_exposure -= in_val * in_deltaTime;

	bool end = false;

	if (m_exposure <= 0.0f)
	{
		end = true;
		m_exposure = m_saveExposure;
	}

	return end;
}

/// <summary>
/// �t�F�[�h�C��
/// </summary>
/// <param name="in_val"> �t���[�����̏㏸�� </param>
/// <param name="in_deltaTime"> �f���^�^�C�� </param>
/// <returns>���S�ɔ��]������true��Ԃ�</returns>
bool RenderBloom::FadeIn(float in_val, float in_deltaTime)
{

	if (m_exposure == m_saveExposure)
	{
		m_exposure = 0.0f;
	}

	m_exposure += in_val * in_deltaTime;

	bool end = false;

	if (m_exposure >= m_saveExposure)
	{
		end = true;
		m_exposure = m_saveExposure;
	}

	return end;
}

bool RenderBloom::WhiteOut(float in_val, float in_deltaTime)
{
	m_exposure += in_val * in_deltaTime;

	bool end = false;

	if (m_exposure >= 500.0f)
	{
		end = true;
		m_exposure = m_saveExposure;
	}

	return end;
}

bool RenderBloom::WhiteIn(float in_val, float in_deltaTime)
{
	if (m_exposure == m_saveExposure)
	{
		m_exposure = 500.0f;
	}

	m_exposure -= in_val * in_deltaTime;

	bool end = false;

	if (m_exposure <= m_saveExposure)
	{
		end = true;
		m_exposure = m_saveExposure;
	}

	return end;
}

// �o�b�t�@�̐���
bool RenderBloom::CreateHDRFBO()
{
	//---------------------------------------------------------------------------------+
    //
    // �}���`�����_�[�^�[�Q�b�g�p�� "HDR�o�b�t�@" "���P�x�o�b�t�@" �̗p��
    //
    //---------------------------------------------------------------------------------+
	glGenFramebuffers(1, &m_hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
	// �J���[�o�b�t�@�̐����ƃA�^�b�`
	CreateColorBuffer(m_defaultBuffer, 0);
	CreateColorBuffer(m_brightBuffer, 1);
	// �����_�[�o�b�t�@����
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	// �t���[���o�b�t�@�ɃA�^�b�`
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	// GL�Ƀ}���`�����_�����O���s�����Ƃ𖾎�
	m_attachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_attachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, m_attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	// �o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

// �J���[�o�b�t�@�̐���
void RenderBloom::CreateColorBuffer(unsigned int& in_colorBuffer, const unsigned int in_attachNum)
{
	glGenTextures(1, &in_colorBuffer);
	glBindTexture(GL_TEXTURE_2D, in_colorBuffer);
	// �X�N���[���T�C�Y�̃e�N�X�`���𐶐� (���������_�^)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	// �e�N�X�`���t�B���^�����O�E���b�s���O�ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// �t���[���o�b�t�@�ɃA�^�b�`
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + in_attachNum, GL_TEXTURE_2D, in_colorBuffer, 0);

}

// �_�E���T���v�����O�E�ڂ����p��FBO���쐬���� (�쐣���u���[���t�B���^)
// �K�E�X�ڂ����ɂďc��2�����g�p����̂ŁA�i�K���Ƃ�2�����쐬
void RenderBloom::CreateBlurFBO()
{
	int w = GAME_CONFIG->GetScreenWidth();
	int h = GAME_CONFIG->GetScreenHeight();

	// FBO�ƃe�N�X�`��ID�̘g���m�ۂ���
	m_blurFBO.resize(maxLevelNum * 2);
	m_blurBufferTex.resize(maxLevelNum * 2);

	// �c���̃u���[���쐬����
	for (unsigned int i = 0; i < maxLevelNum; i++)
	{
		// �k���o�b�t�@�̕��ƍ�����ݒ�
		w /= 2;
		h /= 2;
		// j = 0 : ��, j = 1 : �c
		for (unsigned int j = 0; j < 2; j++)
		{
			glGenFramebuffers(1, &m_blurFBO[i * 2 + j]);
			glGenTextures(1, &m_blurBufferTex[i * 2 + j]);
			// FBO�ƃe�N�X�`�����o�C���h
			glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO[i * 2 + j]);
			glBindTexture(GL_TEXTURE_2D, m_blurBufferTex[i * 2 + j]);

			// �e�N�X�`���̍쐬
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
			// �e�N�X�`���t�B���^�����O�E���b�s���O�ݒ�
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// �t���[���o�b�t�@�Ƀe�N�X�`�����֘A�t��
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_blurBufferTex[i * 2 + j], 0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}
