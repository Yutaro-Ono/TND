//-----------------------------------------------------------------------+
// �����_���[�N���X
// �@�\�F�`��Ɋւ��S�Ă̏���ۗL���A�`�揈�����s��
// �����F�Ȃ�
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <string>
#include <Windows.h>
#include <unordered_map>
#include <SDL.h>
#include <GL/glew.h>
#include "Math.h"
#include "imgui/imconfig.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

// �f�B���N�V���i�����C�g�\����(���s���C�g:�V�[���S�̂ɍ~�蒍����)
typedef struct DirectionalLight
{
	Vector3 position;
	Vector3 direction;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;

}DirectionalLight;


class Renderer
{

public:

	Renderer();
	~Renderer();

	bool Initialize(int in_screenW, int in_screenH, bool in_full);          // �e�평�������� (GL SDL)
	void Delete();                                                          // �e��������
	void Draw();                                                            // �`��

	// SpriteComponent
	void AddSprite(class SpriteComponent* in_sprite);
	void RemoveSprite(class SpriteComponent* in_sprite);
	const std::vector<class SpriteComponent*>& GetSpriteStack() const { return m_spriteComponents; }
	// Sprite (���[���h���)
	void AddSpriteWorld(class WorldSpaceUI* in_sprite);
	void RemoveSpriteWorld(class WorldSpaceUI* in_sprite);

	// MeshComponent
	void AddMeshComponent(class MeshComponent* in_mesh);                    // ���b�V���R���|�[�l���g�̒ǉ�
	void RemoveMeshComponent(class MeshComponent* in_mesh);                 // ���b�V���R���|�[�l���g�̍폜
	void ShowResource();

	// TextureComponent
	void RemoveTexture(class Texture* in_texture);

	// ShadowMap
	class ShadowMap* GetShadowMap() { return m_shadowMap; }

	// SkyBox
	void AddSkyBox(class CubeMapComponent* in_comp);
	void RemoveSkyBox(class CubeMapComponent* in_comp);

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
	void SetProjectionMatrix(const Matrix4& in_proj) { m_projection = in_proj; }


private:

	void CreateCubeMapVerts();                                             // �L���[�u�}�b�v(�X�J�C�{�b�N�X�p)���_�z���`
	void CreateSpriteVerts();                                              // �X�v���C�g�p���_�z���`
	void CreateWorldSpriteVerts();                                            // �p�[�e�B�N���p���_�z���`
	bool LoadShaders();                                                    // �V�F�[�_�[�̏�����
	void SetLightUniforms(class Shader* in_shader);                        // ���C�g�l���V�F�[�_�[�ɃZ�b�g

	int m_screenWidth, m_screenHeight;                                     // �E�B���h�E�̉����A�c��
	bool m_fullScreen;                                                     // �t���X�N���[�����ǂ���

	std::unordered_map<std::string, class Texture*> m_textures;            // �e�N�X�`���o�^�z��
	std::unordered_map<std::string, class Mesh*> m_meshes;                 // ���b�V���o�^�z��
	
	// �X�P���g���E�A�j���[�V�����̓o�^�p�z��(�X�P���^�����b�V���̂�)
	std::unordered_map<std::string, class Skeleton*> m_skeletons;
	std::unordered_map<std::string, class Animation*> m_animations;

	std::vector<class MeshComponent*> m_meshComponents;                    // ���b�V���R���|�[�l���g
	std::vector<class CubeMapComponent*> m_skyBoxComponents;                  // �X�J�C�{�b�N�X�p���b�V���R���|�[�l���g
	std::vector<class SkeletalMeshComponent*> m_skeletalMeshComponents;

	//--------------------------------------------+
	// ���C�e�B���O�֘A
	//--------------------------------------------+
	Vector3 m_ambientLight;                                                // �A���r�G���g���C�g
	DirectionalLight m_directionalLight;                                   // �f�B���N�V���i�����C�g

	//--------------------------------------------+
	// �V���h�E�֘A
	//--------------------------------------------+
	class ShadowMap* m_shadowMap;


	//--------------------------------------------+
	// Sprite�֘A
	//--------------------------------------------+
	class VertexArray* m_spriteVerts;                                      // �X�v���C�g�p���_�z��
	class Shader* m_spriteShader;                                          // �X�v���C�g��{�V�F�[�_
	class Shader* m_worldSpaceSpriteShader;                                // ���[���h��ԏ�̃X�v���C�g�V�F�[�_
	std::vector<class SpriteComponent*> m_spriteComponents;                // �X�v���C�g�z�� (�X�N���[�����)
	std::vector<class WorldSpaceUI*> m_worldSprites;                       // �X�v���C�g�z�� (���[���h���)

	//--------------------------------------------+
	// ���b�V���֘A
	//--------------------------------------------+
	class Shader* m_meshShader;
	class Shader* m_meshNormalShader;
	class Shader* m_skinnedShader;
	int m_switchShader;

	//--------------------------------------------+
	// �p�[�e�B�N���֘A
	//--------------------------------------------+
	class Shader* m_particleShader;
	class VertexArray* m_particleVertex;
	class ParticleManager* m_particleManager;

	//--------------------------------------------+
	// �X�J�C�{�b�N�X�֘A
	//--------------------------------------------+
	class Shader* m_skyboxShader;          // �X�J�C�{�b�N�X�p�V�F�[�_�[
	class VertexArray* m_skyboxVerts;      // �X�J�C�{�b�N�X�p���_�z��

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
	Matrix4 m_projection;     // �v���W�F�N�V����(������)
	//AABB m_projCullingZone;   // ������͈͊O���ǂ��������o����

	//--------------------------------------------+
	// �����_�����O�x�[�X���֘A
	//--------------------------------------------+
	SDL_Window* m_window;                                                   // SDL�̃E�B���h�E�n���h��
	SDL_Renderer* m_SDLRenderer;                                            // SDL�̃����_�����O�n���h��
	SDL_GLContext m_context;                                                // OpenGL�R���e�L�X�g (������ԊǗ�)
	HWND m_hwnd;                                                            // �E�B���h�E�n���h��  

};