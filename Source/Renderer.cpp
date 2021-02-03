//-----------------------------------------------------------------------+
// レンダラー(グラフィック描画)クラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Renderer.h"
#include "GameMain.h"
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
#include "BoxCollider.h"
#include "Collision.h"
#include "WorldSpaceUI.h"
#include "ShadowMap.h"
#include "EnvironmentMapComponent.h"
#include "CameraComponent.h"
#include "RenderBloom.h"
#include "ForwardRenderer.h"
#include "DefferedRenderer.h"

// コンストラクタ
Renderer::Renderer()
	:m_window(nullptr)
	,m_SDLRenderer(nullptr)
	,m_context(0)
	,m_uboMatrices(0)
	,m_spriteShader(nullptr)
	,m_meshShader(nullptr)
	,m_meshNormalShader(nullptr)
	,m_skinnedShader(nullptr)
	,m_skyboxShader(nullptr)
	,m_mapInputShader(nullptr)
	,m_mapOutputShader(nullptr)
	,m_frameBuffer(nullptr)
	,m_bloom(nullptr)
	,m_switchShader(0)
	,m_cameraPos(Vector3::Zero)
	,m_fRenderer(nullptr)
	,m_dRenderer(nullptr)
	,m_mapHUD(nullptr)
	,m_renderMode(RENDER_MODE::DEFFERED)
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
		"TheNightDriver",                                 // ウィンドウの名称
		0,                                                // x座標のウィンドウ描画原点
		0,                                                // y座標のウィンドウ描画原点
		m_screenWidth,                                    // 画面の横幅
		m_screenHeight,                                   // 画面の縦幅
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
    // GLAD初期化
    //----------------------------------------------------------------+
	gladLoadGL();
	const int version = GL_TRUE;
	// 初期化に失敗したら
	if (version == 0)
	{
		SDL_Log("GLAD Initialize : Failed");
		return false;
	}
	glViewport(0, 0, m_screenWidth, m_screenHeight);

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
	ImGui_ImplOpenGL3_Init("#version 420");

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
	// スクリーン空間用Sprite
	//------------------------------------------------------------------+
	CreateSpriteVerts();        // スプライト用の頂点作成

	//------------------------------------------------------------------+
	// キューブ頂点配列の生成
	//------------------------------------------------------------------+
	CreateCubeVerts();

	//------------------------------------------------------------------+
	// ポストエフェクト
	//------------------------------------------------------------------+
	m_frameBuffer = new FrameBuffer();
	m_frameBuffer->CreateFrameBuffer();

	//--------------------------------------------+
    // シャドウマップ
    //--------------------------------------------+
	m_shadowMap = new ShadowMap();

	//--------------------------------------------+
    // Bloom
    //--------------------------------------------+
	m_bloom = new RenderBloom();

	//--------------------------------------------+
	// Forward・DeferedRenderer
	//--------------------------------------------+
	m_fRenderer = new ForwardRenderer(this);
	m_dRenderer = new DefferedRenderer(this);
	if (!m_dRenderer->Initialize())
	{
		return false;
	}
	CreateScreenVerts();
	// uniformバッファ生成
	glGenBuffers(1, &m_uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(Matrix4::mat), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboMatrices, 0, 2 * sizeof(Matrix4::mat));
	glGenBuffers(1, &m_uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Vector3::x) + sizeof(Vector3::y) + sizeof(Vector3::z), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, m_uboCamera, 0, sizeof(Vector3::x) + sizeof(Vector3::y) + sizeof(Vector3::z));


	m_dRenderer->LinkUniformBuffers();

	//// カリング
	//glFrontFace(GL_CCW);
	//glEnable(GL_CULL_FACE);

	// 初期化に成功
	return true;
}

void Renderer::Delete()
{
	// テクスチャの解放
	for (auto i : m_textures)
	{
		printf("Textures Release : %s\n", i.first.c_str());
		i.second->Delete();
		delete i.second;
	}
	m_textures.clear();
	// パーティクルマネージャー破棄
	delete m_particleManager;

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
	m_skeletons.clear();
	// アニメーションの破棄
	for (auto anim : m_animations)
	{
		delete anim.second;
	}
	m_animations.clear();

	// 車メッシュ配列の解放
	for (auto car : m_carMeshComponents)
	{
		delete car;
	}
	m_carMeshComponents.clear();
	// ライトグラス配列の解放
	for (auto light : m_lightGlassComponents)
	{
		delete light;
	}
	m_lightGlassComponents.clear();
	// 環境マップオブジェクト配列の解放
	for (auto env : m_envMeshComponents)
	{
		delete env;
	}
	m_envMeshComponents.clear();

	// スプライトの解放
	for (auto sprite : m_worldSprites)
	{
		delete sprite;
	}
	m_worldSprites.clear();
	// スプライトの解放
	for (auto sprite : m_spriteComponents)
	{
		delete sprite;
	}
	m_spriteComponents.clear();
	// ポイントライト配列の解放
	for (auto light : m_pointLights)
	{
		delete light;
	}
	// スポットライト配列の解放
	for (auto spLight : m_spotLights)
	{
		delete spLight;
	}

	// シェーダーの解放
	delete m_spriteShader;
	delete m_meshShader;
	delete m_worldSpaceSpriteShader;
	delete m_skyboxShader;
	delete m_mapInputShader;
	delete m_mapOutputShader;

	delete m_spriteVerts;
	delete m_cubeVerts;
	delete m_fRenderer;
	delete m_dRenderer;
	delete m_frameBuffer;
	delete m_shadowMap;
	delete m_bloom;
	// コンテキストの破棄
	SDL_GL_DeleteContext(m_context);
	// ウィンドウの破棄
	SDL_DestroyWindow(m_window);
}

// 描画処理
void Renderer::Draw()
{

//#ifdef _DEBUG
	// ImGuiフレームを開始
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplSDL2_NewFrame(GAME_INSTANCE.GetRenderer()->GetSDLWindow());
	//ImGui::NewFrame();
	//// ImGui更新
	//ImGui::Begin("Renderer");
	//ImGui::SliderInt("MeshShader", &m_switchShader, 0, 2);

	//m_renderMode = FORWARD;

//#endif

	//------------------------------------------------+
	// レンダリング (Forward or Deffered)
	//------------------------------------------------+
	// 共通処理
	// uniformバッファへ共通情報を格納する
	// 行列UBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
	// 行列を転送するときは転置する
	Matrix4 transView = m_view;
	transView.Transpose();
	Matrix4 transProj = m_projection;
	transProj.Transpose();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4::mat), transView.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4::mat), sizeof(Matrix4::mat), transProj.GetAsFloatPtr());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// カメラUBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Vector3::x) + sizeof(Vector3::y) + sizeof(Vector3::z), m_view.GetTranslation().GetAsFloatPtr());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	// シャドウ描画用の深度マップにライト視点から見た空間で書き込む
	m_shadowMap->RenderDepthMapFromLightView(m_meshComponents, m_skeletalMeshComponents, m_carMeshComponents);
	// ここから分岐
	if (m_renderMode == RENDER_MODE::FORWARD)
	{
		m_fRenderer->Draw();
	}
	else if (m_renderMode == RENDER_MODE::DEFFERED)
	{
		m_dRenderer->Draw();
	}

//#ifdef _DEBUG

	// ImGuiの終了処理
	//ImGui::End();
	//ImGui::Render();
	//glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//#endif

	// 画面のスワップ
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
/// <summary>
/// 車メッシュ格納
/// </summary>
/// <param name="in_carMesh">追加する車用メッシュコンポーネント</param>
void Renderer::AddCarMeshComponent(CarMeshComponent* in_carMesh)
{
	m_carMeshComponents.emplace_back(in_carMesh);
}
/// <summary>
/// 車メッシュ削除
/// </summary>
/// <param name="in_carMesh">削除する車用メッシュコンポーネント</param>
void Renderer::RemoveCarMeshComponent(CarMeshComponent* in_carMesh)
{
	auto iter = std::find(m_carMeshComponents.begin(), m_carMeshComponents.end(), in_carMesh);
	m_carMeshComponents.erase(iter);
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

// 環境マップオブジェクト配列への追加
void Renderer::AddEnvironmentComponent(EnvironmentMapComponent* in_envMesh)
{
	m_envMeshComponents.push_back(in_envMesh);
}

// 環境マップオブジェクト配列から削除
void Renderer::RemoveEnvironmentComponent(EnvironmentMapComponent* in_envMesh)
{
	auto iter = std::find(m_envMeshComponents.begin(), m_envMeshComponents.end(), in_envMesh);
	m_envMeshComponents.erase(iter);
}

void Renderer::AddLightGlassComponent(LightGlassComponent* in_glassMesh)
{
	m_lightGlassComponents.push_back(in_glassMesh);
}

void Renderer::RemoveLightGlassComponent(LightGlassComponent* in_glassMesh)
{
	auto iter = std::find(m_lightGlassComponents.begin(), m_lightGlassComponents.end(), in_glassMesh);
	m_lightGlassComponents.erase(iter);
}

/// <summary>
/// ポイントライトコンポーネント追加
/// 追加されると描画に適用される
/// </summary>
/// <param name="in_pointL">追加するポイントライトクラス</param>
void Renderer::AddPointLightComponent(PointLightComponent* in_pointL)
{
	m_pointLights.push_back(in_pointL);
}

/// <summary>
/// ポイントライトコンポーネント削除
/// </summary>
/// <param name="in_pointL">削除するポイントライトクラス</param>
void Renderer::RemovePointLightComponent(PointLightComponent* in_pointL)
{
	auto iter = std::find(m_pointLights.begin(), m_pointLights.end(), in_pointL);
	m_pointLights.erase(iter);
}

/// <summary>
/// スポットライトコンポーネント追加
/// </summary>
/// <param name="in_spotL">追加するスポットライトコンポーネント</param>
void Renderer::AddSpotLightComponent(SpotLightComponent* in_spotL)
{
	m_spotLights.push_back(in_spotL);
}

/// <summary>
/// スポットライトコンポーネント削除
/// </summary>
/// <param name="in_spotL">削除するスポットライトコンポーネント</param>
void Renderer::RemoveSpotLightComponent(SpotLightComponent* in_spotL)
{
	auto iter = std::find(m_spotLights.begin(), m_spotLights.end(), in_spotL);
	m_spotLights.erase(iter);
}

void Renderer::RemoveMapHUD()
{
	delete m_mapHUD;
	m_mapHUD = nullptr;
}

// 指定したテクスチャの削除
void Renderer::RemoveTexture(Texture* in_texture)
{
	//auto iter = std::find(m_textures.begin(), m_textures.end(), in_texture);
	//m_textures.erase(iter);

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
void Renderer::SetWorldSpriteVertex()
{
	m_particleVerts->SetActive();
}

// ディレクショナルライトのセット
void Renderer::SetDirectionalLight(const dirLight& in_dirLight)
{
	m_directionalLight = in_dirLight;
}

// スプライト頂点配列のアクティブ化
void Renderer::SetActiveSpriteVAO()
{
	m_spriteVerts->SetActive();
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

// マップHUDのセッター
void Renderer::SetMapHUD(MiniMapHUD* in_mapHUD)
{
	// マップHUDが空でなかったら削除
	if (m_mapHUD != nullptr)
	{
		delete m_mapHUD;
	}
	m_mapHUD = in_mapHUD;
}

// スカイボックス用頂点配列定義
void Renderer::CreateCubeVerts()
{
	m_cubeVerts = new VertexArray();
	m_cubeVerts->CreateCubeVerts();
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
	m_particleVerts = new VertexArray(vertices, 4, VertexArray::POS_NORMAL_TEX, indices, 6);
}

void Renderer::CreateScreenVerts()
{
	m_screenVerts = new VertexArray();
	m_screenVerts->CreateScreenVerts();
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
	// スクリーン用の行列を作成 (UIやスプライトは以降この行列を基準に描画)
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(m_screenWidth, m_screenHeight);
	m_spriteShader->SetMatrixUniform("u_viewProj", viewProj);

	// ワールド空間用スプライトシェーダー
	m_worldSpaceSpriteShader = new Shader();
	if (!m_worldSpaceSpriteShader->Load("Data/Shaders/WorldSpaceSprite.vert", "Data/Shaders/WorldSpaceSprite.frag"))
	{
		return false;
	}


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
	m_meshShader->SetMatrixUniform("u_viewProj", m_view * m_projection);

	// メッシュシェーダー(法線マップ)
	m_meshNormalShader = new Shader();
	if (!m_meshNormalShader->Load("Data/Shaders/NormalMap.vert", "Data/Shaders/NormalMap.frag"))
	{
		return false;
	}
	m_meshNormalShader->SetActive();
	m_meshNormalShader->SetMatrixUniform("u_viewProj", m_view * m_projection);
	m_meshNormalShader->SetVectorUniform("u_lightPos", m_directionalLight.position);
	m_meshNormalShader->SetVectorUniform("u_viewPos", m_cameraPos);
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

	// 環境マップシェーダ
	m_environmentMapShader = new Shader();
	if (!m_environmentMapShader->Load("Data/Shaders/EnvironmentMap.vert", "Data/Shaders/EnvironmentMap.frag"))
	{
		return false;
	}

	// スカイボックス用シェーダー
	m_skyboxShader = new Shader();
	if (!m_skyboxShader->Load("Data/Shaders/SkyBox.vert", "Data/Shaders/SkyBox.frag"))
	{
		return false;
	}

	// マップHUD入力用シェーダー
	m_mapInputShader = new Shader();
	if (!m_mapInputShader->Load("Data/Shaders/HUD/HUD_MapShader.vert", "Data/Shaders/HUD/HUD_MapShader.frag"))
	{
		return false;
	}

	// マップHUD出力用シェーダー
	m_mapOutputShader = new Shader();
	if (!m_mapOutputShader->Load("Data/Shaders/HUD/HUD_MapOutput.vert", "Data/Shaders/HUD/HUD_MapOutput.frag"))
	{
		return false;
	}
	m_mapOutputShader->SetActive();
	// スクリーン用の行列を作成 (UIやスプライトは以降この行列を基準に描画)
	m_mapOutputShader->SetMatrixUniform("u_viewProj", viewProj);

	return true;
}

// シェーダーのuniformに値をセット
void Renderer::SetLightUniforms(Shader * in_shader)
{
	// ビュー行列からカメラ位置を逆算出する
	Matrix4 invView = m_view;
	invView.Invert();
	in_shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	in_shader->SetVectorUniform("u_viewPos", invView.GetTranslation());

	// アンビエントライト
	in_shader->SetVectorUniform("u_ambientLight", m_ambientLight);

	// ディレクショナルライト
	in_shader->SetVectorUniform("uDirLight.mPosition", m_directionalLight.position);
	in_shader->SetVectorUniform("uDirLight.mDirection", m_directionalLight.direction);
	in_shader->SetVectorUniform("uDirLight.mDiffuseColor", m_directionalLight.diffuse);
	in_shader->SetVectorUniform("uDirLight.mSpecColor", m_directionalLight.specular);
}
