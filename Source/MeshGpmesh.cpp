#include "MeshGpmesh.h"



MeshGpmesh::MeshGpmesh()
{
}

MeshGpmesh::~MeshGpmesh()
{
}

bool MeshGpmesh::Load(const std::string& in_filePath, Renderer* in_renderer)
{
	std::ifstream file(in_filePath);
	if (!file.is_open())
	{
		printf("File Not Found : Mesh %s\n", in_filePath.c_str());
		return false;
	}

	// JSON�̉�͂��s��
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	// JSON�I�u�W�F�N�g���H
	if (!doc.IsObject())
	{
		printf("Mesh %s is not valid json", in_filePath.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// �o�[�W�����`�F�b�N
	if (ver != 1)
	{
		printf("Mesh %s not version 1", in_filePath.c_str());
		return false;
	}

	m_shaderName = doc["shader"].GetString();

	// ���_���C�A�E�g�ƃT�C�Y���t�@�C������Z�b�g
	VertexArray::Layout layout = VertexArray::POS_NORMAL_TEX_TAN;
	//size_t vertSize = 8;
	size_t vertSize = 11;     // �@���}�b�v������ꍇ vx,vy,vz,nx,ny,nz,u,v,tx,ty,tz

	std::string vertexFormat = doc["vertexformat"].GetString();
	if (vertexFormat == "PosNormSkinTex")
	{
		//layout = VertexArray::POS_NORMAL_SKIN_TEX;
		// This is the number of "Vertex" unions, which is 8 + 2 (for skinning)s�@1�̒��_�̏W���̐��@�W�@�{�@�Q�i�X�L�j���O���j
		// 3 (�ʒuxyz) + 3(�@��xyz) + 2(Bone�Əd�݁j�{�@2(UV)  �̌v�@10���i40byte) �@
		//vertSize = 10;
		
		layout = VertexArray::POS_NORMAL_SKIN_TEX_TAN;
		vertSize = 13;
	}

	// �e�N�X�`���̃��[�h
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		printf("Mesh %s has no textures, there should be at least one", in_filePath.c_str());
		//return false;
	}

	// �X�y�L�������x
	m_specValue = static_cast<float>(doc["specularPower"].GetDouble());

	// �e�N�X�`������
	AddTexture(in_filePath, in_renderer);

	// �@���}�b�v�^���W�F���g�v�Z�p
	// ���_���W�E�e�N�X�`��UV�i�[�p�z��
	std::vector<Vector3> destPos;
	std::vector<Vector2> uvPos;

	// ���_�ǂݍ���
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		printf("Mesh %s has no vertices", in_filePath.c_str());
		return false;
	}

	// ���_�f�[�^
	std::vector<Vertex> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	m_radius = 0.0f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{

		// �����_�ŁA�W�̗v�f(�ʒuxyz �@��xyz �e�N�X�`��uv�̗v�f�j�������Ă���
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray())
		{
			printf("Unexpected vertex format for %s", in_filePath.c_str());
			return false;
		}

		// ���_�ʒu����@�o�E���f�B���O�X�t�B�A�̔��a���v�Z����i���_����̋������ő�̂��̂𔼌a�Ƃ���j
		Vector3 pos(static_cast<float>(vert[0].GetDouble()),
			static_cast<float>(vert[1].GetDouble()),
			static_cast<float>(vert[2].GetDouble()));
		m_radius = Math::Max(m_radius, pos.LengthSq());
		// �o�E���f�B���O�{�b�N�X���v�Z
		if (i == 0)
		{
			m_box.InitMinMax(pos);
		}
		m_box.UpdateMinMax(pos);

		// ���_���C�A�E�g�� PosNormTex�Ȃ�i�{�[���������j
		if (layout == VertexArray::POS_NORMAL_TEX_TAN)
		{
			Vertex v;
			// float�l��ǉ�
			for (rapidjson::SizeType j = 0; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}


			// �|���S�����\�����钸�_���W���ꎞ�ۑ�
			destPos.push_back(Vector3(vertices[i * 11 + 0].f,
				vertices[i * 11 + 1].f,
				vertices[i * 11 + 2].f));
			// �e�N�X�`�����W���ꎞ�ۑ�
			uvPos.push_back(Vector2(vertices[i * 11 + 6].f,
				vertices[i * 11 + 7].f));

			Vector3 tangent;
			// �^���W�F���g�v�f��ǉ����Ă���
			for (int k = 0; k < 3; k++)
			{
				v.f = 0.0f;
				vertices.emplace_back(v);
			}

			// 3�񃋁[�v������(8 x 3)�^���W�F���g���v�Z����
			if (destPos.size() == 3)
			{
				calcTangent(tangent, destPos[0], destPos[1], destPos[2], uvPos[0], uvPos[1], uvPos[2]);
				for (int k = 2; k >= 0; k--)
				{
					vertices[(i - k) * 11 + 8 + 0].f = static_cast<float>(tangent.x);
					vertices[(i - k) * 11 + 8 + 1].f = static_cast<float>(tangent.y);
					vertices[(i - k) * 11 + 8 + 2].f = static_cast<float>(tangent.z);
				}

				destPos.clear();
				uvPos.clear();
			}


		}
		else if(layout == VertexArray::POS_NORMAL_SKIN_TEX)// �{�[���f�[�^����Ȃ�@PosNormSkinTex�Ȃ�
		{
			Vertex v;
			// ���_�Ɩ@����ǉ��@6��
			for (rapidjson::SizeType j = 0; j < 6; j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}

			// �X�L�����ǉ��i�{�[���ԍ�:unsigned char��1�o�C�g���j
			for (rapidjson::SizeType j = 6; j < 14; j += 4)  //���[�v�Ƃ��Ă�2��]����@1��]�ڂ̓{�[���ԍ��A2��]�ڂ̓{�[���E�F�C�g��int�Ƃ��Ď���Ă���i�g�p���ɕ���������������ۂ��j
			{
				v.b[0] = vert[j].GetUint();
				v.b[1] = vert[j + 1].GetUint();
				v.b[2] = vert[j + 2].GetUint();
				v.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(v);
			}

			// Add tex coords�@�e�N�X�`�����W
			for (rapidjson::SizeType j = 14; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}
		}
		else if(layout == VertexArray::POS_NORMAL_SKIN_TEX_TAN)// �{�[���f�[�^����Ȃ�@PosNormSkinTex�Ȃ�
		{

			Vertex v;
			// ���_�Ɩ@����ǉ��@6��
			for (rapidjson::SizeType j = 0; j < 6; j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}

			// �X�L�����ǉ��i�{�[���ԍ�:unsigned char��1�o�C�g���j
			for (rapidjson::SizeType j = 6; j < 14; j += 4)  //���[�v�Ƃ��Ă�2��]����@1��]�ڂ̓{�[���ԍ��A2��]�ڂ̓{�[���E�F�C�g��int�Ƃ��Ď���Ă���i�g�p���ɕ����������j
			{
				v.b[0] = vert[j].GetUint();
				v.b[1] = vert[j + 1].GetUint();
				v.b[2] = vert[j + 2].GetUint();
				v.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(v);
			}

			// Add tex coords�@�e�N�X�`�����W
			for (rapidjson::SizeType j = 14; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}

			// �|���S�����\�����钸�_���W���ꎞ�ۑ�
			destPos.push_back(Vector3(vertices[i * 13 + 0].f,
				vertices[i * 13 + 1].f,
				vertices[i * 13 + 2].f));
			// �e�N�X�`�����W���ꎞ�ۑ�
			uvPos.push_back(Vector2(vertices[i * 13 + 8].f,
				vertices[i * 13 + 9].f));

			Vector3 tangent;
			// �^���W�F���g�v�f��ǉ����Ă���
			for (int k = 0; k < 3; k++)
			{
				v.f = 0.0f;
				vertices.emplace_back(v);
			}

			// 3�񃋁[�v������(8 x 3)�^���W�F���g���v�Z����
			if (destPos.size() == 3)
			{
				calcTangent(tangent, destPos[1], destPos[2], destPos[0], uvPos[1], uvPos[2], uvPos[0]);
				for (int k = 2; k >= 0; k--)
				{

					v.f = static_cast<float>(tangent.x);
					vertices[(i - k) * 13 + 10 + 0] = v;
					v.f = static_cast<float>(tangent.y);
					vertices[(i - k) * 13 + 10 + 1] = v;
					v.f = static_cast<float>(tangent.z);
					vertices[(i - k) * 13 + 10 + 2] = v;
				}

				destPos.clear();
				uvPos.clear();
			}


		}
	}

	// �o�E���f�B���O�X�t�B�A�̔��a���v�Z
	m_radius = Math::Sqrt(m_radius);

	// ���_�C���f�b�N�X�����[�h
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		printf("Mesh %s has no indices", in_filePath.c_str());
		return false;
	}

	//���_�C���f�b�N�X�����������Ă���
	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			printf("Invalid indices for %s", in_filePath.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	if (layout == layout == VertexArray::POS_NORMAL_SKIN_TEX_TAN)
	{
		// ���_�z����쐬����
		m_vertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
			layout, indices.data(), static_cast<unsigned>(indices.size()));
		return true;
	}

	// ���_�z����쐬����
	m_vertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
		layout, indices.data(), static_cast<unsigned>(indices.size()));
	return true;
}
