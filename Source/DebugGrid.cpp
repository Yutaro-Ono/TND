#include "DebugGrid.h"

DebugGrid::DebugGrid(float in_size, int in_split, Vector3& in_color)
	:m_isReady(false)
	,m_color(in_color)
	,m_gridShader(nullptr)
{
	// ライン数の定義
	int lineNum = in_split + 1;

	float halfSize = in_size * 0.5f;
	float splitLen = in_size / in_split;

	// グリッド頂点数の計算
	// 頂点数 = 分割時のライン本数 * 2 (縦・横) * 2 (1本につき2頂点)
	m_numVerts = lineNum * 2 * 2;

	// 頂点バッファへ渡すラインデータ作成
	m_vertexData = new GLfloat[m_numVerts * 3];

	// 縦方向
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
	// 横方向
	for (int i = 0; i < lineNum; i++)
	{
		m_vertexData[ofs + (i * 6) + 0] = -halfSize;
		m_vertexData[ofs + (i * 6) + 1] = splitLen * i - halfSize;
		m_vertexData[ofs + (i * 6) + 2] = 0.0f;
		m_vertexData[ofs + (i * 6) + 3] = +halfSize;
		m_vertexData[ofs + (i * 6) + 4] = splitLen * i - halfSize;
		m_vertexData[ofs + (i * 6) + 5] = 0.0f;
	}

	// 初期化
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
/// 描画関数
/// </summary>
void DebugGrid::Draw()
{
	// 頂点配列をバインド
	glBindVertexArray(m_vao);

	// シェーダの有効化
	m_gridShader->SetActive();
	// Uniformセット
	m_gridShader->SetMatrixUniform("u_viewProj", RENDERER->GetViewMatrix() * RENDERER->GetProjectionMatrix());
	m_gridShader->SetVectorUniform("u_color", m_color);

	// 頂点配列から描画
	glDrawArrays(GL_LINES, 0, m_numVerts);
}

/// <summary>
/// 初期化関数。計算した頂点データから、頂点配列を作成しGLに登録する
/// また、グリッド用のシェーダも作成
/// </summary>
/// <returns></returns>
bool DebugGrid::Initialize()
{
	// 頂点配列の作成
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// 頂点バッファの作成
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_numVerts * sizeof(GLfloat) * 3, m_vertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);

	// グリッド用シェーダの作成
	m_gridShader = new Shader;
	if (!m_gridShader->Load("Shaders/Line.vert", "Shaders/Line.frag"))
	{
		return false;
	}

	return true;
}
