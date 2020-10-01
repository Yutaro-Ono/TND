//-----------------------------------------------------------------------+
// レンダラー(グラフィック描画)クラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Renderer.h"
#include "GameMain.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Texture.h"
#include "SpriteComponent.h"
#include "Mesh.h"
#include "MeshGpmesh.h"
#include "MeshObj.h"
#include "MeshComponent.h"
#include "Shader.h"
#include "VertexArray.h"
#include "ParticleManager.h"
#include "UIScreen.h"
#include <algorithm>
#include <GL/glew.h>

// コンストラクタ
Renderer::Renderer()
	:m_window(nullptr)
	,m_SDLRenderer(nullptr)
	,m_context(0)
	,m_spriteShader(nullptr)
	,m_meshShader(nullptr)
	,m_skinnedShader(nullptr)
{
}

// デストラクタ
Renderer::~Renderer()
{
}

// 各種初期化処理 (GL SDL)
bool Renderer::Initialize(int in_screenW, int in_screenH, bool in_full)
{
	// 画面サイズを設定
	m_screenWidth = in_screenW;
	m_screenHeight = in_screenH;


	//-----------------------------------------------------------------+
    // OpenGLの属性設定
    //-----------------------------------------------------------------+
    // OpenGLコアプロファイルを使用する
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// 指定 -> OpenGL/ver3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);     // メジャーバージョン
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);     // マイナーバージョン
	// RGBA各チャンネルに8bitのカラーバッファを指定する
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);                  // R
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);                // G
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);                 // B
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);                // A
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// ダブルバッファを有効にする
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// ハードウェアアクセラレーションを使用する
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// SDLウィンドウの作成
	m_window = SDL_CreateWindow
	(
		"FlameCarrier",                                 // ウィンドウの名称
		0,                                              // x座標のウィンドウ描画原点
		0,                                              // y座標のウィンドウ描画原点
		m_screenWidth,              // 画面の横幅
		m_screenHeight,             // 画面の縦幅
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	// ウィンドウの作成に失敗したら
	if (!m_window)
	{
		printf("Create Window Failed : %s", SDL_GetError());
		return false;
	}
	printf("(%d, %d) Create Window\n", m_screenWidth, m_screenHeight);

	// フルスクリーンだったら
	if (in_full)
	{
		if (SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP))
		{
			printf("(%d x %d) Change FullScreen Failed\n", in_screenW, in_screenH);
			return false;
		}
	}
	glViewport(0, 0, m_screenWidth, m_screenHeight);

    //----------------------------------------------------------------+
    // OpenGLコンテキスト設定
    //----------------------------------------------------------------+
    // OpenGLコンテキストを生成
	m_context = SDL_GL_CreateContext(m_window);
	SDL_GL_MakeCurrent(m_window, m_context);

	//----------------------------------------------------------------+
    // GLEW初期化
    //----------------------------------------------------------------+
	glewExperimental = GL_TRUE;
	// 初期化に失敗したら
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("GLEW Initialize : Failed");
		return false;
	}

	//-----------------------------------------------------------------+
	// SDLレンダラーの作成
	//-----------------------------------------------------------------+
	m_SDLRenderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_SDLRenderer)
	{
		printf("SDLRendererの作成に失敗 : %s", SDL_GetError());
		return false;
	}
	printf("Create SDLRenderer : Success\n");

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("SDLImageInitPNG初期化に失敗 : %s", SDL_GetError());
		return false;
	}
	printf("SDLImageInitPNG : Success\n");

	//--------------------------------------------------------------------+
    // Imgui初期化
    //--------------------------------------------------------------------+
	// imguiコンテキスト生成
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// imguiスタイル設定 (黒テーマ)
	ImGui::StyleColorsDark();
	// 使用しているプラットフォームにバインド
	ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
	ImGui_ImplOpenGL3_Init("#version 330");



	// GLEWは無害なエラーコードを出すのでクリアしておく
	glGetError();

	// シェーダープログラムの初期化
	if (!LoadShaders())
	{
		printf("Shader Load : Failed");
		return false;
	}


	//------------------------------------------------------------------+
	// パーティクル関連
	//------------------------------------------------------------------+
	// パーティクル用頂点作成
	CreateParticleVerts();
	// パーティクルマネージャー作成
	m_particleManager = new ParticleManager;

	//------------------------------------------------------------------+
	// Sprite関連
	//------------------------------------------------------------------+
	CreateSpriteVerts();        // スプライト用の頂点作成

	// カリング
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	// 初期化に成功
	return true;
}

void Renderer::Delete()
{
	// パーティクルマネージャー破棄
	delete m_particleManager;

	// テクスチャの解放
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

	// メッシュの解放
	for (auto i : m_meshes)
	{
		printf("Meshes Release : %s\n", i.first.c_str());

		i.second->Delete();
		delete i.second;
	}
	m_meshes.clear();

	// シェーダーの解放
	m_meshShader->Delete();

	// スプライトの解放
	delete m_spriteVerts;
	m_spriteShader->Delete();
	delete m_spriteShader;

	// コンテキストの破棄
	SDL_GL_DeleteContext(m_context);
	// ウィンドウの破棄
	SDL_DestroyWindow(m_window);
}

// 描画処理
void Renderer::Draw()
{

	// Set the clear color to light grey
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);



	//メッシュシェーダーで描画する対象の変数をセット
	m_meshShader->SetActive();
	m_meshShader->SetMatrixUniform("uViewProj", m_view * m_projection);
	// ライティング変数をセット
	SetLightUniforms(m_meshShader);
	// 全てのメッシュコンポーネントを描画
	for (auto mc : m_meshComponents)
	{
		if (mc->GetVisible())
		{
			mc->Draw(m_meshShader);
		}
	}

	// Draw any skinned meshes now
	// m_skinnedShader->SetActive();
	// Update view-projection matrix
	// m_skinnedShader->SetMatrixUniform("uViewProj", m_view * m_projection);
	// Update lighting uniforms
	// SetLightUniforms(m_skinnedShader);

	//for (auto sk : m_skeletalMeshes)
	//{
	//	if (sk->GetVisible())
	//	{
	//		sk->Draw(m_skinnedShader);
	//	}
	//}


	// パーティクル描画
	m_particleManager->Draw();

	// Spriteの描画
	// Draw all sprite components
	// Disable depth buffering
	glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// Set shader/vao as active
	m_spriteShader->SetActive();
	m_spriteVerts->SetActive();

	for (auto sprite : m_spriteComponents)
	{
		if (sprite->GetVisible())
		{
			sprite->Draw(m_spriteShader);
		}
	}




	// 全てのUIを更新
	for (auto ui : GAME_INSTANCE.GetUIStack())
	{
		ui->Draw(m_spriteShader);
	}

	// デバッグコンソール描画
	//MOUSE_INSTANCE.DebugImGui();

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



void Renderer::AddMeshComponent(MeshComponent * in_mesh)
{

	if (in_mesh->GetIsSkeletal())
	{
		//----------------------------------------+
		// 未使用
		//----------------------------------------+
	}
	else
	{
		m_meshComponents.emplace_back(in_mesh);
	}

}

void Renderer::RemoveMeshComponent(MeshComponent * in_mesh)
{

	if (in_mesh->GetIsSkeletal())
	{
		//----------------------------------------+
		// 未使用
		//----------------------------------------+
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



// ウィンドウのタイトル
void Renderer::SetWindowTitle(const std::string & in_title)
{
	SDL_SetWindowTitle(m_window, in_title.c_str());
}

// パーティクルの頂点情報をバインドしアクティブ化
void Renderer::SetParticleVertex()
{
	m_particleVertex->SetActive();
}

// ディレクショナルライトのセット
void Renderer::SetDirectionalLight(const DirectionalLight & in_dirLight)
{
	m_directionalLight = in_dirLight;
}

// テクスチャの取得
Texture * Renderer::GetTexture(const std::string & in_fileName)
{
	Texture* tex = nullptr;
	auto iter = m_textures.find(in_fileName);     // 同じパスのテクスチャがあるかを検索
	// 同じテクスチャが登録してあった
	if (iter != m_textures.end())
	{
		// 同じテクスチャを指す
		tex = iter->second;
	}
	else
	{
		// なかったらテクスチャを生成し、配列へ入れる
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

// メッシュの取得
Mesh * Renderer::GetMesh(const std::string & in_fileName)
{
	// 同じパスのメッシュを検索する
	Mesh* mesh = nullptr;
	auto iter = m_meshes.find(in_fileName);
	// すでに同じメッシュが登録してあった場合は、要素の二つ目(メッシュクラス)を複製
	if (iter != m_meshes.end())
	{
		mesh = iter->second;
	}
	else
	{

		// ファイルパス拡張子を調べ、それに応じてメッシュを生成
		int extNum = in_fileName.find_last_of(".");     // "."以降が何文字あるか調べる
		std::string fileExtension = in_fileName.substr(extNum, in_fileName.size() - extNum);    // ".#####"という形で拡張子を取得

		if (fileExtension == ".gpmesh")       // gpmesh形式
		{
			mesh = new MeshGpmesh();
			if (mesh->Load(in_fileName, GAME_INSTANCE.GetRenderer()))
			{
				m_meshes.emplace(in_fileName, mesh);
			}
			else
			{
				std::cerr << "ERROR: Category: " << "MeshComponent: gpmeshファイルのロード失敗" << std::endl;
				delete mesh;
				mesh = nullptr;
			}
		}
		else if (fileExtension == ".obj" || fileExtension == ".OBJ")     // obj形式
		{
			mesh = new MeshObj();
			if (mesh->Load(in_fileName, GAME_INSTANCE.GetRenderer()))
			{
				m_meshes.emplace(in_fileName, mesh);
			}
			else
			{
				std::cerr << "ERROR: Category: " << "MeshComponent: objファイルのロード失敗" << std::endl;
				delete mesh;
				mesh = nullptr;
			}
		}

	}

	printf("Mesh Load Success : %s\n", in_fileName.c_str());

	return mesh;
}

// パーティクルマネージャー取得
ParticleManager * Renderer::GetParticleManager() const
{
	return m_particleManager;
}

// スプライト(2D用)の頂点配列を生成
void Renderer::CreateSpriteVerts()
{
	float vertices[] = 
	{
      // x     y     z    nx   ny   nz    u    v
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // 左上頂点
		 0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // 右上頂点
		 0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // 右下頂点
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // 左下頂点

	};

	unsigned int indices[] = {
	    0, 2, 1,
	    2, 0, 3
	};

	m_spriteVerts = new VertexArray(vertices, 4, VertexArray::POS_NORMAL_TEX, indices, 6);
}

// パーティクル用の頂点配列を生成
void Renderer::CreateParticleVerts()
{
	float vertices[] = {
	-0.5f, 0.f, 0.5f, 0.f, 0.f, 0.0f, 0.f, 0.f, // 左上頂点
	 0.5f, 0.f, 0.5f, 0.f, 0.f, 0.0f, 1.f, 0.f, // 右上頂点
	 0.5f, 0.f,-0.5f, 0.f, 0.f, 0.0f, 1.f, 1.f, // 右下頂点
	-0.5f, 0.f,-0.5f, 0.f, 0.f, 0.0f, 0.f, 1.f  // 左下頂点
	};

	unsigned int indices[] = {
		0, 2, 1,
		2, 0, 3
	};
	m_particleVertex = new VertexArray(vertices, 4, VertexArray::POS_NORMAL_TEX, indices, 6);
}

// シェーダーのロード
bool Renderer::LoadShaders()
{

	// スプライトシェーダー
	m_spriteShader = new Shader();

	if (!m_spriteShader->Load("Data/Shaders/Sprite.vert", "Data/Shaders/Sprite.frag"))
	{
		return false;
	}

	m_spriteShader->SetActive();
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(m_screenWidth, m_screenHeight);
	m_spriteShader->SetMatrixUniform("uViewProj", viewProj);



	// メッシュシェーダー
	m_meshShader = new Shader();

	m_view = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	m_projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		static_cast<float>(m_screenWidth),
		static_cast<float>(m_screenHeight),
		1.0f, 10000.0f);

	if (!m_meshShader->Load("Source/Shaders/Phong.vert", "Source/Shaders/Phong.frag"))
	{
		return false;
	}

	m_meshShader->SetActive();

	m_meshShader->SetMatrixUniform("uViewProj", m_view * m_projection);


	// スキンシェーダー
	m_skinnedShader = new Shader();
	if (!m_skinnedShader->Load("Data/Shaders/Skinned.vert", "Data/Shaders/Phong.frag"))
	{
		return false;
	}
	m_skinnedShader->SetActive();
	m_skinnedShader->SetMatrixUniform("uViewProj", m_view * m_projection);

	return true;
}

// シェーダーのuniformに値をセット
void Renderer::SetLightUniforms(Shader * in_shader)
{
	// ビュー行列からカメラ位置を逆算出する
	Matrix4 invView = m_view;
	invView.Invert();
	in_shader->SetVectorUniform("uCameraPos", invView.GetTranslation());

	// アンビエントライト
	in_shader->SetVectorUniform("uAmbientLight", m_ambientLight);

	// ディレクショナルライト
	in_shader->SetVectorUniform("uDirLight.mDirection", m_directionalLight.m_direction);
	in_shader->SetVectorUniform("uDirLight.mDiffuseColor", m_directionalLight.m_diffuseColor);
	in_shader->SetVectorUniform("uDirLight.mSpecColor", m_directionalLight.m_specColor);
}
