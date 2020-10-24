//-----------------------------------------------------------------------+
// ���b�V���N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include "VertexArray.h"
#include "tiny_obj_loader.h"
#include "Collision.h"
#include "Renderer.h"

namespace
{
	union Vertex
	{
		float f;
		uint8_t b[4];
	};
}

class Mesh
{
public:

	Mesh();
	~Mesh();

	virtual bool Load(const std::string& in_fileName, class Renderer* in_renderer) = 0;       // ���b�V���̃��[�h
	void Delete();                                                                // ���b�V���̉��

	// �e�N�X�`���̒ǉ� (���b�V���t�@�C���Ɠ��K�w�̃f�B�t���[�Y�E�X�y�L�����E�m�[�}�����擾)
	void AddTexture(const std::string& in_meshName, class Renderer* in_renderer);

	class VertexArray* GetVertexArray() { return m_vertexArray; }                 // ���b�V���̒��_�z��̎擾
	class Texture* GetTexture(size_t in_index);                                      // �w�肳�ꂽ�C���f�b�N�X����e�N�X�`���擾

	const std::string& GetShaderName() const { return m_shaderName; }             // �V�F�[�_�[�����擾
	const AABB& GetCollisionBox() const { return m_box; }                         // �����蔻��{�b�N�X���擾

	float GetRadius() const { return m_radius; }                                  // �o�E���f�B���O�X�t�B�A�̔��a���擾

	// �e��e�N�X�`���̎擾
	
	class Texture* GetDiffuseMap() { return m_diffuseMap; }
	class Texture* GetSpecularMap() { return m_specularMap; }
	class Texture* GetNormalMap() { return m_normalMap; }


protected:

	AABB m_box;

	std::vector<class Texture*> m_textures;
	class Texture* m_diffuseMap;                                // �f�B�t���[�Y�}�b�v
	class Texture* m_specularMap;                               // �X�y�L�����}�b�v
	class Texture* m_normalMap;                                 // �m�[�}���}�b�v

	class VertexArray* m_vertexArray;                                       // ���b�V���̒��_�z��

	std::string m_shaderName;
	float m_radius;
	float m_specValue;

};