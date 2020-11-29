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

				// ���_���W�𒸓_�z��ɃR�s�[
				vertexVec[idx.vertex_index * attribStride + 0] = attrib.vertices[3 * idx.vertex_index + 0];
				vertexVec[idx.vertex_index * attribStride + 1] = attrib.vertices[3 * idx.vertex_index + 2];
				vertexVec[idx.vertex_index * attribStride + 2] = attrib.vertices[3 * idx.vertex_index + 1];

				// �@���f�[�^�𒸓_�z��ɃR�s�[
				vertexVec[idx.vertex_index * attribStride + 3] = attrib.normals[3 * idx.normal_index + 0];
				vertexVec[idx.vertex_index * attribStride + 4] = attrib.normals[3 * idx.normal_index + 2];
				vertexVec[idx.vertex_index * attribStride + 5] = attrib.normals[3 * idx.normal_index + 1];

				// uv�f�[�^�𒸓_�z��ɃR�s�[
				vertexVec[idx.vertex_index * attribStride + 6] = attrib.texcoords[2 * idx.texcoord_index + 0];
				vertexVec[idx.vertex_index * attribStride + 7] = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1];


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
			calcTangent(tangent, destPos[0], destPos[1], destPos[2], uvPos[0], uvPos[1], uvPos[2]);
			// �i�[����3�̒��_���W��UV����Z�o���ꂽ�^���W�F���g�𒸓_�z��Ɋi�[
			for (int i = 0; i < 3; i++)
			{
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
