//-----------------------------------------------------------------------+
// �t�H���g�N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

// �C���N���[�h�t�@�C��
#include <string>
#include <SDL_ttf.h>
#include <unordered_map>
#include "Math.h"

class Font
{
public:

	Font();                                   // �R���X�g���N�^
	~Font();                                  // �f�X�g���N�^

	// �t�@�C���̃��[�h/�A�����[�h
	bool Load(const std::string& in_fileName);
	void UnLoad();

	// �w�肵����������e�N�X�`���ɕ`�悷��
	class Texture* RenderText(const std::string& in_text, const Vector3& in_color = Color::White, int in_ptSize = 30);

private:

	// �|�C���g�T�C�Y�ƃt�H���g�f�[�^�̘A�z�z��
	std::unordered_map<int, TTF_Font*> m_fontData;
};