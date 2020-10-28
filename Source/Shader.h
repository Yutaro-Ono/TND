//-----------------------------------------------------------------------+
// シェーダークラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader
{
public:

	Shader();
	~Shader();

	// 頂点シェーダー、フラグメントシェーダーファイル読み込み
	bool Load(const std::string& in_vertName, const std::string& in_fragName);

	void Delete();          // シェーダー破棄

	void SetActive();       // このシェーダーをアクティブにする

	void SetBool(const std::string& in_variableName, bool in_value) const;          // Bool型Uniformセッター
	void SetInt(const std::string& in_variableName, int in_value) const;            // Int型Uniformセッター
	void SetFloat(const std::string& in_variableName, float in_value) const;        // Float型Uniformセッター

	void SetMatrixUniform(const char* in_name, const Matrix4& in_matrix);     // 行列のシェーダー変数 nameに値をセットする

	void SetMatrixUniforms(const char* in_name, Matrix4* in_matrices, unsigned in_count);     // シェーダー行列配列名に行列をセットする
	void SetVectorUniform(const char* in_name, const Vector3& in_vector);                     // ベクトル値をシェーダー変数nameにセットする
	void SetVector2Uniform(const char* in_name, Vector2& in_vector);                           // 二次元ベクトルセット
	void SetFloatUniform(const char* in_name, float in_value);                                // 変数値をシェーダ変数nameにセットする

private:
	bool CompileShader(const std::string& in_fileName,                                        // 読み込まれたシェーダーのコンパイル
		GLenum in_shaderType,
		GLuint& in_outShader);

	bool IsCompiled(GLuint in_shader);                                                        // コンパイル成功したか？
	bool IsValidProgram();                                                                    // vertex,fragmentともにリンク成功してるか？

	// Store the shader object IDs
	GLuint m_vertexShader;                                                                    // 頂点シェーダーID
	GLuint m_fragShader;                                                                      // フラグメントシェーダーID
	GLuint m_shaderProgram;                                                                   // シェーダープログラム（頂点＋フラグメント）ID

};