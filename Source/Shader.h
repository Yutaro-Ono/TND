//-----------------------------------------------------------------------+
// �V�F�[�_�[�N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <string>
#include "Math.h"
#include <glad/glad.h>

class Shader
{
public:

	Shader();
	~Shader();

	// ���_�V�F�[�_�[�A�t���O�����g�V�F�[�_�[�t�@�C���ǂݍ���
	bool Load(const std::string& in_vertName, const std::string& in_fragName);

	void LinkUniformBuffer();

	void Delete();          // �V�F�[�_�[�j��

	void SetActive();       // ���̃V�F�[�_�[���A�N�e�B�u�ɂ���

	void SetBool(const std::string& in_variableName, bool in_value) const;                    // Bool�^Uniform�Z�b�^�[
	void SetInt(const std::string& in_variableName, int in_value) const;                      // Int�^Uniform�Z�b�^�[
	void SetFloat(const std::string& in_variableName, float in_value) const;                  // Float�^Uniform�Z�b�^�[

	void SetMatrixUniform(const char* in_name, const Matrix4& in_matrix);                     // �s��̃V�F�[�_�[�ϐ� name�ɒl���Z�b�g����

	void SetMatrixUniforms(const char* in_name, Matrix4* in_matrices, unsigned in_count);     // �V�F�[�_�[�s��z�񖼂ɍs����Z�b�g����
	void SetVectorUniform(const char* in_name, const Vector3& in_vector);                     // �x�N�g���l���V�F�[�_�[�ϐ�name�ɃZ�b�g����
	void SetVector2Uniform(const char* in_name, Vector2& in_vector);                          // �񎟌��x�N�g���Z�b�g
	void SetFloatUniform(const char* in_name, float in_value);                                // �ϐ��l���V�F�[�_�ϐ�name�ɃZ�b�g����

private:
	bool CompileShader(const std::string& in_fileName,                                        // �ǂݍ��܂ꂽ�V�F�[�_�[�̃R���p�C��
		GLenum in_shaderType,
		GLuint& in_outShader);

	bool IsCompiled(GLuint in_shader);                                                        // �R���p�C�������������H
	bool IsValidProgram();                                                                    // vertex,fragment�Ƃ��Ƀ����N�������Ă邩�H

	
	unsigned int m_vertexShader;                                                                    // ���_�V�F�[�_�[ID
	unsigned int m_fragShader;                                                                      // �t���O�����g�V�F�[�_�[ID
	unsigned int m_shaderProgram;                                                                   // �V�F�[�_�[�v���O�����i���_�{�t���O�����g�jID
	unsigned int m_uniformBlockIndex;                                                               // uniform�o�b�t�@�p�u���b�N�C���f�b�N�X
};