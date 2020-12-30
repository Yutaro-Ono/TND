//-----------------------------------------------------------------------+
// テクスチャクラス
// 機能：SDLサーフェスよりテクスチャをロード・生成し、システムへ紐づける
// 特性：なし
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <string>
#include <vector>

class Texture
{


public:

	Texture();                                                    // コンストラクタ
	~Texture();                                                   // デストラクタ

	bool Load(const std::string& in_fileName);                    // テクスチャのロード
	bool LoadCubeMap(const std::string& in_filePath);              // キューブマップのロード
	void Delete();                                                // テクスチャの解放処理
	void CreateSurface(struct SDL_Surface* surface);              // SDLサーフェスからテクスチャを作成する

	void SetActive();                                             // テクスチャをアクティブ(ポリゴン描画で使用)にする

	void SetMipMap();                                             // ミップマップ有効化

	void SetWidth(int in_width) { m_width = in_width; }           // 横幅のセッター


	//---------------------------------------------------+
	// Getter / Setter
	//---------------------------------------------------+
	const int& GetWidth() const { return m_width; }               // テクスチャの幅取得
	const int& GetHalfWidth() const { return m_width / 2; }       // テクスチャの幅(/ 2)取得

	const int& GetHeight() const { return m_height; }             // テクスチャの高さ取得
	const int& GetHalfHeight() const { return m_height / 2; }     // テクスチャの高さ(/ 2)取得

	int GetTextureID() const { return m_textureID; }              // テクスチャID
	void SetTextureID(unsigned int in_texID) { m_textureID = in_texID; }

private:

	unsigned int m_textureID;                                     // GLに登録されたテクスチャの番号

	int m_width, m_height;                                        // テクスチャの縦幅サイズ


};