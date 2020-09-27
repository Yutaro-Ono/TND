#include "Font.h"
#include "Texture.h"
#include "GameMain.h"
#include <vector>>

Font::Font()
{
}

Font::~Font()
{
	UnLoad();
}

// フォントデータのロード
bool Font::Load(const std::string & in_fileName)
{
	// サポートするフォントサイズ
	std::vector<int> fontSize =
	{
		8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32,
		34, 36, 38, 40, 42, 44, 46, 48, 52, 56, 60, 64, 68, 72,
	};

	// サイズごとにTTF_OpenFontを1回ずつ呼び出す
	for (auto& size : fontSize)
	{
		TTF_Font* font = TTF_OpenFont(in_fileName.c_str(), size);

		// フォントが開けなかった
		if (font == nullptr)
		{
			SDL_Log("Font %s : Size %d : Load Failed", in_fileName.c_str(), size);
			return false;
		}

		m_fontData.emplace(size, font);
	}

	return true;
}

// フォントデータのアンロード
void Font::UnLoad()
{
	for (auto& font : m_fontData)
	{
		TTF_CloseFont(font.second);
	}
}

// 指定した文字列をテクスチャに描画する
Texture * Font::RenderText(const std::string & in_text, const Vector3 & in_color, int in_ptSize)
{
	Texture* texture = nullptr;

	// 色をSDL_Colorに変換する
	SDL_Color color;

	color.r = static_cast<Uint8>(in_color.x * 255);
	color.g = static_cast<Uint8>(in_color.y * 255);
	color.b = static_cast<Uint8>(in_color.z * 255);
	color.a = 255;

	// 指定サイズのフォントデータを探す
	auto iter = m_fontData.find(in_ptSize);

	if (iter != m_fontData.end())
	{
		TTF_Font* font = iter->second;
		const std::string& actualText = GAME_INSTANCE.GetText(in_text);
		// SDL_Surfaceに描画
		SDL_Surface* surf = TTF_RenderText_Blended(font, in_text.c_str(), color);

		if (surf != nullptr)
		{
			// SDL_surfaceからテクスチャに変換する
			texture = new Texture();
			texture->CreateSurface(surf);

			SDL_FreeSurface(surf);
		}
	}
	else
	{
		SDL_Log("PointSize %d : Not Compatible", in_ptSize);
	}


	return texture;
}
