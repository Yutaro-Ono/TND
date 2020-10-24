//-----------------------------------------------------------------------+
// ���b�V���N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"

#include <SDL_log.h>
#include "Math.h"





Mesh::Mesh()
	:m_vertexArray(nullptr)
	,m_radius(0.0f)
	,m_specValue(100.0f)
	,m_diffuseMap(nullptr)
	,m_specularMap(nullptr)
	,m_normalMap(nullptr)
{
}

Mesh::~Mesh()
{
}


void Mesh::Delete()
{
	delete m_vertexArray;
	m_vertexArray = nullptr;
}

Texture * Mesh::GetTexture(size_t in_index)
{

	if (in_index < m_textures.size())
	{
		return m_textures[in_index];
	}
	else
	{
		return nullptr;
	}

}

// ���͂��ꂽ���b�V���t�@�C��������f�B�t���[�Y�}�b�v�A�X�y�L�����[�}�b�v�A�@���}�b�v�����[�h�E�ǉ�����
void Mesh::AddTexture(const std::string& in_meshName, class Renderer* in_renderer)
{
	std::string pngPath = in_meshName;

	int extNum = pngPath.find_last_of(".");
	pngPath = pngPath.substr(0, extNum);            // �g���q�ȊO�̃��b�V���t�@�C�������擾

	// �f�B�t���[�Y�ǂݍ���
	Texture* t = in_renderer->GetTexture(pngPath + "_DiffuseMap.png");

	if (t != nullptr)
	{
		m_diffuseMap = t;
		m_textures.emplace_back(t);
	}
	else
	{
		std::cout << "Obj Mesh : Load [Diffuse] Texture Error\n";
	}

	// �X�y�L�����ǂݍ���
	t = nullptr;
	t = in_renderer->GetTexture(pngPath + "_SpecularMap.png");

	if (t != nullptr)
	{
		m_specularMap = t;
		m_textures.emplace_back(t);
	}
	else
	{
		std::cout << "Obj Mesh : Load [Specular] Texture Error\n";
	}

	// �@���}�b�v�ǂݍ���
	t = nullptr;
	t = in_renderer->GetTexture(pngPath + "_NormalMap.png");

	if (t != nullptr)
	{
		m_normalMap = t;
		m_textures.emplace_back(t);
	}
	else
	{
		std::cout << "Obj Mesh : Load [Normal] Texture Error\n";
	}


}
