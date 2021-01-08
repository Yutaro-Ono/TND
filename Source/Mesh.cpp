//-----------------------------------------------------------------------+
// ���b�V���N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "ShadowMap.h"
#include <SDL_log.h>
#include "Math.h"

Mesh::Mesh()
	:m_vertexArray(nullptr)
	,m_radius(0.0f)
	,m_specValue(100.0f)
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

/// <summary>
/// ���͂��ꂽ���b�V�����ɑΉ�����e��e�N�X�`���p�X���쐬����
/// �^�C�v���ƂɊe��e�N�X�`���ԍ���A�z�z��֊i�[����
/// �e�N�X�`����������Ȃ������ꍇ�A�����ԍ��ł���"0"��������
/// </summary>
/// <param name="in_meshName"> ���b�V���t�@�C���̃p�X </param>
/// <param name="in_renderer"> �����_���[�N���X�B���������������Ȃ��悤GetTexture�֐����g�p���邽�� </param>
void Mesh::AddTexture(const std::string& in_meshName, class Renderer* in_renderer)
{
	std::string filePath = in_meshName;

	int extNum = filePath.find_last_of(".");          // �t�@�C���p�X��"."�܂ł̕��������擾
	filePath = filePath.substr(0, extNum);            // �g���q���������t�@�C�������擾

	// �e��e�N�X�`���̎擾
	m_textureStages.emplace(TEXTURE_TYPE::DIFFUSE_MAP, CreateTextureStage(TEXTURE_TYPE::DIFFUSE_MAP, filePath));
	m_textureStages.emplace(TEXTURE_TYPE::SPECULAR_MAP, CreateTextureStage(TEXTURE_TYPE::SPECULAR_MAP, filePath));
	m_textureStages.emplace(TEXTURE_TYPE::NORMAL_MAP, CreateTextureStage(TEXTURE_TYPE::NORMAL_MAP, filePath));
	m_textureStages.emplace(TEXTURE_TYPE::EMISSIVE_MAP, CreateTextureStage(TEXTURE_TYPE::EMISSIVE_MAP, filePath));

}


/// <summary>
/// �w�肵���^�C�v�̃e�N�X�`�����쐬�E�܂��͎擾���A�e�N�X�`���X�e�[�W�ԍ���Ԃ�
/// </summary>
/// <param name="in_type"> �e�N�X�`���^�C�v </param>
/// <param name="in_filePath"> �ǉ��������e�N�X�`���̐e�p�X </param>
/// <returns> GL�ɓo�^���ꂽ�e�N�X�`���ԍ� </returns>
int Mesh::CreateTextureStage(TEXTURE_TYPE in_type, std::string& in_filePath)
{
	std::string mapPath;
	if (in_type == TEXTURE_TYPE::DIFFUSE_MAP)
	{
		mapPath = in_filePath + "_DiffuseMap.png";
	}
	if (in_type == TEXTURE_TYPE::SPECULAR_MAP)
	{
		mapPath = in_filePath + "_SpecularMap.png";
	}
	if (in_type == TEXTURE_TYPE::NORMAL_MAP)
	{
		mapPath = in_filePath + "_NormalMap.png";
	}
	if (in_type == TEXTURE_TYPE::EMISSIVE_MAP)
	{
		mapPath = in_filePath + "_EmissiveMap.png";
	}

	Texture* t = RENDERER->GetTexture(mapPath);

	if (t != nullptr)
	{
		return t->GetTextureID();
	}

	std::cout << "Load : " << mapPath << " : Not Found\n";
	return 0;
}

//----------------------------------------------------------------------------------------+
// �^���W�F���g�x�N�g���v�Z
// �O�p�`�ʂ�pos1�`pos3, uv0�`uv2 ���g����Tangent�x�N�g����destTangent�ɕԂ�
// 
// inout destTangent Tangent���i�[����ϐ�
// in : pos0, pos1, pos2 �O�p�`�ʂ̒��_
// in : uv0, uv1, uv2 �O�p�`�ʂ̒��_�ɑΉ�����UV���W
//----------------------------------------------------------------------------------------+
void Mesh::calcTangent(Vector3& destTangent, const Vector3& pos1, const Vector3& pos2, const Vector3& pos3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3)
{
	Vector3 edge1, edge2;
	edge1 = pos2 - pos1;
	edge2 = pos3 - pos1;
	Vector2 deltaUV1 = uv2 - uv1;
	Vector2 deltaUV2 = uv3 - uv1;
	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	destTangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	destTangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	destTangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	destTangent.Normalize();
}

//----------------------------------------------------------------------------------------+
// ���_�z�񂩂璸�_���W�x�N�g�����������o��
// inout : destUV ���_��UV���W���i�[����ϐ�
// in : fv ���_&UV�f�[�^�z��̐擪�A�h���X���i�[���ꂽ�|�C���^
// index ���_&UV�f�[�^�z��̃C���f�b�N�X�l
//----------------------------------------------------------------------------------------+
void Mesh::getPosVec(Vector3& destPos, const std::vector<float> fv, int index)
{
	destPos.x = fv[index * 11 + 0];
	destPos.y = fv[index * 11 + 1];
	destPos.z = fv[index * 11 + 2];
}

//----------------------------------------------------------------------------------------+
// ���_�z�񂩂�UV�l���������o��
// inout : destUV ���_��UV���W���i�[����ϐ�
// in : fv ���_&UV�f�[�^�z��̐擪�A�h���X���i�[���ꂽ�|�C���^
// index ���_&UV�f�[�^�z��̃C���f�b�N�X�l
//----------------------------------------------------------------------------------------+
void Mesh::getUVVec(Vector2& destUV, const std::vector<float> fv, int index)
{
	destUV.x = fv[index * 11 + 6 + 0];
	destUV.y = fv[index * 11 + 6 + 1];
}

//----------------------------------------------------------------------------------------+
// ���_��Tangent����������
// inout : destfv ���_��UV&Tangent �z��̐擪�A�h���X
// in : index ���_��UV&Tangent �z��̏������ݐ�C���f�b�N�X
// in tangent ���_��UV&Tangent �z��ɏ������݂��� tangent�l
//----------------------------------------------------------------------------------------+
void Mesh::setTangent(std::vector<float>& destfv, int index, const Vector3& tangent)
{
	if (destfv.size() > index * 11 + 8 + 2)
	{
		destfv[index * 11 + 8 + 0] = tangent.x;
		destfv[index * 11 + 8 + 1] = tangent.y;
		destfv[index * 11 + 8 + 2] = tangent.z;
	}

}