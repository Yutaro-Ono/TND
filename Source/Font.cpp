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

// �t�H���g�f�[�^�̃��[�h
bool Font::Load(const std::string & in_fileName)
{
	// �T�|�[�g����t�H���g�T�C�Y
	std::vector<int> fontSize =
	{
		8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32,
		34, 36, 38, 40, 42, 44, 46, 48, 52, 56, 60, 64, 68, 72,
	};

	// �T�C�Y���Ƃ�TTF_OpenFont��1�񂸂Ăяo��
	for (auto& size : fontSize)
	{
		TTF_Font* font = TTF_OpenFont(in_fileName.c_str(), size);

		// �t�H���g���J���Ȃ�����
		if (font == nullptr)
		{
			SDL_Log("Font %s : Size %d : Load Failed", in_fileName.c_str(), size);
			return false;
		}

		m_fontData.emplace(size, font);
	}

	return true;
}

// �t�H���g�f�[�^�̃A�����[�h
void Font::UnLoad()
{
	for (auto& font : m_fontData)
	{
		TTF_CloseFont(font.second);
	}
}

// �w�肵����������e�N�X�`���ɕ`�悷��
Texture * Font::RenderText(const std::string & in_text, const Vector3 & in_color, int in_ptSize)
{
	Texture* texture = nullptr;

	// �F��SDL_Color�ɕϊ�����
	SDL_Color color;

	color.r = static_cast<Uint8>(in_color.x * 255);
	color.g = static_cast<Uint8>(in_color.y * 255);
	color.b = static_cast<Uint8>(in_color.z * 255);
	color.a = 255;

	// �w��T�C�Y�̃t�H���g�f�[�^��T��
	auto iter = m_fontData.find(in_ptSize);

	if (iter != m_fontData.end())
	{
		TTF_Font* font = iter->second;
		const std::string& actualText = GAME_INSTANCE.GetText(in_text);
		// SDL_Surface�ɕ`��
		SDL_Surface* surf = TTF_RenderText_Blended(font, in_text.c_str(), color);

		if (surf != nullptr)
		{
			// SDL_surface����e�N�X�`���ɕϊ�����
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
