#include "DebugBox.h"
#include "Shader.h"

// コンストラクタ
DebugBox::DebugBox()
	:m_obbVertexData(nullptr)
	,m_obbIndexData(nullptr)
	,m_shader(nullptr)
	,m_isReady(false)
{
	CreateVertexData();
}

// デストラクタ
DebugBox::~DebugBox()
{
	delete[] m_obbIndexData;
	delete[] m_obbVertexData;

	if (!m_shader)
	{
		delete m_shader;
	}
	if (!m_obbVertexData)
	{
		delete m_obbVertexData;
	}
	if (!m_obbIndexData)
	{
		delete m_obbIndexData;
	}
}

void DebugBox::SetDebugBox(OBB & in_box)
{
	m_obbs.push_back(in_box);
}

void DebugBox::Draw()
{
	Matrix4 view, proj;
	view = GAME_INSTANCE.GetRenderer()->GetViewMatrix();
	proj = GAME_INSTANCE.GetRenderer()->GetProjectionMatrix();

	m_shader->SetActive();
	m_shader->SetMatrixUniform("uViewProj", view * proj);
	m_shader->SetVectorUniform("uColor", Vector3(1.0f, 1.0f, 0.0f));

	for (auto box : m_obbs)
	{
		m_shader->SetMatrixUniform("uWorldTransform", box.GetMatrix());
		glBindVertexArray(m_vertexArray);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, nullptr);
	}
}

bool DebugBox::CreateVertexData()
{
	// OBBの基本ボックスデータ
	// VertexBuufer
	m_obbVertexData = new GLfloat[8 * 3];
	m_obbVertexData[0] = -0.5f;
	m_obbVertexData[1] = -0.5f;
	m_obbVertexData[2] = -0.5f;

	m_obbVertexData[3] = +0.5f;
	m_obbVertexData[4] = -0.5f;
	m_obbVertexData[5] = -0.5f;

	m_obbVertexData[6] = +0.5f;
	m_obbVertexData[7] = -0.5f;
	m_obbVertexData[8] = +0.5f;

	m_obbVertexData[9] = -0.5f;
	m_obbVertexData[10] = -0.5f;
	m_obbVertexData[11] = +0.5f;

	for (int i = 0; i < 12; i++)
	{
		m_obbVertexData[i + 12] = m_obbVertexData[i];
	}
	for (int i = 0; i < 4; i++)
	{
		m_obbVertexData[i * 3 + 12 + 1] = +0.5f;
	}

	// IndexBuffer
	m_obbIndexData = new GLuint[12 * 2];
	m_obbIndexData[0] = 0;
	m_obbIndexData[1] = 1;
	m_obbIndexData[2] = 1;
	m_obbIndexData[3] = 2;
	m_obbIndexData[4] = 2;
	m_obbIndexData[5] = 3;
	m_obbIndexData[6] = 3;
	m_obbIndexData[7] = 0;

	m_obbIndexData[8] = 4;
	m_obbIndexData[9] = 5;
	m_obbIndexData[10] = 5;
	m_obbIndexData[11] = 6;
	m_obbIndexData[12] = 6;
	m_obbIndexData[13] = 7;
	m_obbIndexData[14] = 7;
	m_obbIndexData[15] = 4;

	m_obbIndexData[16] = 0;
	m_obbIndexData[17] = 4;
	m_obbIndexData[18] = 1;
	m_obbIndexData[19] = 5;
	m_obbIndexData[20] = 2;
	m_obbIndexData[21] = 6;
	m_obbIndexData[22] = 3;
	m_obbIndexData[23] = 7;

	//頂点配列の作成
	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);
	{
		// 頂点バッファの作成
		glGenBuffers(1, &m_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

		//頂点バッファに頂点データ転送
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 3, m_obbVertexData, GL_STATIC_DRAW);
		// この頂点バッファオブジェクトを0番の attributeから取り出せるようにする
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		// GLErrorHandle("DebugBox::CreateVertexBuffer() VertexBuffer");

		// インデックスバッファの作成
		glGenBuffers(1, &m_indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 12 * 2, m_obbIndexData, GL_STATIC_DRAW);
		// GLErrorHandle("DebugBox::CreateVertexBuffer() IndexBuffer");
	}
	glBindVertexArray(0);

	m_shader = new Shader;
	if (!m_shader->Load("Data/Shaders/Line.vert", "Data/Shaders/Line.frag"))
	{
		return false;
	}
	// GLErrorHandle("DebugGrid::Init() ShaderLoad");

	m_isReady = true;

	return true;
}
