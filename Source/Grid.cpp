//-----------------------------------------------------------------------+
// �O���b�h�N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Grid.h"
#include "Shader.h"


Grid::Grid(float in_size, int in_split, Vector3 & in_color)
	:m_isReady(false)
	,m_color(in_color)
{
	int lineNum = in_split + 1;
	float halfSize = in_size * 0.5f;
	float splitLen = in_size / in_split;

	// ���_�� = �������̃��C���{�� * 2(�c�Ɖ�) * 2(1�{�ɂ�2���_)
	m_vertsNum = lineNum * 2 * 2;

	// ���_�o�b�t�@�֗������ރ��C���f�[�^�쐬
	m_vertexData = new GLfloat[m_vertsNum * 3];

	// �c����
	int i;
	for (i = 0; i < lineNum; i++)
	{
		m_vertexData[(i * 6) + 0] = splitLen * i - halfSize;
		m_vertexData[(i * 6) + 1] = -halfSize;
		m_vertexData[(i * 6) + 2] = 0.0f;
		m_vertexData[(i * 6) + 3] = splitLen * i - halfSize;
		m_vertexData[(i * 6) + 4] = +halfSize;
		m_vertexData[(i * 6) + 5] = 0.0f;
	}
	int ofs = lineNum * 6;

	// ������
	for (i = 0; i < lineNum; i++)
	{
		m_vertexData[ofs + (i * 6) + 0] = -halfSize;
		m_vertexData[ofs + (i * 6) + 1] = splitLen * i - halfSize;
		m_vertexData[ofs + (i * 6) + 2] = 0.0f;
		m_vertexData[ofs + (i * 6) + 3] = +halfSize;
		m_vertexData[ofs + (i * 6) + 4] = splitLen * i - halfSize;
		m_vertexData[ofs + (i * 6) + 5] = 0.0f;
	}

	if (Init())
	{
		m_isReady = true;
	}

}

Grid::~Grid()
{
	delete[] m_vertexData;
}

void Grid::Draw()
{
	glBindVertexArray(m_vertexArray);

	m_shader->SetActive();
	Matrix4 mWorld, mView, mProj;
	mView = GAME_INSTANCE.GetRenderer()->GetViewMatrix();
	mProj = GAME_INSTANCE.GetRenderer()->GetProjectionMatrix();

	m_shader->SetMatrixUniform("uViewProj", mView * mProj);
	m_shader->SetVectorUniform("uColor", m_color);

	glDrawArrays(GL_LINES, 0, m_vertsNum);
}

bool Grid::Init()
{
	// ���_�z��̍쐬
	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);

	// ���_�o�b�t�@�̍쐬
	glGenBuffers(1, &m_vertexBuff);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuff);
	glBufferData(GL_ARRAY_BUFFER, m_vertsNum * sizeof(GLfloat) * 3, m_vertexData, GL_STATIC_DRAW);
	// GLErrorHandle("DebugGrid::Init()");

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);

	m_shader = new Shader;
	if (!m_shader->Load("Data/Shaders/DirectLine.vert", "Data/Shaders/Line.frag"))
	{
		return false;
	}
	// GLErrorHandle("DebugGrid::Init() ShaderLoad");

	return true;
}
