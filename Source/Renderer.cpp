//-----------------------------------------------------------------------+
// �����_���[(�O���t�B�b�N�`��)�N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Renderer.h"
#include "GameMain.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_syswm.h"
#include "Texture.h"
#include "SpriteComponent.h"
#include "Mesh.h"
#include "MeshGpmesh.h"
#include "MeshObj.h"
#include "MeshComponent.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "CubeMapComponent.h"
#include "Animation.h"
#include "Shader.h"
#include "VertexArray.h"
#include "ParticleManager.h"
#include "UIScreen.h"
#include <algorithm>
#include <GL/glew.h>

// �R���X�g���N�^
Renderer::Renderer()
	:m_window(nullptr)
	,m_SDLRenderer(nullptr)
	,m_context(0)
	,m_spriteShader(nullptr)
	,m_meshShader(nullptr)
	,m_meshNormalShader(nullptr)
	,m_skinnedShader(nullptr)
	,m_skyboxShader(nullptr)
	,m_switchShader(0)
	,m_cameraPos(Vector3::Zero)
{
}

// �f�X�g���N�^
Renderer::~Renderer()
{
}

// �e�평�������� (GL SDL)
bool Renderer::Initialize(int in_screenW, int in_screenH, bool in_full)
{
	// ��ʃT�C�Y��ݒ�
	m_screenWidth = in_screenW;
	m_screenHeight = in_screenH;


	//-----------------------------------------------------------------+
    // OpenGL�̑����ݒ�
    //-----------------------------------------------------------------+
    // OpenGL�R�A�v���t�@�C�����g�p����
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// �w�� -> OpenGL/ver3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);     // ���W���[�o�[�W����
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);     // �}�C�i�[�o�[�W����
	// RGBA�e�`�����l����8bit�̃J���[�o�b�t�@���w�肷��
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);                  // R
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);                // G
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);                 // B
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);                // A
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// �_�u���o�b�t�@��L���ɂ���
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// �n�[�h�E�F�A�A�N�Z�����[�V�������g�p����
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// SDL�E�B���h�E�̍쐬
	m_window = SDL_CreateWindow
	(
		"FlameCarrier",                                 // �E�B���h�E�̖���
		0,                                              // x���W�̃E�B���h�E�`�挴�_
		0,                                              // y���W�̃E�B���h�E�`�挴�_
		m_screenWidth,              // ��ʂ̉���
		m_screenHeight,             // ��ʂ̏c��
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	// �E�B���h�E�̍쐬�Ɏ��s������
	if (!m_window)
	{
		printf("Create Window Failed : %s", SDL_GetError());
		return false;
	}
	printf("(%d, %d) Create Window\n", m_screenWidth, m_screenHeight);

	// �t���X�N���[����������
	if (in_full)
	{
		if (SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP))
		{
			printf("(%d x %d) Change FullScreen Failed\n", in_screenW, in_screenH);
			return false;
		}
	}
	glViewport(0, 0, m_screenWidth, m_screenHeight);

	// wminfo�\����
	SDL_SysWMinfo wminfo;
	SDL_VERSION(&wminfo.version);
	SDL_GetWindowWMInfo(m_window, &wminfo);
	m_hwnd = wminfo.info.win.window;
	
    //----------------------------------------------------------------+
    // OpenGL�R���e�L�X�g�ݒ�
    //----------------------------------------------------------------+
    // OpenGL�R���e�L�X�g�𐶐�
	m_context = SDL_GL_CreateContext(m_window);
	SDL_GL_MakeCurrent(m_window, m_context);

	//----------------------------------------------------------------+
    // GLEW������
    //----------------------------------------------------------------+
	glewExperimental = GL_TRUE;
	// �������Ɏ��s������
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("GLEW Initialize : Failed");
		return false;
	}

	//-----------------------------------------------------------------+
	// SDL�����_���[�̍쐬
	//-----------------------------------------------------------------+
	m_SDLRenderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_SDLRenderer)
	{
		printf("SDLRenderer�̍쐬�Ɏ��s : %s", SDL_GetError());
		return false;
	}
	printf("Create SDLRenderer : Success\n");

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("SDLImageInitPNG�������Ɏ��s : %s", SDL_GetError());
		return false;
	}
	printf("SDLImageInitPNG : Success\n");

	//--------------------------------------------------------------------+
    // Imgui������
    //--------------------------------------------------------------------+
	// imgui�R���e�L�X�g����
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// imgui�X�^�C���ݒ� (���e�[�})
	ImGui::StyleColorsDark();
	// �g�p���Ă���v���b�g�t�H�[���Ƀo�C���h
	ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
	ImGui_ImplOpenGL3_Init("#version 330");



	// GLEW�͖��Q�ȃG���[�R�[�h���o���̂ŃN���A���Ă���
	glGetError();

	// �V�F�[�_�[�v���O�����̏�����
	if (!LoadShaders())
	{
		printf("Shader Load : Failed");
		return false;
	}


	//------------------------------------------------------------------+
	// �p�[�e�B�N���֘A
	//------------------------------------------------------------------+
	// �p�[�e�B�N���p���_�쐬
	CreateParticleVerts();
	// �p�[�e�B�N���}�l�[�W���[�쐬
	m_particleManager = new ParticleManager;

	//------------------------------------------------------------------+
	// Sprite�֘A
	//------------------------------------------------------------------+
	CreateSpriteVerts();        // �X�v���C�g�p�̒��_�쐬

	//------------------------------------------------------------------+
	// �L���[�u�}�b�v�p���_�z��
	//------------------------------------------------------------------+
	CreateCubeMapVerts();

	// �J�����O
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	// �������ɐ���
	return true;
}

void Renderer::Delete()
{
	// �p�[�e�B�N���}�l�[�W���[�j��
	delete m_particleManager;

	// �e�N�X�`���̉��
	for (auto i : m_textures)
	{
		printf("Textures Release : %s\n", i.first.c_str());

		if (i.second != nullptr)
		{
			i.second->Delete();

			delete i.second;

			i.second = nullptr;
		}

		if (i.second == nullptr)
		{
			break;
		}

	}
	m_textures.clear();

	// ���b�V���̉��
	for (auto i : m_meshes)
	{
		printf("Meshes Release : %s\n", i.first.c_str());

		i.second->Delete();
		delete i.second;
	}
	m_meshes.clear();

	// �X�P���g���̔j��
	for (auto sk : m_skeletons)
	{
		delete sk.second;
	}
	// �A�j���[�V�����̔j��
	for (auto anim : m_animations)
	{
		delete anim.second;
	}

	// �V�F�[�_�[�̉��
	m_meshShader->Delete();

	// �X�v���C�g�̉��
	delete m_spriteVerts;
	m_spriteShader->Delete();
	delete m_spriteShader;
	delete m_skyboxVerts;
	m_skyboxShader->Delete();

	// �R���e�L�X�g�̔j��
	SDL_GL_DeleteContext(m_context);
	// �E�B���h�E�̔j��
	SDL_DestroyWindow(m_window);
}

// �`�揈��
void Renderer::Draw()
{

	// Set the clear color to light grey
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	//----------------------------------------------+
	// ���b�V���V�F�[�_�[(phong)
	//----------------------------------------------+
	if (m_switchShader == 0)
	{
		//���b�V���V�F�[�_�[�ŕ`�悷��Ώۂ̕ϐ����Z�b�g
		m_meshShader->SetActive();
		m_meshShader->SetMatrixUniform("uViewProj", m_view * m_projection);
		// ���C�e�B���O�ϐ����Z�b�g
		SetLightUniforms(m_meshShader);
		// �S�Ẵ��b�V���R���|�[�l���g��`��
		for (auto mc : m_meshComponents)
		{
			if (mc->GetVisible())
			{
				mc->Draw(m_meshShader);
			}
		}
	}

	//-----------------------------------------------+
	// ���b�V���V�F�[�_�[(normal)
	//-----------------------------------------------+
	if (m_switchShader == 1)
	{
		//���b�V���V�F�[�_�[�ŕ`�悷��Ώۂ̕ϐ����Z�b�g
		m_meshNormalShader->SetActive();
		m_meshNormalShader->SetMatrixUniform("uViewProj", m_view * m_projection);
		// ���C�e�B���O�ϐ����Z�b�g
		SetLightUniforms(m_meshNormalShader);
		m_meshNormalShader->SetVectorUniform("uLightPos", m_directionalLight.m_position);
		m_meshNormalShader->SetVectorUniform("uCameraPos", m_cameraPos);
		// �S�Ẵ��b�V���R���|�[�l���g��`��
		for (auto mc : m_meshComponents)
		{
			if (mc->GetVisible())
			{
				mc->Draw(m_meshNormalShader);
			}
		}
	}

	//-----------------------------------------------------------+
	// �{�[�����胁�b�V���̕`��
	//-----------------------------------------------------------+
	m_skinnedShader->SetActive();
	// �r���[�E�v���W�F�N�V�����̍����s����Z�b�g
	m_skinnedShader->SetMatrixUniform("uViewProj", m_view * m_projection);
	// ���C�e�B���O�ϐ����Z�b�g
	SetLightUniforms(m_skinnedShader);

	for (auto sk : m_skeletalMeshComponents)
	{
		if (sk->GetVisible())
		{
			sk->Draw(m_skinnedShader);
		}
	}

	
	//---------------------------------------------------------------+
	// �X�J�C�{�b�N�X�̕`��
	//---------------------------------------------------------------+
	glDepthFunc(GL_LEQUAL);
	// �V�F�[�_�[�A�N�e�B�u��
	m_skyboxShader->SetActive();
	m_skyboxVerts->SetActive();
	// ���s�ړ��v�f���폜�����r���[�s����V�F�[�_�[�ɃZ�b�g
	Matrix4 view = m_view;
	view.Invert();
	//Matrix4 view = m_view;
	m_skyboxShader->SetMatrixUniform("uViewMat", view);
	m_skyboxShader->SetMatrixUniform("uProjMat", m_projection);

	for (auto sky : m_skyBoxComponents)
	{
		sky->Draw(m_skyboxShader);
	}


	//----------------------------------------------------------------+
	// �p�[�e�B�N���`��
	//----------------------------------------------------------------+
	m_particleManager->Draw();

	// Sprite�̕`��
	// �[�x�e�X�g�̒�~
	glDisable(GL_DEPTH_TEST);
	// �u�����h�̃A�N�e�B�u��
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// sprite�V�F�[�_�[�̃A�N�e�B�u��
	m_spriteShader->SetActive();
	m_spriteVerts->SetActive();

	for (auto sprite : m_spriteComponents)
	{
		if (sprite->GetVisible())
		{
			sprite->Draw(m_spriteShader);
		}
	}

	// �S�Ă�UI���X�V
	for (auto ui : GAME_INSTANCE.GetUIStack())
	{
		ui->Draw(m_spriteShader);
	}



	// �f�o�b�O�R���\�[���`��
	//MOUSE_INSTANCE.ImGuiDebugRendering();
	// STEERING_CONTROLLER_INSTANCE.RenderDebugImGui();

	// ImGui�t���[�����J�n
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(GAME_INSTANCE.GetRenderer()->GetSDLWindow());
	ImGui::NewFrame();

	// ImGui�X�V
	ImGui::Begin("Debug Console : Mouse Input");
	ImGui::SliderInt("MeshShader", &m_switchShader, 0, 1);

	ImGui::End();
	ImGui::Render();
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	//glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Swap the buffers
	SDL_GL_SwapWindow(m_window);

}


void Renderer::AddSprite(SpriteComponent * in_sprite)
{
	int myDrawOrder = in_sprite->GetDrawOrder();
	auto iter = m_spriteComponents.begin();
	for (;
		iter != m_spriteComponents.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	m_spriteComponents.insert(iter, in_sprite);
}

void Renderer::RemoveSprite(SpriteComponent * in_sprite)
{
	auto iter = std::find(m_spriteComponents.begin(), m_spriteComponents.end(), in_sprite);
	m_spriteComponents.erase(iter);
}


// ���b�V���R���|�[�l���g�ǉ�
void Renderer::AddMeshComponent(MeshComponent * in_mesh)
{

	if (in_mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(in_mesh);
		m_skeletalMeshComponents.emplace_back(sk);
	}
	else
	{
		m_meshComponents.emplace_back(in_mesh);
	}

}

// ���b�V���R���|�[�l���g�i�[�z�񂩂�폜
void Renderer::RemoveMeshComponent(MeshComponent * in_mesh)
{

	if (in_mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(in_mesh);
		auto iter = std::find(m_skeletalMeshComponents.begin(), m_skeletalMeshComponents.end(), sk);
		m_skeletalMeshComponents.erase(iter);
	}
	else
	{
		auto iter = std::find(m_meshComponents.begin(), m_meshComponents.end(), in_mesh);
		m_meshComponents.erase(iter);
	}

}

void Renderer::ShowResource()
{
	printf("\n\n<------------------ textures ------------------>\n");
	for (auto i : m_textures)
	{
		printf("texfile %s\n", i.first.c_str());
	}

	printf("\n<------------------  meshes  ------------------->\n");
	for (auto i : m_meshes)
	{
		printf("meshfile %s\n", i.first.c_str());
	}
}

void Renderer::RemoveTexture(Texture* in_texture)
{
	// auto iter = std::find(m_textures.begin(), m_textures.end(), in_texture);

	for (auto iter : m_textures)
	{
		if (iter.second == in_texture)
		{
			m_textures.erase(iter.first);

			break;
		}
	}
}

// skybox�z��ɒǉ�����
void Renderer::AddSkyBox(CubeMapComponent* in_comp)
{
	m_skyBoxComponents.emplace_back(in_comp);
}

// skybox�z�񂩂�̍폜
void Renderer::RemoveSkyBox(CubeMapComponent* in_comp)
{
	auto iter = std::find(m_skyBoxComponents.begin(), m_skyBoxComponents.end(), in_comp);
	m_skyBoxComponents.erase(iter);
}

// skyboxVAO�̃o�C���h
void Renderer::SetCubeMapVAO()
{
	m_skyboxVerts->SetActive();
}



// �E�B���h�E�̃^�C�g��
void Renderer::SetWindowTitle(const std::string & in_title)
{
	SDL_SetWindowTitle(m_window, in_title.c_str());
}

// �p�[�e�B�N���̒��_�����o�C���h���A�N�e�B�u��
void Renderer::SetParticleVertex()
{
	m_particleVertex->SetActive();
}

// �f�B���N�V���i�����C�g�̃Z�b�g
void Renderer::SetDirectionalLight(const DirectionalLight & in_dirLight)
{
	m_directionalLight = in_dirLight;
}

// �e�N�X�`���̎擾
Texture * Renderer::GetTexture(const std::string & in_fileName)
{
	Texture* tex = nullptr;
	auto iter = m_textures.find(in_fileName);     // �����p�X�̃e�N�X�`�������邩������
	// �����e�N�X�`�����o�^���Ă�����
	if (iter != m_textures.end())
	{
		// �����e�N�X�`�����w��
		tex = iter->second;
	}
	else
	{
		// �Ȃ�������e�N�X�`���𐶐����A�z��֓����
		tex = new Texture();
		if (tex->Load(in_fileName))
		{
			m_textures.emplace(in_fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	printf("Load Texture Success : %s \n", in_fileName.c_str());
	return tex;
}

// ���b�V���̎擾
Mesh * Renderer::GetMesh(const std::string & in_fileName)
{
	// �����p�X�̃��b�V������������
	Mesh* mesh = nullptr;
	auto iter = m_meshes.find(in_fileName);
	// ���łɓ������b�V�����o�^���Ă������ꍇ�́A�v�f�̓��(���b�V���N���X)�𕡐�
	if (iter != m_meshes.end())
	{
		mesh = iter->second;
	}
	else
	{

		// �t�@�C���p�X�g���q�𒲂ׁA����ɉ����ă��b�V���𐶐�
		int extNum = in_fileName.find_last_of(".");     // "."�ȍ~�����������邩���ׂ�
		std::string fileExtension = in_fileName.substr(extNum, in_fileName.size() - extNum);    // ".#####"�Ƃ����`�Ŋg���q���擾

		if (fileExtension == ".gpmesh")       // gpmesh�`��
		{
			mesh = new MeshGpmesh();
			if (mesh->Load(in_fileName, GAME_INSTANCE.GetRenderer()))
			{
				m_meshes.emplace(in_fileName, mesh);
			}
			else
			{
				std::cerr << "ERROR: Category: " << "MeshComponent: gpmesh�t�@�C���̃��[�h���s" << std::endl;
				delete mesh;
				mesh = nullptr;
			}
		}
		else if (fileExtension == ".obj" || fileExtension == ".OBJ")     // obj�`��
		{
			mesh = new MeshObj();
			if (mesh->Load(in_fileName, GAME_INSTANCE.GetRenderer()))
			{
				m_meshes.emplace(in_fileName, mesh);
			}
			else
			{
				std::cerr << "ERROR: Category: " << "MeshComponent: obj�t�@�C���̃��[�h���s" << std::endl;
				delete mesh;
				mesh = nullptr;
			}
		}

	}

	printf("Mesh Load Success : %s\n", in_fileName.c_str());

	return mesh;
}

//-------------------------------------------------------------------------------------+
// �X�P���^�����̎擾
// in  : �X�P���^����� .gpskel�t�@�C����
// out : Skeleton���ւ̃|�C���^
// Desc: gpskel�t�@�C�������A�X�P���^������Ԃ��B�Ȃ��ꍇ�͂��̃t�@�C������
//       �ǂݍ��݂��s���B�ǂݍ��݂��s���Ă��t�@�C�������݂��Ȃ��ꍇ nullptr��Ԃ�
//       ������gpskel�t�@�C�������L�[�Ƃ���X�P���^������map���쐬�����
//-------------------------------------------------------------------------------------+
const Skeleton* Renderer::GetSkeleton(const char* in_fileName)
{
	std::string file(in_fileName);
	auto iter = m_skeletons.find(file);
	if (iter != m_skeletons.end())
	{
		return iter->second;
	}
	else
	{
		Skeleton* sk = new Skeleton();
		if (sk->Load(file))
		{
			m_skeletons.emplace(file, sk);
		}
		else
		{
			delete sk;
			sk = nullptr;
		}
		return sk;
	}
}

//-------------------------------------------------------------------------------------------+
// �A�j���[�V�������̎擾
// in  : �A�j���[�V�����f�[�^���i�[����Ă��� .gpanim�t�@�C����
// out : �A�j���[�V�������ւ̃|�C���^
// Desc: gpanim�t�@�C�������A�j���[�V�����f�[�^��Ԃ��B�Ȃ��ꍇ�͂��̃t�@�C������
//       �ǂݍ��݂��s���B�ǂݍ��݂��s���Ă��t�@�C�������݂��Ȃ��ꍇ nullptr��Ԃ�
//       ������gpanim�t�@�C�������L�[�Ƃ���A�j���[�V��������map���쐬�����
//-------------------------------------------------------------------------------------------+
const Animation* Renderer::GetAnimation(const char* in_fileName, bool in_loop)
{
	auto iter = m_animations.find(in_fileName);
	if (iter != m_animations.end())
	{
		return iter->second;
	}
	else
	{
		Animation* anim = new Animation();
		if (anim->Load(in_fileName, in_loop))
		{
			m_animations.emplace(in_fileName, anim);
		}
		else
		{
			delete anim;
			anim = nullptr;
		}
		return anim;
	}
}

// �p�[�e�B�N���}�l�[�W���[�擾
ParticleManager * Renderer::GetParticleManager() const
{
	return m_particleManager;
}

// �X�J�C�{�b�N�X�p���_�z���`
void Renderer::CreateCubeMapVerts()
{
	float skyboxVertices[] = 
	{
      
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int indices[] = 
	{
		0, 1, 2, 0, 2, 3,    // �O��
		4, 5, 6, 4, 6, 7,    // �w��
		8, 9, 10, 8, 10, 11,   // ���
		12, 13, 14, 12, 14, 15,   // ���
		16, 17, 18, 16, 18, 19,   // �E����
		20, 21, 22, 20, 22, 23    // ������
	};




	// VAO, VBO�쐬
	m_skyboxVerts = new VertexArray(skyboxVertices, 36);
}

// �X�v���C�g(2D�p)�̒��_�z��𐶐�
void Renderer::CreateSpriteVerts()
{
	float vertices[] = 
	{
      // x     y     z    nx   ny   nz    u    v
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // ���㒸�_
		 0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // �E�㒸�_
		 0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // �E�����_
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // �������_

	};

	unsigned int indices[] = 
	{
	    0, 2, 1,
	    2, 0, 3
	};

	m_spriteVerts = new VertexArray(vertices, 4, VertexArray::POS_NORMAL_TEX, indices, 6);
}

// �p�[�e�B�N���p�̒��_�z��𐶐�
void Renderer::CreateParticleVerts()
{
	float vertices[] = 
	{
		-0.5f, 0.f, 0.5f, 0.f, 0.f, 0.0f, 0.f, 0.f, // ���㒸�_
		 0.5f, 0.f, 0.5f, 0.f, 0.f, 0.0f, 1.f, 0.f, // �E�㒸�_
		 0.5f, 0.f,-0.5f, 0.f, 0.f, 0.0f, 1.f, 1.f, // �E�����_
		-0.5f, 0.f,-0.5f, 0.f, 0.f, 0.0f, 0.f, 1.f  // �������_
	};

	unsigned int indices[] = 
	{
		0, 2, 1,
		2, 0, 3
	};
	m_particleVertex = new VertexArray(vertices, 4, VertexArray::POS_NORMAL_TEX, indices, 6);
}

// �V�F�[�_�[�̃��[�h
bool Renderer::LoadShaders()
{

	// �X�v���C�g�V�F�[�_�[
	m_spriteShader = new Shader();

	if (!m_spriteShader->Load("Data/Shaders/Sprite.vert", "Data/Shaders/Sprite.frag"))
	{
		return false;
	}

	m_spriteShader->SetActive();
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(m_screenWidth, m_screenHeight);
	m_spriteShader->SetMatrixUniform("uViewProj", viewProj);



	// ���b�V���V�F�[�_�[
	m_meshShader = new Shader();

	m_view = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	m_projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		static_cast<float>(m_screenWidth),
		static_cast<float>(m_screenHeight),
		1.0f, 10000.0f);

	if (!m_meshShader->Load("Data/Shaders/Phong.vert", "Data/Shaders/Phong.frag"))
	{
		return false;
	}

	m_meshShader->SetActive();
	m_meshShader->SetMatrixUniform("uViewProj", m_view * m_projection);

	// ���b�V���V�F�[�_�[(�@���}�b�v)
	m_meshNormalShader = new Shader();
	if (!m_meshNormalShader->Load("Data/Shaders/NormalMap.vert", "Data/Shaders/NormalMap.frag"))
	{
		return false;
	}
	m_meshNormalShader->SetActive();
	m_meshNormalShader->SetMatrixUniform("uViewProj", m_view * m_projection);
	m_meshNormalShader->SetVectorUniform("uLightPos", m_directionalLight.m_position);
	m_meshNormalShader->SetVectorUniform("uCameraPos", m_cameraPos);

	// �X�L���V�F�[�_�[
	m_skinnedShader = new Shader();
	if (!m_skinnedShader->Load("Data/Shaders/Skinned.vert", "Data/Shaders/Phong.frag"))
	{
		return false;
	}
	m_skinnedShader->SetActive();
	m_skinnedShader->SetMatrixUniform("uViewProj", m_view * m_projection);

	// �X�J�C�{�b�N�X�p�V�F�[�_�[
		// �X�L���V�F�[�_�[
	m_skyboxShader = new Shader();
	if (!m_skyboxShader->Load("Data/Shaders/SkyBox.vert", "Data/Shaders/SkyBox.frag"))
	{
		return false;
	}


	return true;
}

// �V�F�[�_�[��uniform�ɒl���Z�b�g
void Renderer::SetLightUniforms(Shader * in_shader)
{
	// �r���[�s�񂩂�J�����ʒu���t�Z�o����
	Matrix4 invView = m_view;
	invView.Invert();
	in_shader->SetVectorUniform("uCameraPos", invView.GetTranslation());

	// �A���r�G���g���C�g
	in_shader->SetVectorUniform("uAmbientLight", m_ambientLight);

	// �f�B���N�V���i�����C�g
	in_shader->SetVectorUniform("uDirLight.mPosition", m_directionalLight.m_direction);
	in_shader->SetVectorUniform("uDirLight.mDirection", m_directionalLight.m_direction);
	in_shader->SetVectorUniform("uDirLight.mDiffuseColor", m_directionalLight.m_diffuseColor);
	in_shader->SetVectorUniform("uDirLight.mSpecColor", m_directionalLight.m_specColor);
}
