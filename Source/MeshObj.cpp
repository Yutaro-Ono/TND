#include "MeshObj.h"

MeshObj::MeshObj()
{
}

MeshObj::~MeshObj()
{
}

bool MeshObj::Load(const std::string& in_filePath, Renderer* in_renderer)
{
	// ���_���C�A�E�g���Z�b�g(��{���b�V���^�C�v�ɌŒ�)
	VertexArray::Layout layout = VertexArray::POS_NORMAL_TEX_TAN;

	tinyobj::attrib_t attrib;                       // ���W�R���e�i
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;     // �}�e���A���R���e�i
	std::string warning, error;                     // �x���E�G���[���e�̊i�[

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, in_filePath.c_str()))
	{
		std::cerr << "ERROR: Category: Mesh: LoadObj " << std::endl;
		return false;
	}

	const int attribVertexNum = 3;       // ���_���W�̌�(x, y, z = 3��)
	const int attribNormalNum = 3;       // �@���x�N�g���̌�(nx, ny, nz = 3��)
	const int attribTexCoordNum = 2;     // �e�N�X�`�����W�̌�(u, v = 2��)
	const int attribTangentNum = 3;

	const int attribStride = attribVertexNum + attribNormalNum + attribTexCoordNum + attribTangentNum;     // �S���W�̌�

	int vertexNum = attrib.vertices.size() / attribVertexNum;     // ���_��

	// �C���f�b�N�X�o�b�t�@�̎擾
	std::vector<unsigned int> indices;
	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			indices.push_back(index.vertex_index);
		}
	}

	// �^���W�F���g��Ԍv�Z�p
	Vector3 tangent;
	// ���_�f�[�^���m��
	std::vector<float> vertexVec(vertexNum * attribStride);

	// ���_���W�E�e�N�X�`��UV�i�[�p�z��
	std::vector<Vector3> destPos;
	std::vector<Vector2> uvPos;

	// �`�󃋁[�v
	for (const auto& shape : shapes)
	{
		size_t indexOffset = 0;

		// ��(�O�p�`or�l�p�`)�̃��[�v
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
		{

			size_t num_vertices = shape.mesh.num_face_vertices[f];

			// ���_���W�E�e�N�X�`��UV�i�[�p�z��
			std::vector<Vector3> destPos;
			std::vector<Vector2> uvPos;

			// ���_�����[�v(��{3��)
			for (size_t v = 0; v < num_vertices; v++)
			{
				// �ʂ��\������C���f�b�N�X���擾
				tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];

				//// ���_���W�𒸓_�z��ɃR�s�[
				//vertexVec[idx.vertex_index * attribStride + 0] = attrib.vertices[3 * idx.vertex_index + 0];
				//vertexVec[idx.vertex_index * attribStride + 1] = attrib.vertices[3 * idx.vertex_index + 1];
				//vertexVec[idx.vertex_index * attribStride + 2] = attrib.vertices[3 * idx.vertex_index + 2];

				//// �@���f�[�^�𒸓_�z��ɃR�s�[
				//vertexVec[idx.vertex_index * attribStride + 3] = attrib.normals[3 * idx.normal_index + 0];
				//vertexVec[idx.vertex_index * attribStride + 4] = attrib.normals[3 * idx.normal_index + 1];
				//vertexVec[idx.vertex_index * attribStride + 5] = attrib.normals[3 * idx.normal_index + 2];

				//// uv�f�[�^�𒸓_�z��ɃR�s�[
				//vertexVec[idx.vertex_index * attribStride + 6] = attrib.texcoords[2 * idx.texcoord_index + 0];
				//vertexVec[idx.vertex_index * attribStride + 7] = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1];

				// ���_���W�𒸓_�z��ɃR�s�[
				vertexVec[idx.vertex_index * attribStride + 0] = attrib.vertices[3 * idx.vertex_index + 0];
				vertexVec[idx.vertex_index * attribStride + 1] = attrib.vertices[3 * idx.vertex_index + 2];
				vertexVec[idx.vertex_index * attribStride + 2] = attrib.vertices[3 * idx.vertex_index + 1];

				// �@���f�[�^�𒸓_�z��ɃR�s�[
				vertexVec[idx.vertex_index * attribStride + 3] = attrib.normals[3 * idx.normal_index + 0];
				vertexVec[idx.vertex_index * attribStride + 4] = attrib.normals[3 * idx.normal_index + 2];
				vertexVec[idx.vertex_index * attribStride + 5] = attrib.normals[3 * idx.normal_index + 1];

				// uv�f�[�^�𒸓_�z��ɃR�s�[
				vertexVec[idx.vertex_index * attribStride + 6] = attrib.texcoords[2 * idx.texcoord_index + 1];
				vertexVec[idx.vertex_index * attribStride + 7] = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 0];


				//-------------------------------------------------------------------------------------------------+
				// �����蔻��v�Z
				//-------------------------------------------------------------------------------------------------+
				// ���_�ʒu����@�o�E���f�B���O�X�t�B�A�̔��a���v�Z����i���_����̋������ő�̂��̂𔼌a�Ƃ���j
				Vector3 pos(vertexVec[idx.vertex_index * attribStride + 0],
					vertexVec[idx.vertex_index * attribStride + 1],
					vertexVec[idx.vertex_index * attribStride + 2]);
				m_radius = Math::Max(m_radius, pos.LengthSq());
				// �o�E���f�B���O�{�b�N�X���v�Z
				if (idx.vertex_index * attribStride + 0 == 0)
				{
					m_box.InitMinMax(pos);
				}
				m_box.UpdateMinMax(pos);


				// �|���S�����\�����钸�_���W���ꎞ�ۑ�
				destPos.push_back(Vector3(vertexVec[idx.vertex_index * attribStride + 0],
					vertexVec[idx.vertex_index * attribStride + 1],
					vertexVec[idx.vertex_index * attribStride + 2]));
				// �e�N�X�`�����W���ꎞ�ۑ�
				uvPos.push_back(Vector2(vertexVec[idx.vertex_index * attribStride + 6],
					vertexVec[idx.vertex_index * attribStride + 7]));
			}



			// �^���W�F���g�v�Z
			calcTangent(tangent, destPos[0], destPos[2], destPos[1], uvPos[0], uvPos[2], uvPos[1]);
			// �i�[����3�̒��_���W��UV����Z�o���ꂽ�^���W�F���g�𒸓_�z��Ɋi�[
			for (int i = 0; i < 3; i++)
			{
				setTangent(vertexVec, indexOffset + i, tangent);
				setTangent(vertexVec, indexOffset + i, tangent);
				setTangent(vertexVec, indexOffset + i, tangent);
			}
			destPos.clear();
			uvPos.clear();

			indexOffset += num_vertices;
		}
	}

	// ���_�z���GPU�ɓo�^
	m_vertexArray = new VertexArray(static_cast<float*>(vertexVec.data()), static_cast<unsigned>(vertexNum), layout, indices.data(), static_cast<unsigned>(indices.size()));


	// �e�N�X�`���̒ǉ�����
	AddTexture(in_filePath, in_renderer);


	return true;
}



//----------------------------------------------------------------------------------------+
// �^���W�F���g�x�N�g���v�Z
// �O�p�`�ʂ�pos1�`pos3, uv0�`uv2 ���g����Tangent�x�N�g����destTangent�ɕԂ�
// 
// inout destTangent Tangent���i�[����ϐ�
// in : pos0, pos1, pos2 �O�p�`�ʂ̒��_
// in : uv0, uv1, uv2 �O�p�`�ʂ̒��_�ɑΉ�����UV���W
//----------------------------------------------------------------------------------------+
void MeshObj::calcTangent(Vector3& destTangent, const Vector3& pos1, const Vector3& pos2, const Vector3& pos3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3)
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
void MeshObj::getPosVec(Vector3& destPos, const std::vector<float> fv, int index)
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
void MeshObj::getUVVec(Vector2& destUV, const std::vector<float> fv, int index)
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
void MeshObj::setTangent(std::vector<float>& destfv, int index, const Vector3& tangent)
{
	if (destfv.size() > index * 11 + 8 + 2)
	{
		destfv[index * 11 + 8 + 0] = tangent.x;
		destfv[index * 11 + 8 + 1] = tangent.y;
		destfv[index * 11 + 8 + 2] = tangent.z;
	}

}
