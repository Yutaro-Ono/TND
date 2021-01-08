#include "DebugGrid.h"

DebugGrid::DebugGrid(float in_size, int in_split, Vector3& in_color)
	:m_isReady(false)
	,m_color(in_color)
	,m_gridShader(nullptr)
{
	// ���C�����̒�`
	int lineNum = in_split + 1;

	float halfSize = in_size * 0.5f;
	float splitLen = in_size / in_split;

	// �O���b�h���_���̌v�Z
	// ���_�� = �������̃��C���{�� * 2 (�c�E��) * 2 (1�{�ɂ�2���_)
	m_numVerts = lineNum * 2 * 2;

	// ���_�o�b�t�@�֓n�����C���f�[�^�쐬
	m_vertexData = new GLfloat[m_numVerts * 3];

	// �c����
	for (int i = 0; i < lineNum; i++)
	{
		m_vertexData[(i * 6) + 0] = splitLen * i - halfSize;
		m_vertexData[(i * 6) + 1] = -halfSize;;
		m_vertexData[(i * 6) + 2] = 0.0f;
		m_vertexData[(i * 6) + 3] = splitLen * i - halfSize;
		m_vertexData[(i * 6) + 4] = +halfSize;
		m_vertexData[(i * 6) + 5] = 0.0f;
	}
	int ofs = lineNum * 6;
	// ������
	for (int i = 0; i < lineNum; i++)
	{
		m_vertexData[ofs + (i * 6) + 0] = -halfSize;
		m_vertexData[ofs + (i * 6) + 1] = splitLen * i - halfSize;
		m_vertexData[ofs + (i * 6) + 2] = 0.0f;
		m_vertexData[ofs + (i * 6) + 3] = +halfSize;
		m_vertexData[ofs + (i * 6) + 4] = splitLen * i - halfSize;
		m_vertexData[ofs + (i * 6) + 5] = 0.0f;
	}

	// ������
	if (Initialize())
	{
		m_isReady = true;
	}
}

DebugGrid::~DebugGrid()
{
	delete[] m_vertexData;
}

/// <summary>
/// �`��֐�
/// </summary>
void DebugGrid::Draw()
{
	// ���_�z����o�C���h
	glBindVertexArray(m_vao);

	// �V�F�[�_�̗L����
	m_gridShader->SetActive();
	// Uniform�Z�b�g
	m_gridShader->SetMatrixUniform("u_viewProj", RENDERER->GetViewMatrix() * RENDERER->GetProjectionMatrix());
	m_gridShader->SetVectorUniform("u_color", m_color);

	// ���_�z�񂩂�`��
	glDrawArrays(GL_LINES, 0, m_numVerts);
}

/// <summary>
/// �������֐��B�v�Z�������_�f�[�^����A���_�z����쐬��GL�ɓo�^����
/// �܂��A�O���b�h�p�̃V�F�[�_���쐬
/// </summary>
/// <returns></returns>
bool DebugGrid::Initialize()
{
	// ���_�z��̍쐬
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// ���_�o�b�t�@�̍쐬
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_numVerts * sizeof(GLfloat) * 3, m_vertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);

	// �O���b�h�p�V�F�[�_�̍쐬
	m_gridShader = new Shader;
	if (!m_gridShader->Load("Shaders/Line.vert", "Shaders/Line.frag"))
	{
		return false;
	}

	return true;
}
