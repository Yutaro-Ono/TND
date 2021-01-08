//-----------------------------------------------------------------------+
// �Q�[�����[�v.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+

// �C���N���[�h�t�@�C��
#include "GameMain.h"
#include "SceneBase.h"
#include "Camera.h"
#include "Actor.h"
#include "Camera.h"
#include "CameraComponent.h"
#include "PhysicsWorld.h"
#include "Collision.h"
#include "DebugBox.h"
#include "ParticleManager.h"
#include "AudioManager.h"
#include "UIScreen.h"
#include "Font.h"
#include "PauseScreen.h"
#include "LoadScreen.h"
#include "glad/glad.h"
#include <algorithm>
#include <utility>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>

// �W���t�H���g
const std::string GameMain::FONT_FILE_PATH = "Data/Fonts/SackersGothicLightAT.ttf";

// �R���X�g���N�^
GameMain::GameMain()
	:m_state(PLAYING)
	,m_renderer(nullptr)
	,m_debugWindow(nullptr)
	,m_debugRenderer(nullptr)
	,m_audio(nullptr)
	,m_config(nullptr)
	,m_isRunning(true)
	,m_isPauseMode(false)
	,m_activeCamera(nullptr)
	,m_nowScene(nullptr)
	,m_physicsWorld(nullptr)
	,m_pause(nullptr)
	,m_load(nullptr)
{
	// �����Ȃ�
}

// �f�X�g���N�^
GameMain::~GameMain()
{
	// �����Ȃ�
}

// �e�평��������(SDL, Renderer)
// in_full -> �t���X�N���[�����ǂ���
bool GameMain::Initialize()
{
	//--------------------------------------------------------------------+
	// SDL
	//--------------------------------------------------------------------+
	// SDL�̏�����
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		SDL_Log("SDL Initialize Failed : %s\n", SDL_GetError());
		return false;
	}

	//--------------------------------------------------------------------+
    // �R���t�B�O
    //--------------------------------------------------------------------+
    // �R���t�B�O�̐����E���[�h
	m_config = new GameConfig();
	m_config->LoadConfig();

	//--------------------------------------------------------------------+
	// �����_���[
	//--------------------------------------------------------------------+
	// �����_���[�̐���
	m_renderer = new Renderer();
	// �����_���[�̏�����
	if (!m_renderer->Initialize(GetConfig()->GetScreenWidth(), GetConfig()->GetScreenHeight(), GetConfig()->GetFullScreen()))
	{
		SDL_Log("Renderer Initialize Failed : %s\n", SDL_GetError());
		delete m_renderer;
		return false;
	}

	//--------------------------------------------------------------------+
	// �t�H���g(TTF)�����_�����O�V�X�e��������
	//--------------------------------------------------------------------+
	if (TTF_Init() != 0)
	{
		SDL_Log("SDL_ttf Initialize Failed : %s\n", SDL_GetError());
		return false;
	}

	//--------------------------------------------------------------------+
	// �T�E���h�V�X�e��������
	//--------------------------------------------------------------------+
	if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
	{
		delete m_renderer;
		return false;
	}
	printf("SDLMixer����������\n");
	// SDLMixer API�������@44100:�����̎��g��, 44.1kHz 2:�X�e���I 4096:�����o�b�t�@�T�C�Y
	// ���ʉ���MP3�g�p�s��
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		//delete m_renderer;
		//Mix_CloseAudio();
		//Mix_Quit();
		//return false;
	}
	int decordNum = Mix_GetNumMusicDecoders();
	for (int i = 0; i < decordNum; ++i)
	{
		printf("MusicDecorder %d : %s\n", i, Mix_GetMusicDecoder(i));
	}
	int chunkNum = Mix_GetNumChunkDecoders();
	for (int i = 0; i < chunkNum; ++i)
	{
		printf("SoundDecorder %d : %s\n", i, Mix_GetChunkDecoder(i));
	}
	printf("SDLMixerAPI����������\n");

	m_audio = new AudioManager();

	// �����蔻��V�X�e������
	m_physicsWorld = new PhysicsWorld;

	// �f�o�b�O�p�{�b�N�X
	m_debugBox = new DebugBox;

	// ���̓V�X�e��(�R���g���[���[)������
	if (!CONTROLLER_INSTANCE.Initialize())
	{
		return false;
	}

	// ���̓V�X�e��(�X�e�A�����O)������
	if (!STEERING_CONTROLLER_INSTANCE.Initialize())
	{
		return false;
	}

	// �t���[��������
	m_frame = 0;

	// SDL������������Ă���o�߂�������(�~���b�P��)
	m_ticksCount = SDL_GetTicks();

	// �t�H���g����
	Font* font = new Font();
	font->Load(FONT_FILE_PATH);
	m_fonts.emplace(FONT_FILE_PATH, font);

	// �|�[�Y��ʐ���
	m_pause = new PauseScreen();


	// ���[�h��ʐ���
	m_load = new LoadScreen();
	m_load->Initialize();


	return true;
}

// �e��������
void GameMain::Delete()
{

	// �A�N�^�[�̍폜�@�i�A�N�^�[��ʂ��ăR���|�[�l���g���폜�����j
	while (!m_actors.empty())
	{
		delete m_actors.back();
	}
	while (!m_pendingActors.empty())
	{
		delete m_pendingActors.back();
	}
	if (m_renderer)
	{
		m_renderer->Delete();
	}

	// UI�폜
	while (!m_uiStack.empty())
	{
		delete m_uiStack.back();
		m_uiStack.pop_back();
	}

	// �I�[�f�B�I�̍폜
	m_audio->ShutDown();
	delete m_audio;
	Mix_CloseAudio();
	Mix_Quit();

	delete m_physicsWorld;
	delete m_debugBox;

	// ���͊֘A�̍폜
	CONTROLLER_INSTANCE.Delete();
	STEERING_CONTROLLER_INSTANCE.Delete();

	// Imgui�̍폜
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	// SDL�E�B���h�E�̔j��(�f�o�b�O�p)
	SDL_DestroyWindow(m_debugWindow);
	// SDL�����_���[�̔j��(�f�o�b�O�p)
	SDL_DestroyRenderer(m_debugRenderer);

	SDL_Quit();


}

// ���C�����[�v
void GameMain::RunLoop()
{
	// �J�n�V�[������`����Ă��Ȃ�������I��
	if (!m_nowScene)
	{
		printf("�J�n�V�[�����ݒ肳��Ă��܂���");
		return;
	}

	// �Q�[�����[�v
	// m_isRunning��false�Ȃ烋�[�v�𔲂��I��
	while (m_isRunning)
	{
		Input();
		// LoopHelper();

		if (UpdateGame())
		{
			continue;
		}

		Draw();
		// DebugRenderer();
	}
}

// �Q�[���̍X�V����
int GameMain::UpdateGame()
{
	// 16�~���b(= 60�t���[��/�b)�ɂȂ�悤�ɁA�O�̃t���[������16�~���b�ȏ�o�߂���܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksCount + 16))
		;

	// �t���[���X�V
	m_frame++;

	// �O�̃t���[�����猻�ݎ����܂ł̌o�ߎ��ԎZ�o(�b�P�ʁj
	m_deltaTime = (SDL_GetTicks() - m_ticksCount + 16) / 1000.0f;

	//���̃t���[���̊J�n���ԕۑ��i���̃t���[���܂ł̌o�ߎ��Ԃ��͂��邽�߁j
	m_ticksCount = SDL_GetTicks();

	// �t���[�����Ԃ����܂�ɂ��o�߂��Ă���ꍇ��0.05 �� 20fps�Œ�
	if (m_deltaTime > 0.05f)
	{
		m_deltaTime = 0.05f;
	}

	if (!m_nowScene)
	{
		return 0;
	}

	// ���t���[���A�V�[������ۑ�
	SceneBase* tmpScene;

	// �A�N�^�[�̏���
	UpdateActor();

	// �J�����X�V
	if (m_activeCamera != nullptr)
	{
		// �J�����s��v�Z
		m_activeCamera->Update(m_deltaTime);
	}

	// �����蔻��
	m_physicsWorld->Collision();

	// �p�[�e�B�N���X�V
	m_renderer->GetParticleManager()->Update(m_deltaTime);

	//�V�[�������s
	tmpScene = m_nowScene->Update();

	// UI��CLOSE�Ȃ����
	auto iter = m_uiStack.begin();
	while (iter != m_uiStack.end())
	{
		if ((*iter)->GetState() == UIScreen::CLOSE)
		{
			delete *iter;
			iter = m_uiStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	// UI�̍X�V(CLOSE�Ȃ����)
	for (auto ui : m_uiStack)
	{
		if (ui->GetState() == UIScreen::ACTIVE)
		{
			ui->Update(m_deltaTime);
		}

	}

	// �V�[���X�V�̔��菈��
	if (tmpScene != m_nowScene)
	{

		// ���݂̃V�[�����폜
		delete m_nowScene;

		//�V�K�V�[�������̃V�[���ɍ����ւ�
		m_nowScene = tmpScene;

		// ����������V�K�V�[���p�ɍX�V����
		delete m_physicsWorld;
		m_physicsWorld = new PhysicsWorld();

		// ���[�h��ʐ���
		m_load = new LoadScreen();
		m_load->Initialize();

		// �V�[���̏�����
		m_nowScene->Initialize();

		// �|�[�Y��ʐ���
		m_pause = new PauseScreen();

		return 1;
	}


	return 0;

}

void GameMain::UpdateActor()
{
	// �S�A�N�^�[�X�V
	for (auto actor : m_actors)
	{
		actor->Update(m_deltaTime);
	}

	// �y���f�B���O���A�N�^�[���A�N�e�B�u�Ɉړ�
	for (auto pending : m_pendingActors)
	{
		pending->ComputeWorldTransform();
		m_actors.emplace_back(pending);
	}
	m_pendingActors.clear();

	// �S�Ă̎��S���Ă���A�N�^�[���ꎞ�ۊ�
	std::vector<Actor*> deadActors;
	for (auto actor : m_actors)
	{
		if (actor->GetState() == Actor::STATE_DEAD)
		{
			deadActors.emplace_back(actor);
		}
	}

	// ���S���Ă���A�N�^�[��Delete
	for (auto actor : deadActors)
	{
		delete actor;
	}
	deadActors.clear();

}


// �I������
void GameMain::CloseGame()
{
	Delete();
}

// �ŏ��̃V�[�����Z�b�g��������
void GameMain::SetFirstScene(SceneBase * in_scene)
{
	m_nowScene = in_scene;
	m_nowScene->Initialize();
}


// �f�[�^�̃��[�h
void GameMain::Load()
{
}

// �f�[�^�̃A�����[�h
void GameMain::Unload()
{
}

void GameMain::Input()
{
	// �I���C�x���g�̃L���b�`
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			m_isRunning = false;
			break;

		default:
			CONTROLLER_INSTANCE.ReceiveEvent(event);
			break;
		}
	}

	// �L�[�{�[�h���͍X�V
	INPUT_INSTANCE.Update();

	// �R���g���[�����͍X�V
	CONTROLLER_INSTANCE.Update();

	// �X�e�A�����O�R���g���[�����͍X�V
	STEERING_CONTROLLER_INSTANCE.Update();

	// �}�E�X���͍X�V
	MOUSE_INSTANCE.Update();

	// ESC�������ꂽ��I��
	if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_ESCAPE))
	{
		m_isRunning = false;
	}

	// �A�N�^�[�f�o�b�O
	if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_F12))
	{
		ShowActor();
	}

	// �����_�����O���\�[�X�\��
	if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_F11))
	{
		GetRenderer()->ShowResource();
	}

	// �|�[�Y���[�h�ڍs�^����
	if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_BACKSPACE) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_START))
	{
		//�|�[�Y���[�h�ؑ�
		m_isPauseMode = !m_isPauseMode;
		Actor::State changeState;
		if (m_isPauseMode)
		{
			changeState = Actor::STATE_PAUSED;        // �|�[�Y
			m_pause->SetModeON();                     // �|�[�Y��ʂ��I��
		}
		else
		{
			m_pause->SetModeOFF();                    // �|�[�Y��ʂ��I�t
			changeState = Actor::STATE_ACTIVE;        // �A�N�e�B�u
		}

		//�S�ẴX�e�[�g��ύX����
		for (auto itr : m_actors)
		{
			if (itr->GetState() != Actor::STATE_DEAD)
			{
				itr->SetState(changeState);
			}
		}
	}
}

void GameMain::SetDrawDebugBox(OBB in_box)
{
	if (!m_debugBox)
	{
		return;
	}

	m_debugBox->SetDebugBox(in_box);
}

void GameMain::DrawDebugBoxes()
{
	if (!m_debugBox)
	{
		return;
	}

	m_debugBox->Draw();
}

SDL_Renderer* GameMain::GetSDLRenderer()
{
	return m_renderer->GetSDLRenderer();
}

// �A�N�^�[�̒ǉ�����
void GameMain::AddActor(Actor * in_actor)
{
	// �y���f�B���O�A�N�^�[���ɒǉ�
	m_pendingActors.emplace_back(in_actor);
}

// �A�N�^�[�̍폜����
void GameMain::RemoveActor(Actor * in_actor)
{
	// �y���f�B���O�A�N�^�[���ɂ��邩
	auto iter = std::find(m_pendingActors.begin(), m_pendingActors.end(), in_actor);
	// �C�e���[�^�[���y���f�B���O�A�N�^�[�̖����ɗ�����폜���Ă悢
	if (iter != m_pendingActors.end())
	{
		// �y���f�B���O�A�N�^�[�̖����Ƀf�[�^���ڂ��A���̃f�[�^��pop_back����
		std::iter_swap(iter, m_pendingActors.end() - 1);
		m_pendingActors.pop_back();
		// �I��
		return;
	}

	// �A�N�e�B�u�A�N�^�[���ɂ��邩
	iter = std::find(m_actors.begin(), m_actors.end(), in_actor);

	if (iter != m_actors.end())
	{
		// �A�N�e�B�u�A�N�^�[�̖����Ƀf�[�^���ڂ��A���̃f�[�^��pop_back����
		std::iter_swap(iter, m_actors.end() - 1);
		m_actors.pop_back();
	}
}

void GameMain::DeadAllActor()
{
	for (auto actor : m_actors)
	{
		actor->SetState(Actor::STATE_DEAD);
	}
}

// UI��ʂ̒ǉ�
void GameMain::AddUI(UIScreen * in_screen)
{
	m_uiStack.emplace_back(in_screen);
}

// UI�̔��](�|�[�Y��ʂ��őO�ʂɂ��邽�߂̏���)
void GameMain::SwapPauseUI()
{
	m_pause->Close();

	m_pause = new PauseScreen();
}

// �����̃L�[�ƂȂ�t�@�C���p�X�Ńt�H���g�z����������A��v�����t�H���g��Ԃ�
// ������Ȃ������ꍇ�A�V�K�Ƀt�H���g�𐶐����A�z��ɒǉ���A�t�H���g��Ԃ�
Font* GameMain::GetFont(const std::string & in_keyPath)
{
	// �����t�H���g�����łɔz��ɂȂ�������
	auto iter = m_fonts.find(in_keyPath);

	// �����t�H���g�������Ă�����
	if (iter != m_fonts.end())
	{
		// �����t�H���g��Ԃ�
		return iter->second;
	}
	// �t�H���g�������Ă��Ȃ�������t�H���g�𐶐����A���������t�H���g��Ԃ�
	else
	{
		Font* font = new Font();

		if (font->Load(in_keyPath))
		{
			m_fonts.emplace(in_keyPath, font);
		}
		else
		{
			font->UnLoad();
			delete font;

			font = nullptr;
		}
		return font;
	}


}

// �e�L�X�g�̃��[�h
void GameMain::LoadText(const std::string & in_fileName)
{
	// �e�L�X�g�}�b�v�̃N���A
	m_text.clear();
	// �w��p�X�̃t�@�C�����󂯎��
	std::ifstream file(in_fileName);
	// �J���Ȃ�������
	if (!file.is_open())
	{
		SDL_Log("Text file %s not found\n", in_fileName.c_str());
		return;
	}

	// ��������擾
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	// rapidjson�ŊJ��
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);
	if (!doc.IsObject())
	{
		SDL_Log("Text file %s is not valid JSON\n", in_fileName.c_str());
		return;
	}
	// �e�L�X�g�}�b�v�ɕۊ�
	const rapidjson::Value& actions = doc["TextMap"];
	for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin();
		itr != actions.MemberEnd(); ++itr)
	{
		if (itr->name.IsString() && itr->value.IsString())
		{
			m_text.emplace(itr->name.GetString(),
				itr->value.GetString());
		}
	}
}

// �e�L�X�g�̎擾
const std::string & GameMain::GetText(const std::string & in_key)
{
	static std::string errorMsg("**KEY NOT FOUND**");
	//�}�b�v����w��̃L�[������
	auto iter = m_text.find(in_key);
	if (iter != m_text.end())
	{
		return iter->second;
	}
	else
	{
		return errorMsg;
	}
}

// �J�����̃Z�b�g
void GameMain::SetCamera(Camera * in_camera)
{

	//printf("SetCamera [%p]\n", in_camera);
	//m_activeCamera = in_camera;

}

void GameMain::SetCamera(CameraComponent* in_camera)
{
	//printf("SetCamera [%p]\n", in_camera);
	m_activeCamera = in_camera;
}

// �����̃J�������A�N�e�B�u�ɂ���(�����_���[�̃J������o�^)
void GameMain::InActiveCamera(Camera * in_activeCam)
{
	//if (in_activeCam == m_activeCamera)
	//{
	//	printf("Camera is inActive, change to default view.\n");
	//	m_activeCamera = nullptr;
	//}

	//m_viewMatrix = 
	//	Matrix4::CreateLookAt(Vector3(0, 0, 100),
	//	Vector3(0, 0, 0),
	//	Vector3(0, 0, 1));
}

void GameMain::InActiveCamera(CameraComponent* in_activeCam)
{
	if (in_activeCam == m_activeCamera)
	{
		printf("Camera is inActive, change to default view.\n");
		m_activeCamera = nullptr;
	}

	m_viewMatrix =
		Matrix4::CreateLookAt(Vector3(0, 0, 100),
			Vector3(0, 0, 0),
			Vector3(0, 0, 1));
}

const Vector3 & GameMain::GetViewVector()
{
	if (m_activeCamera == nullptr)
	{
		printf("Camera is inActive. return IllegalVec\n");
		return Vector3::Zero;
	}
	return m_activeCamera->GetViewMatrix().GetTranslation();

}



void GameMain::ShowActor()
{
	printf("\n\n<--------------ActorList----------------->\n");
	printf("---------> Active Actor ( %zd ) <-----------\n", m_actors.size());

	for (auto i : m_actors)
	{
		printf("mem [%p] : id: %d ", i, i->GetID());
		std::cout << typeid(*i).name() << "\n";
	}
	printf("---------> Pending Actor ( %zd ) <-----------\n", m_pendingActors.size());
	for (auto i : m_pendingActors)
	{
		printf("mem [%p] : id: %d ", i, i->GetID());
		std::cout << typeid(*i).name() << "\n";
	}
}

void GameMain::Draw()
{


	if (!m_nowScene)
	{
		return;
	}


	m_nowScene->Draw();

	

}

// �Q�[�����[�v����p�̃w���p�[�֐�
void GameMain::LoopHelper()
{
	SDL_Event event;
	// �L���[�ɃC�x���g������ΌJ��Ԃ�
	switch (SDL_PollEvent(&event))
	{
		// �E�B���h�E�����(x���N���b�N����)�ꍇ�A���C�����[�v���I��
		case SDL_QUIT:
			m_isRunning = false;
			break;
	}

	// �L�[�{�[�h�̉�����Ԃ��擾
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	// ESC�L�[�Ń��C�����[�v�𔲂���
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		m_isRunning = false;
	}
}

void GameMain::DebugRenderer()
{
	// SDL�E�B���h�E�̓h��Ԃ�(�f�o�b�O�p)
	SDL_SetRenderDrawColor
	(
		m_debugRenderer,
		0,                   // R
		0,                   // G
		255,                 // B
		255                  // A
	);

	// �o�b�N�o�b�t�@�����݂̕`��F�ŃN���A
	SDL_RenderClear(m_debugRenderer);



	// �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@������
	SDL_RenderPresent(m_debugRenderer);
}
