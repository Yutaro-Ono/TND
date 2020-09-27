//-----------------------------------------------------------------------+
// フォントクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

// インクルードファイル
#include <string>
#include <SDL_ttf.h>
#include <unordered_map>
#include "Math.h"

class Font
{
public:

	Font();                                   // コンストラクタ
	~Font();                                  // デストラクタ

	// ファイルのロード/アンロード
	bool Load(const std::string& in_fileName);
	void UnLoad();

	// 指定した文字列をテクスチャに描画する
	class Texture* RenderText(const std::string& in_text, const Vector3& in_color = Color::White, int in_ptSize = 30);

private:

	// ポイントサイズとフォントデータの連想配列
	std::unordered_map<int, TTF_Font*> m_fontData;
};