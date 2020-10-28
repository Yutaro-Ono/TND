//-----------------------------------------------------------------------+
// レンダラークラス
// 機能：描画に関わる全ての情報を保有し、描画処理も行う
// 特性：なし
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

// ディレクショナルライト構造体(平行ライト:シーン全体に降り注ぐ光)
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

	bool Initialize(int in_screenW, int in_screenH, bool in_full);          // 各種初期化処理 (GL SDL)
	void Delete();                                                          // 各種解放処理
	void Draw();                                                            // 描画

	// SpriteComponent
	void AddSprite(class SpriteComponent* in_sprite);
	void RemoveSprite(class SpriteComponent* in_sprite);
	const std::vector<class SpriteComponent*>& GetSpriteStack() const { return m_spriteComponents; }
	// Sprite (ワールド空間)
	void AddSpriteWorld(class WorldSpaceUI* in_sprite);
	void RemoveSpriteWorld(class WorldSpaceUI* in_sprite);

	// MeshComponent
	void AddMeshComponent(class MeshComponent* in_mesh);                    // メッシュコンポーネントの追加
	void RemoveMeshComponent(class MeshComponent* in_mesh);                 // メッシュコンポーネントの削除
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
	void SetProjectionMatrix(const Matrix4& in_proj) { m_projection = in_proj; }


private:

	void CreateCubeMapVerts();                                             // キューブマップ(スカイボックス用)頂点配列定義
	void CreateSpriteVerts();                                              // スプライト用頂点配列定義
	void CreateWorldSpriteVerts();                                            // パーティクル用頂点配列定義
	bool LoadShaders();                                                    // シェーダーの初期化
	void SetLightUniforms(class Shader* in_shader);                        // ライト値をシェーダーにセット

	int m_screenWidth, m_screenHeight;                                     // ウィンドウの横幅、縦幅
	bool m_fullScreen;                                                     // フルスクリーンかどうか

	std::unordered_map<std::string, class Texture*> m_textures;            // テクスチャ登録配列
	std::unordered_map<std::string, class Mesh*> m_meshes;                 // メッシュ登録配列
	
	// スケルトン・アニメーションの登録用配列(スケルタルメッシュのみ)
	std::unordered_map<std::string, class Skeleton*> m_skeletons;
	std::unordered_map<std::string, class Animation*> m_animations;

	std::vector<class MeshComponent*> m_meshComponents;                    // メッシュコンポーネント
	std::vector<class CubeMapComponent*> m_skyBoxComponents;                  // スカイボックス用メッシュコンポーネント
	std::vector<class SkeletalMeshComponent*> m_skeletalMeshComponents;

	//--------------------------------------------+
	// ライティング関連
	//--------------------------------------------+
	Vector3 m_ambientLight;                                                // アンビエントライト
	DirectionalLight m_directionalLight;                                   // ディレクショナルライト

	//--------------------------------------------+
	// シャドウ関連
	//--------------------------------------------+
	class ShadowMap* m_shadowMap;


	//--------------------------------------------+
	// Sprite関連
	//--------------------------------------------+
	class VertexArray* m_spriteVerts;                                      // スプライト用頂点配列
	class Shader* m_spriteShader;                                          // スプライト基本シェーダ
	class Shader* m_worldSpaceSpriteShader;                                // ワールド空間上のスプライトシェーダ
	std::vector<class SpriteComponent*> m_spriteComponents;                // スプライト配列 (スクリーン空間)
	std::vector<class WorldSpaceUI*> m_worldSprites;                       // スプライト配列 (ワールド空間)

	//--------------------------------------------+
	// メッシュ関連
	//--------------------------------------------+
	class Shader* m_meshShader;
	class Shader* m_meshNormalShader;
	class Shader* m_skinnedShader;
	int m_switchShader;

	//--------------------------------------------+
	// パーティクル関連
	//--------------------------------------------+
	class Shader* m_particleShader;
	class VertexArray* m_particleVertex;
	class ParticleManager* m_particleManager;

	//--------------------------------------------+
	// スカイボックス関連
	//--------------------------------------------+
	class Shader* m_skyboxShader;          // スカイボックス用シェーダー
	class VertexArray* m_skyboxVerts;      // スカイボックス用頂点配列

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
	Matrix4 m_projection;     // プロジェクション(視錐台)
	//AABB m_projCullingZone;   // 視錐台範囲外かどうかを検出する

	//--------------------------------------------+
	// レンダリングベース情報関連
	//--------------------------------------------+
	SDL_Window* m_window;                                                   // SDLのウィンドウハンドル
	SDL_Renderer* m_SDLRenderer;                                            // SDLのレンダリングハンドル
	SDL_GLContext m_context;                                                // OpenGLコンテキスト (内部状態管理)
	HWND m_hwnd;                                                            // ウィンドウハンドル  

};