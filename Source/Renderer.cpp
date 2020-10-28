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
#include "FrameBuffer.h"
#include <algorithm>
#include <GL/glew.h>
#include "BoxCollider.h"
#include "Collision.h"
#include "WorldSpaceUI.h"
#include "ShadowMap.h"

// コンストラクタ
Renderer::Renderer()
	:m_window(nullptr)
	,m_SDLRenderer(nullptr)
	,m_context(0)
	,m_spriteShader(nullptr)
	,m_meshShader(nullptr)
	,m_meshNormalShader(nullptr)
	,m_skinnedShader(nullptr)
	,m_skyboxShader(nullptr)
	,m_frameBuffer(nullptr)
	,m_switchShader(0)
	,m_cameraPos(Vector3::Zero)
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
	// 指定 -> OpenGL/ver4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);     // メジャーバージョン
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);     // マイナーバージョン
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

	// wminfo構造体
	SDL_SysWMinfo wminfo;
	SDL_VERSION(&wminfo.version);
	SDL_GetWindowWMInfo(m_window, &wminfo);
	m_hwnd = wminfo.info.win.window;
	
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
	// ワールド空間用スプライト関連
	//------------------------------------------------------------------+
	// ワールド空間用スプライト頂点作成
	CreateWorldSpriteVerts();
	// パーティクルマネージャー作成
	m_particleManager = new ParticleManager;

	//------------------------------------------------------------------+
	// スクリーン空間用のSprite関連
	//------------------------------------------------------------------+
	CreateSpriteVerts();        // スプライト用の頂点作成

	//------------------------------------------------------------------+
	// キューブマップ用頂点配列
	//------------------------------------------------------------------+
	CreateCubeMapVerts();


	//------------------------------------------------------------------+
	// ポストエフェクト
	//------------------------------------------------------------------+
	m_frameBuffer = new FrameBuffer();
	m_frameBuffer->CreateFrameBuffer();

	//--------------------------------------------+
    // シャドウマップ関連
    //--------------------------------------------+
	m_shadowMap = new ShadowMap();

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

	// スケルトンの破棄
	for (auto sk : m_skeletons)
	{
		delete sk.second;
	}
	// アニメーションの破棄
	for (auto anim : m_animations)
	{
		delete anim.second;
	}

	// シェーダーの解放
	m_meshShader->Delete();

	// スプライトの解放
	for (auto sprite : m_worldSprites)
	{
		delete sprite;
	}
	m_worldSprites.clear();

	delete m_spriteVerts;
	m_spriteShader->Delete();
	m_worldSpaceSpriteShader->Delete();
	delete m_spriteShader;
	delete m_skyboxVerts;
	m_skyboxShader->Delete();
	delete m_frameBuffer;
	delete m_shadowMap;

	// コンテキストの破棄
	SDL_GL_DeleteContext(m_context);
	// ウィンドウの破棄
	SDL_DestroyWindow(m_window);
}

// 描画処理
void Renderer::Draw()
{

	// ImGuiフレームを開始
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(GAME_INSTANCE.GetRenderer()->GetSDLWindow());
	ImGui::NewFrame();
	// ImGui更新
	ImGui::Begin("Renderer");
	ImGui::SliderInt("MeshShader", &m_switchShader, 0, 2);

	// フレームバッファ書き込み処理
	m_frameBuffer->WriteFrameBuffer();

	// 深度テストをON、ブレンドをオフ
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	// 画面をクリア
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//----------------------------------------------+
	// メッシュシェーダー(phong)
	//----------------------------------------------+
	if (m_switchShader == 0)
	{
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
	}

	//-----------------------------------------------+
	// メッシュシェーダー(normal)
	//-----------------------------------------------+
	if (m_switchShader == 1)
	{
		//メッシュシェーダーで描画する対象の変数をセット
		m_meshNormalShader->SetActive();
		m_meshNormalShader->SetMatrixUniform("uViewProj", m_view * m_projection);
		// ライティング変数をセット
		SetLightUniforms(m_meshNormalShader);
		m_meshNormalShader->SetVectorUniform("uLightPos", m_directionalLight.position);
		m_meshNormalShader->SetVectorUniform("uViewPos", m_cameraPos);
		m_meshNormalShader->SetVectorUniform("uCameraPos", m_cameraPos);
		// 全てのメッシュコンポーネントを描画
		for (auto mc : m_meshComponents)
		{
			if (mc->GetVisible())
			{
				mc->Draw(m_meshNormalShader);
			}
		}
	}


	//-----------------------------------------------------------+
	// ボーン入りメッシュの描画
	//-----------------------------------------------------------+
	m_skinnedShader->SetActive();
	// ビュー・プロジェクションの合成行列をセット
	m_skinnedShader->SetMatrixUniform("uViewProj", m_view * m_projection);
	// ライティング変数をセット
	SetLightUniforms(m_skinnedShader);

	for (auto sk : m_skeletalMeshComponents)
	{
		if (sk->GetVisible())
		{
			sk->Draw(m_skinnedShader);
		}
	}
	
	//-----------------------------------------------+
    // メッシュシェーダー(shadow)
    //-----------------------------------------------+
	if (m_switchShader == 2)
	{
		m_shadowMap->RenderDepthMapFromLightView(this, m_meshComponents);
		m_shadowMap->DrawShadowMesh(m_meshComponents);
	}
	//---------------------------------------------------------------+
	// スカイボックスの描画
	//---------------------------------------------------------------+
	// シェーダーアクティブ化・スカイボックス用VAOをバインド
	m_skyboxShader->SetActive();
	m_skyboxVerts->SetActive();

	for (auto sky : m_skyBoxComponents)
	{
		sky->Draw(m_skyboxShader);
	}

	//----------------------------------------------------------------+
	// パーティクル描画
	//----------------------------------------------------------------+
	m_particleManager->Draw();

	// ワールド空間上のスプライト描画
	m_worldSpaceSpriteShader->SetActive();
	for (auto spr : m_worldSprites)
	{
		spr->Draw(m_worldSpaceSpriteShader);
	}


	// Spriteの描画
	// ブレンドのアクティブ化
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 深度テストの停止
	glDisable(GL_DEPTH_TEST);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// spriteシェーダーのアクティブ化
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
	//MOUSE_INSTANCE.ImGuiDebugRendering();
	// STEERING_CONTROLLER_INSTANCE.RenderDebugImGui();

	// フレームバッファ描画
	m_frameBuffer->DrawFrameBuffer();

	ImGui::End();
	ImGui::Render();
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
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

	// イテレータで指定した番号にスプライトを挿入
	m_spriteComponents.insert(iter, in_sprite);
}

// 指定したスプライトの削除
void Renderer::RemoveSprite(SpriteComponent * in_sprite)
{
	// イテレータで削除指示の出たスプライトを探索
	auto iter = std::find(m_spriteComponents.begin(), m_spriteComponents.end(), in_sprite);
	m_spriteComponents.erase(iter);
}

// ワールド上のスプライトを配列に追加
void Renderer::AddSpriteWorld(WorldSpaceUI* in_sprite)
{
	m_worldSprites.push_back(in_sprite);
}

// ワールドスプライト配列から指定したスプライトを削除
void Renderer::RemoveSpriteWorld(WorldSpaceUI* in_sprite)
{
	auto iter = std::find(m_worldSprites.begin(), m_worldSprites.end(), in_sprite);
	m_worldSprites.erase(iter);
}


// メッシュコンポーネント追加
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

// メッシュコンポーネント格納配列から削除
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

// skybox配列に追加する
void Renderer::AddSkyBox(CubeMapComponent* in_comp)
{
	m_skyBoxComponents.emplace_back(in_comp);
}

// skybox配列からの削除
void Renderer::RemoveSkyBox(CubeMapComponent* in_comp)
{
	auto iter = std::find(m_skyBoxComponents.begin(), m_skyBoxComponents.end(), in_comp);
	m_skyBoxComponents.erase(iter);
}



// ウィンドウのタイトル
void Renderer::SetWindowTitle(const std::string & in_title)
{
	SDL_SetWindowTitle(m_window, in_title.c_str());
}

// パーティクルの頂点情報をバインドしアクティブ化
void Renderer::SetWorldSpriteVertex()
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

//-------------------------------------------------------------------------------------+
// スケルタル情報の取得
// in  : スケルタル情報 .gpskelファイル名
// out : Skeleton情報へのポインタ
// Desc: gpskelファイル名より、スケルタル情報を返す。ない場合はそのファイル名で
//       読み込みを行う。読み込みを行ってもファイルが存在しない場合 nullptrを返す
//       内部でgpskelファイル名をキーとするスケルタル情報のmapが作成される
//-------------------------------------------------------------------------------------+
const Skeleton* Renderer::GetSkeleton(const std::string& in_fileName)
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
// アニメーション情報の取得
// in  : アニメーションデータが格納されている .gpanimファイル名
// out : アニメーション情報へのポインタ
// Desc: gpanimファイル名よりアニメーションデータを返す。ない場合はそのファイル名で
//       読み込みを行う。読み込みを行ってもファイルが存在しない場合 nullptrを返す
//       内部でgpanimファイル名をキーとするアニメーション情報のmapが作成される
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

// パーティクルマネージャー取得
ParticleManager * Renderer::GetParticleManager() const
{
	return m_particleManager;
}

// スカイボックス用頂点配列定義
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
		0, 1, 2, 0, 2, 3,    // 前面
		4, 5, 6, 4, 6, 7,    // 背面
		8, 9, 10, 8, 10, 11,   // 上面
		12, 13, 14, 12, 14, 15,   // 底面
		16, 17, 18, 16, 18, 19,   // 右側面
		20, 21, 22, 20, 22, 23    // 左側面
	};




	// VAO, VBO作成
	m_skyboxVerts = new VertexArray(skyboxVertices, 36);
}

// スプライト(2D用)の頂点配列を生成
void Renderer::CreateSpriteVerts()
{
	float vertices[] = 
	{
      // x     y     z    nx   ny   nz    u    v
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 左上頂点
		 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 右上頂点
		 0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右下頂点
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // 左下頂点

	};

	unsigned int indices[] = 
	{
	    0, 2, 1,
	    2, 0, 3
	};

	m_spriteVerts = new VertexArray(vertices, 4, VertexArray::POS_NORMAL_TEX, indices, 6);
}

// パーティクル用の頂点配列を生成
void Renderer::CreateWorldSpriteVerts()
{
	float vertices[] = 
	{
		-0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 左上頂点
		 0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 右上頂点
		 0.5f, 0.0f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右下頂点
		-0.5f, 0.0f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // 左下頂点
	};

	unsigned int indices[] = 
	{
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

	if (!m_spriteShader->Load("Data/Shaders/SpriteShader.vert", "Data/Shaders/SpriteShader.frag"))
	{
		return false;
	}
	m_spriteShader->SetActive();
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(m_screenWidth, m_screenHeight);
	m_spriteShader->SetMatrixUniform("u_ViewProj", viewProj);

	// ワールド空間用スプライトシェーダー
	m_worldSpaceSpriteShader = new Shader();
	if (!m_worldSpaceSpriteShader->Load("Data/Shaders/WorldSpaceSprite.vert", "Data/Shaders/WorldSpaceSprite.frag"))
	{
		return false;
	}
	m_worldSpaceSpriteShader->SetActive();
	m_worldSpaceSpriteShader->SetMatrixUniform("u_ViewProj", viewProj);


	// メッシュシェーダー
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

	// メッシュシェーダー(法線マップ)
	m_meshNormalShader = new Shader();
	if (!m_meshNormalShader->Load("Data/Shaders/NormalMap.vert", "Data/Shaders/NormalMap.frag"))
	{
		return false;
	}
	m_meshNormalShader->SetActive();
	m_meshNormalShader->SetMatrixUniform("uViewProj", m_view * m_projection);
	m_meshNormalShader->SetVectorUniform("uLightPos", m_directionalLight.position);
	m_meshNormalShader->SetVectorUniform("uCameraPos", m_cameraPos);
	// サンプリング用テクスチャセット
	m_meshNormalShader->SetInt("u_mat.diffuseMap", 0);
	m_meshNormalShader->SetInt("u_mat.specularMap", 1);
	m_meshNormalShader->SetInt("u_mat.normalMap", 2);
	m_meshNormalShader->SetInt("u_mat.depthMap", 3);

	// スキンシェーダー
	m_skinnedShader = new Shader();
	if (!m_skinnedShader->Load("Data/Shaders/Skinned.vert", "Data/Shaders/Phong.frag"))
	{
		return false;
	}
	m_skinnedShader->SetActive();
	m_skinnedShader->SetMatrixUniform("uViewProj", m_view * m_projection);
	// サンプリング用テクスチャセット
	m_skinnedShader->SetInt("u_mat.diffuseMap", 0);
	m_skinnedShader->SetInt("u_mat.specularMap", 1);
	m_skinnedShader->SetInt("u_mat.normalMap", 2);
	m_skinnedShader->SetInt("u_mat.depthMap", 3);

	// スカイボックス用シェーダー
	m_skyboxShader = new Shader();
	if (!m_skyboxShader->Load("Data/Shaders/SkyBox.vert", "Data/Shaders/SkyBox.frag"))
	{
		return false;
	}


	return true;
}

// シェーダーのuniformに値をセット
void Renderer::SetLightUniforms(Shader * in_shader)
{
	// ビュー行列からカメラ位置を逆算出する
	Matrix4 invView = m_view;
	invView.Invert();
	in_shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	in_shader->SetVectorUniform("uViewPos", invView.GetTranslation());

	// アンビエントライト
	in_shader->SetVectorUniform("uAmbientLight", m_ambientLight);

	// ディレクショナルライト
	in_shader->SetVectorUniform("uDirLight.mPosition", m_directionalLight.position);
	in_shader->SetVectorUniform("uDirLight.mDirection", m_directionalLight.direction);
	in_shader->SetVectorUniform("uDirLight.mDiffuseColor", m_directionalLight.diffuse);
	in_shader->SetVectorUniform("uDirLight.mSpecColor", m_directionalLight.specular);
}
