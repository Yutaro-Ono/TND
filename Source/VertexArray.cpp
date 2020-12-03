//------------------------------------------------------------------------------+
// ���_�N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//------------------------------------------------------------------------------+
#include "VertexArray.h"
#include <glad/glad.h>

// �W���R���X�g���N�^
VertexArray::VertexArray()
{
}

//-------------------------------------------------------------------------------------------------------------------+
// ���_���C�A�E�g
// PosNormTex = 8 * sizeof(float) = 32 bytes
// | �ʒu      | �@��      | Texture|
// | x | y | z | x | y | z | u | v | 
//
// posNormSkinTex
// PosNormSkinTex = (8 * sizeof(float)) + (8 * sizeof(char)) = 40 bytes
// | �ʒu      | �@��       | Bones   | weight  |Texture|
// | x | y | z | x | y | z | char[4] | char[4] | u | v |
//                                    ��weight�̊m�ۂ�char�����A���x���K�v�Ȃ��̂�8bit�Œ菬���Ƃ��Ďg�p����
//-------------------------------------------------------------------------------------------------------------------+
VertexArray::VertexArray(const void * in_verts, unsigned int in_vertsNum, Layout in_layout, const unsigned int * in_inDices, unsigned int in_numInDices)
	:m_vertsNum(in_vertsNum)
	,m_numInDices(in_numInDices)
{
	// ���_�z��̍쐬
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// ���_���C�A�E�g�� �X�P���^�����f���Ȃ�@�{�[��ID�A�e���x�����T�C�Y���₷
	unsigned vertexSize = 8 * sizeof(float);
	if (in_layout == POS_NORMAL_SKIN_TEX)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
	}
	if (in_layout == POS_NORMAL_TEX_TAN)
	{
		vertexSize = 11 * sizeof(float);
	}
	if (in_layout == POS_NORMAL_SKIN_TEX_TAN)
	{
		vertexSize = 11 * sizeof(float) + 8 * sizeof(char);
	}
	// ���_�o�b�t�@�̍쐬
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, in_vertsNum * vertexSize, in_verts, GL_STATIC_DRAW);

	// �C���f�b�N�X�o�b�t�@�̍쐬
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, in_numInDices * sizeof(unsigned int), in_inDices, GL_STATIC_DRAW);

	// ���_����
	if (in_layout == POS_NORMAL_TEX)
	{
		// float 3���@���@�ʒu x,y,z�@�ʒu�������Z�b�g
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		// ����float 3�� �� �@�� nx, ny, nz�@�@���������Z�b�g
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));
		// ����float 2�� u, v  �e�N�X�`�����W�������Z�b�g
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
	}
	else if (in_layout == POS_NORMAL_SKIN_TEX)
	{
		// float 3���@���@�ʒu x,y,z�@�ʒu�������Z�b�g
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		// ����float 3�� �� �@�� nx, ny, nz�@�@���������Z�b�g
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));
		//�@�e���󂯂�{�[���C���f�b�N�X�ԍ�  (int�^���L�[�v)
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
		// �{�[���E�F�C�g��� (float �ɕϊ�)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4));
		// ����float 2�� u, v  �e�N�X�`�����W�������Z�b�g
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8));
	}
	else if (in_layout == POS_NORMAL_TEX_TAN)
	{
		// 0 : ���_���W
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);
		// 1 : �@�����W
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(3 * sizeof(float)));
		// 2 : �e�N�X�`�����W
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(6 * sizeof(float)));
		// 3 : �ڃx�N�g�� (�^���W�F���g)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(8 * sizeof(float)));
	}
	else if (in_layout == POS_NORMAL_SKIN_TEX_TAN)
	{
		//// float 3���@���@�ʒu x,y,z�@�ʒu�������Z�b�g
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		//// ����float 3�� �� �@�� nx, ny, nz�@�@���������Z�b�g
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
		//	reinterpret_cast<void*>(sizeof(float) * 3));
		//// ����float 2�� u, v  �e�N�X�`�����W�������Z�b�g
		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
		//	reinterpret_cast<void*>(sizeof(float) * 6));
		//// 3 : �ڃx�N�g�� (�^���W�F���g)
		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertexSize, 
		//	reinterpret_cast<void*>(sizeof(float) * 8));
		////�@�e���󂯂�{�[���C���f�b�N�X�ԍ�  (int�^���L�[�v)
		//glEnableVertexAttribArray(4);
		//glVertexAttribIPointer(4, 4, GL_UNSIGNED_BYTE, vertexSize,
		//	reinterpret_cast<void*>(sizeof(float) * 11));
		//// �{�[���E�F�C�g��� (float �ɕϊ�)
		//glEnableVertexAttribArray(5);
		//glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
		//	reinterpret_cast<void*>(sizeof(float) * 11 + sizeof(char) * 4));

		// float 3���@���@�ʒu x,y,z�@�ʒu�������Z�b�g
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		// ����float 3�� �� �@�� nx, ny, nz�@�@���������Z�b�g
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));
		//�@�e���󂯂�{�[���C���f�b�N�X�ԍ�  (int�^���L�[�v)
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
		// �{�[���E�F�C�g��� (float �ɕϊ�)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4));
		// ����float 2�� u, v  �e�N�X�`�����W�������Z�b�g
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8));
		// 3 : �ڃx�N�g�� (�^���W�F���g)
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 8 + sizeof(char) * 8));
	}
}

VertexArray::VertexArray(const float * in_verts, unsigned int in_vertsNum, const unsigned int * in_inDices, unsigned int in_numInDices)
	:m_vertsNum(in_vertsNum)
	,m_numInDices(in_numInDices)
{

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, in_vertsNum * 8 * sizeof(float), in_verts, GL_STATIC_DRAW);

	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, in_numInDices * sizeof(unsigned int), in_inDices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		reinterpret_cast<void*>(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		reinterpret_cast<void*>(sizeof(float) * 6));
}

// �L���[�u�}�b�v�p���_�z��
void VertexArray::CreateCubeVerts()
{
	float cubeVertices[] =
	{

		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int indices[] =
	{
		 0,  1,  2,  0,  2,  3,    // �O��
		 4,  5,  6,  4,  6,  7,    // �w��
		 8,  9, 10,  8, 10, 11,    // ���
		12, 13, 14, 12, 14, 15,    // ���
		16, 17, 18, 16, 18, 19,    // �E����
		20, 21, 22, 20, 22, 23     // ������
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 36 * 8 * sizeof(float), cubeVertices, GL_STATIC_DRAW);
	// �A�g���r���[�g�ւ̃Z�b�g(���_���W�̂�)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

// �X�N���[���`��p���_�z��
void VertexArray::CreateScreenVerts()
{
	// �X�N���[���S�̂̎l�p�`�p���_�z��
	float quadVertices[] = 
	{
		// �|�W�V����   // �e�N�X�`�����W
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		 -1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// �X�N���[���S�̂�`���l�p�`�p VAO
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

// �f�X�g���N�^
VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_indexBuffer);
	glDeleteVertexArrays(1, &m_VAO);
}

void VertexArray::SetActive()
{
	glBindVertexArray(m_VAO);
}
