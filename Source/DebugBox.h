#pragma once

#include "GameMain.h"
#include "Renderer.h"
#include "Collision.h"
#include <vector>

class DebugBox
{
public:

	DebugBox();                       // コンストラクタ
	~DebugBox();                      // デストラクタ

	void SetDebugBox(OBB& in_box);
	void Draw();

private:

	std::vector<OBB> m_obbs;
	GLfloat* m_obbVertexData;
	GLuint* m_obbIndexData;


	GLuint m_vertexArray;
	GLuint m_vertexBuffer;
	GLuint m_indexBuffer;
	GLuint m_indexArray;

	class Shader* m_shader;
	bool CreateVertexData();

	bool m_isReady;

};