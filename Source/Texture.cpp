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
#include <SDL.h>
#include <SDL_image.h>
#include <stb_image.h>

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

// 6つの面を持つキューブマップ用テクスチャの生成
bool Texture::LoadSkyBox(const std::string& in_filePath)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

	SDL_Surface* surface;         // ロード


	int channels;

	// 各スカイボックス用画像のパス
	std::string faces[] =
	{
		in_filePath + "right.jpg",
		in_filePath + "left.jpg",
		in_filePath + "top.jpg",
		in_filePath + "bottom.jpg",
		in_filePath + "front.jpg",
		in_filePath + "back.jpg"
	};

	for (int i = 0; i < 6; i++)
	{
		std::string str;
		if (i == 0)
		{
			str = in_filePath + "right.jpg";
		}
		if (i == 1)
		{
			str = in_filePath + "left.jpg";
		}
		if (i == 2)
		{
			str = in_filePath + "top.jpg";

		}
		if (i == 3)
		{
			str = in_filePath + "bottom.jpg";
		}
		if (i == 4)
		{
			str = in_filePath + "front.jpg";
		}
		if (i == 5)
		{
			str = in_filePath + "back.jpg";
		}


		surface = IMG_Load(str.c_str());

		// テクスチャ画像の幅、高さを取得
		m_width = surface->w;
		m_height = surface->h;
		int channels = surface->format->BytesPerPixel;


		if (surface)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
		}
		else
		{
			std::cout << "CubeMap Texture Load Failed at Path : " << str << std::endl;
		}

		// SDLサーフェスは不要なので解放しておく
		SDL_FreeSurface(surface);

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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
