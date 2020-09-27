//-----------------------------------------------------------------------+
// テクスチャクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+

// インクルードファイル
#include "Texture.h"
#include "GameMain.h"
#include "Renderer.h"
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


// コンストラクタ
Texture::Texture()
	:m_textureID(0)
	,m_width(0)
	,m_height(0)
{
	// 処理なし
}

// デストラクタ
Texture::~Texture()
{
	// 処理なし
}

// テクスチャのロード
bool Texture::Load(const std::string & in_fileName)
{

	// SDLサーフェス(面)をテクスチャから作成
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(in_fileName.c_str());          // ロード
	// ロードに失敗したら
	if (!surface)
	{
		SDL_Log("Texture Load Failed : %s", in_fileName.c_str());
		return false;
	}

	// サーフェスからテクスチャを生成
	texture = SDL_CreateTextureFromSurface(GAME_INSTANCE.GetSDLRenderer(), surface);
	// 生成に失敗したら
	if (!texture)
	{
		SDL_Log("Create Texture From Surface Failed : %s", in_fileName.c_str());
		return false;
	}

	// テクスチャ画像の幅、高さを取得
	m_width = surface->w;
	m_height = surface->h;
	int channels = surface->format->BytesPerPixel;

	// OpenGLにテクスチャを登録
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	// テクスチャオブジェクトの生成
	// 2次元としてバインド
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	// テクスチャの割り当て
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

	// SDLサーフェスは不要なので解放しておく
	SDL_FreeSurface(surface);

	// テクスチャのフィルタリング設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ロードに成功
	return true;
}

// テクスチャ解放処理
void Texture::Delete()
{
	RENDERER->RemoveTexture(this);

	glDeleteTextures(1, &m_textureID);
}

// サーフェスからテクスチャを生成
void Texture::CreateSurface(SDL_Surface * surface)
{
	m_width = surface->w;
	m_height = surface->h;

	// テクスチャを生成し、OpenGL側でタグ付けされたIDを返す
	glGenTextures(1, &m_textureID);
	// IDに設定されたテクスチャを2Dとして読み込む
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	/*   ↓ 以降は上で生成されたIDを基に設定していく ↓   */
	
	// テクスチャの割り当て
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	// テクスチャのフィルタリング設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// 描画時のアクティブ化
void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}
