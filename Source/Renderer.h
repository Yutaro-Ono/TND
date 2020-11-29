//----------------------------------------------------------------------------------------------+
// レンダラークラス
// 機能：描画に関わる全ての情報を保有する。描画はForwardかDefferedレンダラークラスを使用する
// 特性：なし
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

// ディレクショナルライト構造体(平行ライト:シーン全体に降り注ぐ光)
typedef struct DirectionalLight
{
	Vector3 position;      // 光源位置
	Vector3 target;        // ターゲット(方向を割り出しやすくするため)
	Vector3 direction;     // ターゲット - 光源位置
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

	bool Initialize(int in_screenW, int in_screenH, bool in_full);          // 各種初期化処理 (GL SDL)
	void Delete();                                                          // 各種解放処理
	void Draw();                                                            // 描画

	// SpriteComponent
	void AddSprite(class SpriteComponent* in_sprite);
	void RemoveSprite(class SpriteComponent* in_sprite);
	// Sprite (ワールド空間)
	void AddSpriteWorld(class WorldSpaceUI* in_sprite);
	void RemoveSpriteWorld(class WorldSpaceUI* in_sprite);
	// MeshComponent
	void AddMeshComponent(class MeshComponent* in_mesh);                    // メッシュコンポーネントの追加
	void RemoveMeshComponent(class MeshComponent* in_mesh);                 // メッシュコンポーネントの削除
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
	SDL_Renderer* GetSDLRenderer() { return m_SDLRenderer; }                // SDL刑の描画に用いるSDLRendererのゲッター
	SDL_Window* GetSDLWindow() { return m_window; }                         // SDL Windowのゲッター
	SDL_GLContext GetSDLGLContext() { return m_context; }                   // SDL_GLコンテキストのゲッター
	HWND GetHWND() { return m_hwnd; }                                       // HWNDのゲッター (ハンドルコントローラの初期化に使用)
	
	class Texture* GetTexture(const std::string& in_fileName);              // 指定したファイル名のテクスチャを返す
	class Mesh* GetMesh(const  std::string& in_fileName);                   // 指定したファイル名のメッシュを返す
	const class Skeleton* GetSkeleton(const std::string& in_fileName);             // 指定したファイル名のスケルタルメッシュの取得
	const class Animation* GetAnimation(const char* in_fileName, bool in_loop);
	// スクリーンサイズ
	float GetScreenWidth() { return static_cast<float>(m_screenWidth); }
	float GetScreenHeight() { return static_cast<float>(m_screenHeight); }
	// ディレクショナルライト
	DirectionalLight& GetDirectionalLight() { return m_directionalLight; }
	// カメラ位置セット
	void SetCameraPosition(const Vector3& in_cameraPos) { m_cameraPos = in_cameraPos; }
	// 行列
	const Matrix4& GetViewMatrix() { return m_view; }
	const Matrix4& GetProjectionMatrix() { return m_projection; }
	// ParticleManager
	class ParticleManager* GetParticleManager() const;

	void SetViewMatrix(const Matrix4& in_view) { m_view = in_view; }
	void SetAmbientLight(const Vector3& in_ambientColor) { m_ambientLight = in_ambientColor; }
	const Vector3 GetAmbientLight() { return m_ambientLight; }
	void SetProjectionMatrix(const Matrix4& in_proj) { m_projection = in_proj; }

	// アクティブスカイボックス
	void SetActiveSkyBox(class CubeMapComponent* in_skyBox) { m_activeSkyBox = in_skyBox; }
	class CubeMapComponent* GetSkyBox() { return m_activeSkyBox; }
	class VertexArray* GetCubeMapVerts() { return m_cubeVerts; }

	// スクリーン用頂点配列クラスのゲッター
	class VertexArray* GetScreenVAO() { return m_screenVerts; }


private:

	void CreateCubeVerts();                                                // キューブマップ(スカイボックス用)頂点配列定義
	void CreateSpriteVerts();                                              // スプライト用頂点配列定義
	void CreateWorldSpriteVerts();                                         // パーティクル用頂点配列定義
	void CreateScreenVerts();                                              // スクリーン用四角形頂点配列

	bool LoadShaders();                                                    // シェーダーの初期化
	void SetLightUniforms(class Shader* in_shader);                        // ライト値をシェーダーにセット

	int m_screenWidth, m_screenHeight;                                     // ウィンドウの横幅、縦幅
	bool m_fullScreen;                                                     // フルスクリーンかどうか

	std::unordered_map<std::string, class Texture*> m_textures;            // テクスチャ登録配列
	std::unordered_map<std::string, class Mesh*> m_meshes;                 // メッシュ登録配列
	
	// スケルトン・アニメーションの登録用配列(スケルタルメッシュのみ)
	std::unordered_map<std::string, class Skeleton*> m_skeletons;
	std::unordered_map<std::string, class Animation*> m_animations;

	// メッシュ配列
	std::vector<class MeshComponent*> m_meshComponents;                       // メッシュコンポーネント
	std::vector<class SkeletalMeshComponent*> m_skeletalMeshComponents;       // ボーン入りメッシュ配列
	std::vector<class MeshComponent*> m_glassMeshComponent;                   // ガラス(環境に影響を受ける)メッシュ配列
	std::vector<class EnvironmentMapComponent*> m_envMeshComponents;          // 環境マップオブジェクト配列
	// スプライト配列
	std::vector<class SpriteComponent*> m_spriteComponents;                // スプライト配列 (スクリーン空間)
	std::vector<class WorldSpaceUI*> m_worldSprites;                       // スプライト配列 (ワールド空間)
	
	class CubeMapComponent* m_activeSkyBox;                                // 有効な(描画する)スカイボックス

	// ライト関連配列
	std::vector<class PointLightComponent*> m_pointLights;                           // ポイントライト配列



	//--------------------------------------------+
	// シェーダオブジェクト
	//--------------------------------------------+
	class Shader* m_meshShader;                                            // 標準メッシュシェーダ
	class Shader* m_meshNormalShader;                                      // 法線マップメッシュシェーダ
	class Shader* m_skinnedShader;                                         // スキン(ボーン入り)メッシュシェーダ
	class Shader* m_environmentMapShader;                                  // 環境マップシェーダ

	class Shader* m_spriteShader;                                          // スプライト基本シェーダ
	class Shader* m_worldSpaceSpriteShader;                                // ワールド空間上のスプライトシェーダ
	class Shader* m_particleShader;
	class Shader* m_skyboxShader;                                          // スカイボックス用シェーダ

	//--------------------------------------------+
	// 頂点配列
	//--------------------------------------------+
	class VertexArray* m_spriteVerts;                                      // スプライト用頂点配列
	class VertexArray* m_particleVerts;                                    // パーティクル用頂点配列
	class VertexArray* m_cubeVerts;                                        // キューブ頂点配列
	class VertexArray* m_screenVerts;                                      // スクリーン全体を覆う四角形

	//--------------------------------------------+
	// ライティング関連
	//--------------------------------------------+
	Vector3 m_ambientLight;                                                // アンビエントライト
	DirectionalLight m_directionalLight;                                   // ディレクショナルライト構造体

	//--------------------------------------------+
	// シャドウ関連
	//--------------------------------------------+
	class ShadowMap* m_shadowMap;

	//--------------------------------------------+
    // HDR・Bloom関連
    //--------------------------------------------+
	class RenderBloom* m_bloom;
	int m_switchShader;

	//--------------------------------------------+
	// パーティクル関連
	//--------------------------------------------+
	class ParticleManager* m_particleManager;

	//--------------------------------------------+
	// ポストエフェクト関連
	//--------------------------------------------+
	class FrameBuffer* m_frameBuffer;

	//--------------------------------------------+
    // カメラ関連
    //--------------------------------------------+
	Vector3 m_cameraPos;

	//--------------------------------------------+
	// 基本行列関連
	//--------------------------------------------+
	Matrix4 m_view;
	Matrix4 m_projection;       // プロジェクション(視錐台)
	//AABB m_projCullingZone;   // 視錐台範囲外かどうかを検出する

	//--------------------------------------------+
	// アプリケーション情報関連
	//--------------------------------------------+
	SDL_Window* m_window;                                                   // SDLのウィンドウハンドル
	SDL_Renderer* m_SDLRenderer;                                            // SDLのレンダリングハンドル
	SDL_GLContext m_context;                                                // OpenGLコンテキスト (内部状態管理)
	HWND m_hwnd;                                                            // ウィンドウハンドル  

	// フォワード・ディファードレンダリングクラス (描画はどちらかで行う)
	class ForwardRenderer* m_fRenderer;
	class DefferedRenderer* m_dRenderer;
	RENDER_MODE m_renderMode;             // レンダリングモード (フォワードかディファードか)

	// フレンドクラス (フォワード・ディファードクラスはレンダラーに直接アクセス可能)
	friend class ForwardRenderer;
	friend class DefferedRenderer;
};