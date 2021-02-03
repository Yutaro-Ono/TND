//-----------------------------------------------------------------------+
// シェーダークラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Shader.h"
#include "Texture.h"
#include <SDL.h>
#include <fstream>
#include <sstream>
#include <iostream>

// コンストラクタ
Shader::Shader()
	:m_shaderProgram(0)
	,m_vertexShader(0)
	,m_fragShader(0)
{
	
}

// デストラクタ
Shader::~Shader()
{
	Delete();
}

// シェーダーの読み込み
bool Shader::Load(const std::string & in_vertName, const std::string & in_fragName)
{
	// 頂点シェーダー、ピクセル(フラグメント)シェーダーをコンパイル
	if (!CompileShader(in_vertName, GL_VERTEX_SHADER, m_vertexShader) ||
		!CompileShader(in_fragName, GL_FRAGMENT_SHADER, m_fragShader))
	{
		std::cout << "ERROR::SHADER::LOAD::" << "VERTEX : " << in_vertName <<std::endl << "FRAGMENT : " << in_fragName << std::endl;
		return false;
	}

	// 頂点シェーダー・フラグメントシェーダーをリンクさせる
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, m_vertexShader);
	glAttachShader(m_shaderProgram, m_fragShader);
	glLinkProgram(m_shaderProgram);

	// uniformバッファブロックへのリンク
// 行列
	m_uniformBlockIndex = glGetUniformBlockIndex(m_shaderProgram, "Matrices");
	glUniformBlockBinding(m_shaderProgram, m_uniformBlockIndex, 0);
	// カメラ
	m_uniformBlockIndex = glGetUniformBlockIndex(m_shaderProgram, "CameraVariable");
	glUniformBlockBinding(m_shaderProgram, m_uniformBlockIndex, 1);


	// リンクが失敗
	if (!IsValidProgram())
	{
		std::cout << "ERROR::SHADER::LINK::" << "VERTEX : " << in_vertName << std::endl << "FRAGMENT : " << in_fragName << std::endl;
		return false;
	}


	return true;
}

void Shader::LinkUniformBuffer()
{

}

// シェーダー破棄
void Shader::Delete()
{
	// シェーダー、プログラムの解放
	glDeleteProgram(m_shaderProgram);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragShader);
}

// シェーダーをアクティブにする
void Shader::SetActive()
{
	// シェーダープログラムをアクティブにする
	glUseProgram(m_shaderProgram);
}

// Bool型Uniformセッター
void Shader::SetBool(const std::string& in_variableName, bool in_value) const
{
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_variableName.c_str());
	glUniform1i(loc, (int)in_value);
}

// Int型Uniformセッター
void Shader::SetInt(const std::string& in_variableName, int in_value) const
{
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_variableName.c_str());
	glUniform1i(loc, in_value);
}

// Float型Uniformセッター
void Shader::SetFloat(const std::string& in_variableName, float in_value) const
{
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_variableName.c_str());
	glUniform1f(loc, in_value);
}

// 行列のシェーダー変数 nameに値をセットする
void Shader::SetMatrixUniform(const char * in_name, const Matrix4 & in_matrix)
{
	// シェーダー変数(uniform)を名前で検索する
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_name);
	// 行列データをシェーダー変数に送る
	glUniformMatrix4fv(loc, 1, GL_TRUE, in_matrix.GetAsFloatPtr());
}

// シェーダー行列配列名に行列をセットする
void Shader::SetMatrixUniforms(const char * in_name, Matrix4 * in_matrices, unsigned in_count)
{
	// シェーダー変数(uniform)を名前で検索する
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_name);
	// 行列データをシェーダー変数に送る
	glUniformMatrix4fv(loc, in_count, GL_TRUE, in_matrices->GetAsFloatPtr());
}


void Shader::SetVectorUniform(const char * in_name, const Vector3 & in_vector)
{
	// シェーダー変数(uniform)を名前で検索する
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_name);
	// ベクトル値をシェーダー変数に送る
	glUniform3fv(loc, 1, in_vector.GetAsFloatPtr());
}

void Shader::SetVector2Uniform(const char* in_name, Vector2& in_vector)
{
	// シェーダー変数(uniform)を名前で検索する
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_name);
	// ベクトル値をシェーダー変数に送る
	glUniform2f(loc, in_vector.x, in_vector.y);
}


void Shader::SetFloatUniform(const char * in_name, float in_value)
{
	// シェーダー変数(uniform)を名前で検索する
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_name);
	// 行列データをシェーダー変数に送る
	glUniform1f(loc, in_value);
}

bool Shader::CompileShader(const std::string & in_fileName, GLenum in_shaderType, GLuint & in_outShader)
{
	// ファイルを開く
	std::ifstream shaderFile(in_fileName);

	if (shaderFile.is_open())
	{
		// テキストを読み込んで、文字列に変換する
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		// シェーダーをshaderTypeに従って作成する
		in_outShader = glCreateShader(in_shaderType);
		// ソース文字列のコンパイルを試みる
		glShaderSource(in_outShader, 1, &(contentsChar), nullptr);
		glCompileShader(in_outShader);

		if (!IsCompiled(in_outShader))
		{
			printf("シェーダーコンパイルに失敗 : %s", in_fileName.c_str());
			return false;
		}
	}
	else
	{
		printf("シェーダーファイルが見つかりませんでした : %s", in_fileName.c_str());
		return false;
	}


	return true;
}

bool Shader::IsCompiled(GLuint in_shader)
{
	GLint status;

	// コンパイルの状態を問い合わせる
	glGetShaderiv(in_shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(in_shader, 511, nullptr, buffer);
		printf("GLSL compileに失敗 : \n%s", buffer);

		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{
	GLint status;

	// リンク状態を問い合わせる
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);

	// リンク状態の問い合わせ
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);

		glGetProgramInfoLog(m_shaderProgram, 511, nullptr, buffer);
		printf("GLSL Link 状態 : \n%s", buffer);

		return false;
	}

	return true;
}
