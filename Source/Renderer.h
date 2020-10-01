//-----------------------------------------------------------------------+
// レンダラークラス
// 機能：描画に関わる全ての情報を保有し、描画処理も行う
// 特性：なし
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <string>
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
	Vector3 m_position;
	Vector3 m_direction;
	Vector3 m_diffuseColor;
	Vector3 m_specColor;

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

	// MeshComponent
	void AddMeshComponent(class MeshComponent* in_mesh);                    // メッシュコンポーネントの追加
	void RemoveMeshComponent(class MeshComponent* in_mesh);                 // メッシュコンポーネントの削除
	void ShowResource();

	// TextureComponent
	void RemoveTexture(class Texture* in_texture);

	void WindowClear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
	void WindowFlip() { SDL_GL_SwapWindow(m_window); }

	void SetWindowTitle(const std::string& in_title);
	void SetParticleVertex();
	void SetDirectionalLight(const DirectionalLight& in_dirLight);

	//--------------------------------------------+
	// Getter / Setter
	//--------------------------------------------+
	SDL_Renderer* GetSDLRenderer() { return m_SDLRenderer; }                // SDL刑の描画に用いるSDLRendererのゲッター
	SDL_Window* GetSDLWindow() { return m_window; }                         // SDL Windowのゲッター
	SDL_GLContext GetSDLGLContext() { return m_context; }                   // SDL_GLコンテキストのゲッター
	class Texture* GetTexture(const std::string& in_fileName);              // 指定したファイル名のテクスチャを返す
	class Mesh* GetMesh(const  std::string& in_fileName);                   // 指定したファイル名のメッシュを返す
	// スクリーンサイズ
	float GetScreenWidth() { return static_cast<float>(m_screenWidth); }
	float GetScreenHeight() { return static_cast<float>(m_screenHeight); }
	// ディレクショナルライト
	DirectionalLight& GetDirectionalLight() { return m_directionalLight; }
	// 行列
	const Matrix4& GetViewMatrix() { return m_view; }
	const Matrix4& GetProjectionMatrix() { return m_projection; }
	// ParticleManager
	class ParticleManager* GetParticleManager() const;

	void SetViewMatrix(const Matrix4& in_view) { m_view = in_view; }
	void SetAmbientLight(const Vector3& in_ambientColor) { m_ambientLight = in_ambientColor; }
	void SetProjectionMatrix(const Matrix4& in_proj) { m_projection = in_proj; }


private:

	void CreateSpriteVerts();
	void CreateParticleVerts();
	bool LoadShaders();                                                    // シェーダーの初期化
	void SetLightUniforms(class Shader* in_shader);                        // ライト値をシェーダーにセット

	int m_screenWidth, m_screenHeight;                                     // ウィンドウの横幅、縦幅
	bool m_fullScreen;                                                     // フルスクリーンかどうか

	std::unordered_map<std::string, class Texture*> m_textures;            // テクスチャ登録配列
	std::unordered_map<std::string, class Mesh*> m_meshes;                 // メッシュ登録配列
	
	std::vector<class SpriteComponent*> m_spriteComponents;
	std::vector<class MeshComponent*> m_meshComponents;                    // メッシュコンポーネント

	//--------------------------------------------//
	// ライティング関連
	//-------------------------------------------//
	Vector3 m_ambientLight;                                                // アンビエントライト
	DirectionalLight m_directionalLight;                                   // ディレクショナルライト

	//-------------------------------------------//
	// Sprite関連
	//------------------------------------------//
	class Shader* m_spriteShader;
	class VertexArray* m_spriteVerts;

	//-------------------------------------------//
	// メッシュ関連
	//------------------------------------------//
	class Shader* m_meshShader;
	class Shader* m_skinnedShader;

	//-------------------------------------------//
	// パーティクル関連
	//------------------------------------------//
	class Shader* m_particleShader;
	class VertexArray* m_particleVertex;
	class ParticleManager* m_particleManager;

	//-------------------------------------------//
	// 基本行列関連
	//------------------------------------------//
	Matrix4 m_view;
	Matrix4 m_projection;

	//-------------------------------------------//
	// レンダリングベース情報関連
	//------------------------------------------//
	SDL_Window* m_window;                                                   // SDLのウィンドウハンドル
	SDL_Renderer* m_SDLRenderer;                                            // SDLのレンダリングハンドル
	SDL_GLContext m_context;                                                // OpenGLコンテキスト (内部状態管理)

};