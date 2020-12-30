//-----------------------------------------------------------------------+
// �e�N�X�`���N���X
// �@�\�FSDL�T�[�t�F�X���e�N�X�`�������[�h�E�������A�V�X�e���֕R�Â���
// �����F�Ȃ�
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <string>
#include <vector>

class Texture
{


public:

	Texture();                                                    // �R���X�g���N�^
	~Texture();                                                   // �f�X�g���N�^

	bool Load(const std::string& in_fileName);                    // �e�N�X�`���̃��[�h
	bool LoadCubeMap(const std::string& in_filePath);              // �L���[�u�}�b�v�̃��[�h
	void Delete();                                                // �e�N�X�`���̉������
	void CreateSurface(struct SDL_Surface* surface);              // SDL�T�[�t�F�X����e�N�X�`�����쐬����

	void SetActive();                                             // �e�N�X�`�����A�N�e�B�u(�|���S���`��Ŏg�p)�ɂ���

	void SetMipMap();                                             // �~�b�v�}�b�v�L����

	void SetWidth(int in_width) { m_width = in_width; }           // �����̃Z�b�^�[


	//---------------------------------------------------+
	// Getter / Setter
	//---------------------------------------------------+
	const int& GetWidth() const { return m_width; }               // �e�N�X�`���̕��擾
	const int& GetHalfWidth() const { return m_width / 2; }       // �e�N�X�`���̕�(/ 2)�擾

	const int& GetHeight() const { return m_height; }             // �e�N�X�`���̍����擾
	const int& GetHalfHeight() const { return m_height / 2; }     // �e�N�X�`���̍���(/ 2)�擾

	int GetTextureID() const { return m_textureID; }              // �e�N�X�`��ID
	void SetTextureID(unsigned int in_texID) { m_textureID = in_texID; }

private:

	unsigned int m_textureID;                                     // GL�ɓo�^���ꂽ�e�N�X�`���̔ԍ�

	int m_width, m_height;                                        // �e�N�X�`���̏c���T�C�Y


};