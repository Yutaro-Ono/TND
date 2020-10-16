//-----------------------------------------------------------------------+
// �e�N�X�`���N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+

// �C���N���[�h�t�@�C��
#include "Texture.h"
#include "GameMain.h"
#include "Renderer.h"
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stb_image.h>

// �R���X�g���N�^
Texture::Texture()
	:m_textureID(0)
	,m_width(0)
	,m_height(0)
{
	// �����Ȃ�
}

// �f�X�g���N�^
Texture::~Texture()
{
	// �����Ȃ�
}

// �e�N�X�`���̃��[�h
bool Texture::Load(const std::string & in_fileName)
{

	// SDL�T�[�t�F�X(��)���e�N�X�`������쐬
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(in_fileName.c_str());          // ���[�h
	// ���[�h�Ɏ��s������
	if (!surface)
	{
		SDL_Log("Texture Load Failed : %s", in_fileName.c_str());
		return false;
	}

	// �T�[�t�F�X����e�N�X�`���𐶐�
	texture = SDL_CreateTextureFromSurface(GAME_INSTANCE.GetSDLRenderer(), surface);
	// �����Ɏ��s������
	if (!texture)
	{
		SDL_Log("Create Texture From Surface Failed : %s", in_fileName.c_str());
		return false;
	}

	// �e�N�X�`���摜�̕��A�������擾
	m_width = surface->w;
	m_height = surface->h;
	int channels = surface->format->BytesPerPixel;

	// OpenGL�Ƀe�N�X�`����o�^
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}
	
	// �e�N�X�`���I�u�W�F�N�g�̐���
	// 2�����Ƃ��ăo�C���h
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	// �e�N�X�`���̊��蓖��
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

	// SDL�T�[�t�F�X�͕s�v�Ȃ̂ŉ�����Ă���
	SDL_FreeSurface(surface);

	// �e�N�X�`���̃t�B���^�����O�ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ���[�h�ɐ���
	return true;
}

// 6�̖ʂ����L���[�u�}�b�v�p�e�N�X�`���̐���
bool Texture::LoadSkyBox(const std::string& in_filePath)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

	SDL_Surface* surface;         // ���[�h


	int channels;

	// �e�X�J�C�{�b�N�X�p�摜�̃p�X
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

		// �e�N�X�`���摜�̕��A�������擾
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

		// SDL�T�[�t�F�X�͕s�v�Ȃ̂ŉ�����Ă���
		SDL_FreeSurface(surface);

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return true;

}

// �e�N�X�`���������
void Texture::Delete()
{
	RENDERER->RemoveTexture(this);

	glDeleteTextures(1, &m_textureID);
}

// �T�[�t�F�X����e�N�X�`���𐶐�
void Texture::CreateSurface(SDL_Surface * surface)
{
	m_width = surface->w;
	m_height = surface->h;

	// �e�N�X�`���𐶐����AOpenGL���Ń^�O�t�����ꂽID��Ԃ�
	glGenTextures(1, &m_textureID);
	// ID�ɐݒ肳�ꂽ�e�N�X�`����2D�Ƃ��ēǂݍ���
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	/*   �� �ȍ~�͏�Ő������ꂽID����ɐݒ肵�Ă��� ��   */
	
	// �e�N�X�`���̊��蓖��
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	// �e�N�X�`���̃t�B���^�����O�ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// �`�掞�̃A�N�e�B�u��
void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}
