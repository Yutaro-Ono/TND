//------------------------------------------------+
// �f�o�b�O�p�O���b�h���C��
//------------------------------------------------+
#include "GameMain.h"
#include "Renderer.h"
#include "Shader.h"

class DebugGrid
{

public:

	// �R���X�g���N�^�E�f�X�g���N�^
	DebugGrid(float in_size, int in_split, Vector3& in_color);
	~DebugGrid();

	void Draw();                     // �`��֐�


private:

	bool Initialize();               // �������֐�


	bool m_isReady;                  // ������������������
	unsigned int m_numVerts;         // ���_��
	unsigned int m_vbo;              // ���_�o�b�t�@
	unsigned int m_vao;              // ���_�z��

	GLfloat* m_vertexData;           // ���_�f�[�^

	Vector3 m_color;                 // �O���b�h�J���[

	class Shader* m_gridShader;      // �O���b�h���C���p�V�F�[�_�[

};