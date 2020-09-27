//-----------------------------------------------------------------------+
// グリッドクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

#include "GameMain.h"
#include "Renderer.h"


class Grid
{


public:

	Grid(float in_size, int in_split, Vector3& in_color);
	~Grid();

	void Draw();



private:

	bool Init();

	bool m_isReady;

	unsigned int m_vertsNum;
	unsigned int m_vertexBuff;
	unsigned int m_vertexArray;

	GLfloat* m_vertexData;

	GLuint m_vertShader;
	GLuint m_fragShader;
	GLuint m_shaderProgram;
	Vector3 m_color;

	class Shader* m_shader;


};