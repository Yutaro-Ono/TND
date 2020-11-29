//----------------------------------------------------------------------------------------------+
// �����_���[�N���X
// �@�\�F�`��Ɋւ��S�Ă̏���ۗL����B�`���Forward��Deffered�����_���[�N���X���g�p����
// �����F�Ȃ�
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//----------------------------------------------------------------------------------------------+
#pragma once
#include <Windows.h>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_syswm.h>
#include <GL/glew.h>
#include "Math.h"
#include "imgui/imconfig.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

// �f�B���N�V���i�����C�g�\����(���s���C�g:�V�[���S�̂ɍ~�蒍����)
typedef struct DirectionalLight
{
	Vector3 position;      // �����ʒu
	Vector3 target;        // �^�[�Q�b�g(����������o���₷�����邽��)
	Vector3 direction;     // �^�[�Q�b�g - �����ʒu
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;

}DirectionalLight;


class Renderer
{

public:

	enum RENDER_MODE
	{
		FORWARD = 0,
		DEFFERED
	};

	Renderer();
	~Renderer();

	bool Initialize(int in_screenW, int in_screenH, bool in_full);          // �e�평�������� (GL SDL)
	void Delete();                                                          // �e��������
	void Draw();                                                            // �`��

	// SpriteComponent
	void AddSprite(class SpriteComponent* in_sprite);
	void RemoveSprite(class SpriteComponent* in_sprite);
	// Sprite (���[���h���)
	void AddSpriteWorld(class WorldSpaceUI* in_sprite);
	void RemoveSpriteWorld(class WorldSpaceUI* in_sprite);
	// MeshComponent
	void AddMeshComponent(class MeshComponent* in_mesh);                    // ���b�V���R���|�[�l���g�̒ǉ�
	void RemoveMeshComponent(class MeshComponent* in_mesh);                 // ���b�V���R���|�[�l���g�̍폜
	void ShowResource();
	// EnvironmentMapComponent
	void AddEnvironmentComponent(class EnvironmentMapComponent* in_envMesh);
	void RemoveEnvironmentComponent(class EnvironmentMapComponent* in_envMesh);
	// PointLightComponent
	void AddPointLightComponent(class PointLightComponent* in_pointL);
	void RemovePointLightComponent(class PointLightComponent* in_pointL);

	// TextureComponent
	void RemoveTexture(class Texture* in_texture);

	// ShadowMap
	class ShadowMap* GetShadowMap() { return m_shadowMap; }

	void WindowClear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
	void WindowFlip() { SDL_GL_SwapWindow(m_window); }

	void SetWindowTitle(const std::string& in_title);
	void SetWorldSpriteVertex();
	void SetDirectionalLight(const DirectionalLight& in_dirLight);

	//--------------------------------------------+
	// Getter / Setter
	//--------------------------------------------+
	SDL_Renderer* GetSDLRenderer() { return m_SDLRenderer; }                // SDL�Y�̕`��ɗp����SDLRenderer�̃Q�b�^�[
	SDL_Window* GetSDLWindow() { return m_window; }                         // SDL Window�̃Q�b�^�[
	SDL_GLContext GetSDLGLContext() { return m_context; }                   // SDL_GL�R���e�L�X�g�̃Q�b�^�[
	HWND GetHWND() { return m_hwnd; }                                       // HWND�̃Q�b�^�[ (�n���h���R���g���[���̏������Ɏg�p)
	
	class Texture* GetTexture(const std::string& in_fileName);              // �w�肵���t�@�C�����̃e�N�X�`����Ԃ�
	class Mesh* GetMesh(const  std::string& in_fileName);                   // �w�肵���t�@�C�����̃��b�V����Ԃ�
	const class Skeleton* GetSkeleton(const std::string& in_fileName);             // �w�肵���t�@�C�����̃X�P���^�����b�V���̎擾
	const class Animation* GetAnimation(const char* in_fileName, bool in_loop);
	// �X�N���[���T�C�Y
	float GetScreenWidth() { return static_cast<float>(m_screenWidth); }
	float GetScreenHeight() { return static_cast<float>(m_screenHeight); }
	// �f�B���N�V���i�����C�g
	DirectionalLight& GetDirectionalLight() { return m_directionalLight; }
	// �J�����ʒu�Z�b�g
	void SetCameraPosition(const Vector3& in_cameraPos) { m_cameraPos = in_cameraPos; }
	// �s��
	const Matrix4& GetViewMatrix() { return m_view; }
	const Matrix4& GetProjectionMatrix() { return m_projection; }
	// ParticleManager
	class ParticleManager* GetParticleManager() const;

	void SetViewMatrix(const Matrix4& in_view) { m_view = in_view; }
	void SetAmbientLight(const Vector3& in_ambientColor) { m_ambientLight = in_ambientColor; }
	const Vector3 GetAmbientLight() { return m_ambientLight; }
	void SetProjectionMatrix(const Matrix4& in_proj) { m_projection = in_proj; }

	// �A�N�e�B�u�X�J�C�{�b�N�X
	void SetActiveSkyBox(class CubeMapComponent* in_skyBox) { m_activeSkyBox = in_skyBox; }
	class CubeMapComponent* GetSkyBox() { return m_activeSkyBox; }
	class VertexArray* GetCubeMapVerts() { return m_cubeVerts; }

	// �X�N���[���p���_�z��N���X�̃Q�b�^�[
	class VertexArray* GetScreenVAO() { return m_screenVerts; }


private:

	void CreateCubeVerts();                                                // �L���[�u�}�b�v(�X�J�C�{�b�N�X�p)���_�z���`
	void CreateSpriteVerts();                                              // �X�v���C�g�p���_�z���`
	void CreateWorldSpriteVerts();                                         // �p�[�e�B�N���p���_�z���`
	void CreateScreenVerts();                                              // �X�N���[���p�l�p�`���_�z��

	bool LoadShaders();                                                    // �V�F�[�_�[�̏�����
	void SetLightUniforms(class Shader* in_shader);                        // ���C�g�l���V�F�[�_�[�ɃZ�b�g

	int m_screenWidth, m_screenHeight;                                     // �E�B���h�E�̉����A�c��
	bool m_fullScreen;                                                     // �t���X�N���[�����ǂ���

	std::unordered_map<std::string, class Texture*> m_textures;            // �e�N�X�`���o�^�z��
	std::unordered_map<std::string, class Mesh*> m_meshes;                 // ���b�V���o�^�z��
	
	// �X�P���g���E�A�j���[�V�����̓o�^�p�z��(�X�P���^�����b�V���̂�)
	std::unordered_map<std::string, class Skeleton*> m_skeletons;
	std::unordered_map<std::string, class Animation*> m_animations;

	// ���b�V���z��
	std::vector<class MeshComponent*> m_meshComponents;                       // ���b�V���R���|�[�l���g
	std::vector<class SkeletalMeshComponent*> m_skeletalMeshComponents;       // �{�[�����胁�b�V���z��
	std::vector<class MeshComponent*> m_glassMeshComponent;                   // �K���X(���ɉe�����󂯂�)���b�V���z��
	std::vector<class EnvironmentMapComponent*> m_envMeshComponents;          // ���}�b�v�I�u�W�F�N�g�z��
	// �X�v���C�g�z��
	std::vector<class SpriteComponent*> m_spriteComponents;                // �X�v���C�g�z�� (�X�N���[�����)
	std::vector<class WorldSpaceUI*> m_worldSprites;                       // �X�v���C�g�z�� (���[���h���)
	
	class CubeMapComponent* m_activeSkyBox;                                // �L����(�`�悷��)�X�J�C�{�b�N�X

	// ���C�g�֘A�z��
	std::vector<class PointLightComponent*> m_pointLights;                           // �|�C���g���C�g�z��



	//--------------------------------------------+
	// �V�F�[�_�I�u�W�F�N�g
	//--------------------------------------------+
	class Shader* m_meshShader;                                            // �W�����b�V���V�F�[�_
	class Shader* m_meshNormalShader;                                      // �@���}�b�v���b�V���V�F�[�_
	class Shader* m_skinnedShader;                                         // �X�L��(�{�[������)���b�V���V�F�[�_
	class Shader* m_environmentMapShader;                                  // ���}�b�v�V�F�[�_

	class Shader* m_spriteShader;                                          // �X�v���C�g��{�V�F�[�_
	class Shader* m_worldSpaceSpriteShader;                                // ���[���h��ԏ�̃X�v���C�g�V�F�[�_
	class Shader* m_particleShader;
	class Shader* m_skyboxShader;                                          // �X�J�C�{�b�N�X�p�V�F�[�_

	//--------------------------------------------+
	// ���_�z��
	//--------------------------------------------+
	class VertexArray* m_spriteVerts;                                      // �X�v���C�g�p���_�z��
	class VertexArray* m_particleVerts;                                    // �p�[�e�B�N���p���_�z��
	class VertexArray* m_cubeVerts;                                        // �L���[�u���_�z��
	class VertexArray* m_screenVerts;                                      // �X�N���[���S�̂𕢂��l�p�`

	//--------------------------------------------+
	// ���C�e�B���O�֘A
	//--------------------------------------------+
	Vector3 m_ambientLight;                                                // �A���r�G���g���C�g
	DirectionalLight m_directionalLight;                                   // �f�B���N�V���i�����C�g�\����

	//--------------------------------------------+
	// �V���h�E�֘A
	//--------------------------------------------+
	class ShadowMap* m_shadowMap;

	//--------------------------------------------+
    // HDR�EBloom�֘A
    //--------------------------------------------+
	class RenderBloom* m_bloom;
	int m_switchShader;

	//--------------------------------------------+
	// �p�[�e�B�N���֘A
	//--------------------------------------------+
	class ParticleManager* m_particleManager;

	//--------------------------------------------+
	// �|�X�g�G�t�F�N�g�֘A
	//--------------------------------------------+
	class FrameBuffer* m_frameBuffer;

	//--------------------------------------------+
    // �J�����֘A
    //--------------------------------------------+
	Vector3 m_cameraPos;

	//--------------------------------------------+
	// ��{�s��֘A
	//--------------------------------------------+
	Matrix4 m_view;
	Matrix4 m_projection;       // �v���W�F�N�V����(������)
	//AABB m_projCullingZone;   // ������͈͊O���ǂ��������o����

	//--------------------------------------------+
	// �A�v���P�[�V�������֘A
	//--------------------------------------------+
	SDL_Window* m_window;                                                   // SDL�̃E�B���h�E�n���h��
	SDL_Renderer* m_SDLRenderer;                                            // SDL�̃����_�����O�n���h��
	SDL_GLContext m_context;                                                // OpenGL�R���e�L�X�g (������ԊǗ�)
	HWND m_hwnd;                                                            // �E�B���h�E�n���h��  

	// �t�H���[�h�E�f�B�t�@�[�h�����_�����O�N���X (�`��͂ǂ��炩�ōs��)
	class ForwardRenderer* m_fRenderer;
	class DefferedRenderer* m_dRenderer;
	RENDER_MODE m_renderMode;             // �����_�����O���[�h (�t�H���[�h���f�B�t�@�[�h��)

	// �t�����h�N���X (�t�H���[�h�E�f�B�t�@�[�h�N���X�̓����_���[�ɒ��ڃA�N�Z�X�\)
	friend class ForwardRenderer;
	friend class DefferedRenderer;
};