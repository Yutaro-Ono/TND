//-----------------------------------------------------------------------+
// ���b�V���N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
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

// �e�N�X�`���^�C�v
enum class TEXTURE_TYPE
{
	DIFFUSE_MAP,     // �f�B�t���[�Y (�x�[�X�J���\)
	SPECULAR_MAP,    // �X�y�L���� (���ˌ�)
	NORMAL_MAP,      // �m�[�}�� (�@��)
	EMISSIVE_MAP     // �G�~�b�V�u (���ˌ�)
};

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
	class Texture* GetTexture(TEXTURE_TYPE in_type);

	const std::string& GetShaderName() const { return m_shaderName; }             // �V�F�[�_�[�����擾
	const AABB& GetCollisionBox() const { return m_box; }                         // �����蔻��{�b�N�X���擾

	float GetRadius() const { return m_radius; }                                  // �o�E���f�B���O�X�t�B�A�̔��a���擾


	// �e�N�X�`���z��̃T�C�Y�擾
	int GetTextureArraySize() { return m_textureStages.size(); }

	// �w�肵���^�C�v�̃e�N�X�`���ԍ����擾 (�`�掞�Ɏg�p)
	int GetTextureID(TEXTURE_TYPE in_type) { return m_textureStages[in_type]; }


	// �w�肵���^�C�v�̃e�N�X�`�����擾���A�e�N�X�`���ԍ���Ԃ�
	int CreateTextureStage(TEXTURE_TYPE in_type, std::string& in_filePath);

	// �^���W�F���g�X�y�[�X�v�Z�p�֐�
	void calcTangent(Vector3& destTangent, const Vector3& pos1, const Vector3& pos2, const Vector3& pos3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3);
	void getPosVec(Vector3& destPos, const std::vector<float> fv, int index);
	void getUVVec(Vector2& destUV, const std::vector<float> fv, int index);
	void setTangent(std::vector<float>& destfv, int index, const Vector3& tangent);


protected:

	AABB m_box;

	std::unordered_map<TEXTURE_TYPE, int> m_textureStages;      // key:�e�N�X�`���^�C�v  val:�e�N�X�`���X�e�[�W(GL��Ŋ���U��ꂽ�ԍ�)

	class VertexArray* m_vertexArray;                           // ���b�V���̒��_�z��

	std::string m_shaderName;
	float m_radius;
	float m_specValue;

};