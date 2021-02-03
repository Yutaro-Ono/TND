//-----------------------------------------------------------------------+
// �V�F�[�_�[�N���X
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

// �R���X�g���N�^
Shader::Shader()
	:m_shaderProgram(0)
	,m_vertexShader(0)
	,m_fragShader(0)
{
	
}

// �f�X�g���N�^
Shader::~Shader()
{
	Delete();
}

// �V�F�[�_�[�̓ǂݍ���
bool Shader::Load(const std::string & in_vertName, const std::string & in_fragName)
{
	// ���_�V�F�[�_�[�A�s�N�Z��(�t���O�����g)�V�F�[�_�[���R���p�C��
	if (!CompileShader(in_vertName, GL_VERTEX_SHADER, m_vertexShader) ||
		!CompileShader(in_fragName, GL_FRAGMENT_SHADER, m_fragShader))
	{
		std::cout << "ERROR::SHADER::LOAD::" << "VERTEX : " << in_vertName <<std::endl << "FRAGMENT : " << in_fragName << std::endl;
		return false;
	}

	// ���_�V�F�[�_�[�E�t���O�����g�V�F�[�_�[�������N������
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, m_vertexShader);
	glAttachShader(m_shaderProgram, m_fragShader);
	glLinkProgram(m_shaderProgram);

	// uniform�o�b�t�@�u���b�N�ւ̃����N
// �s��
	m_uniformBlockIndex = glGetUniformBlockIndex(m_shaderProgram, "Matrices");
	glUniformBlockBinding(m_shaderProgram, m_uniformBlockIndex, 0);
	// �J����
	m_uniformBlockIndex = glGetUniformBlockIndex(m_shaderProgram, "CameraVariable");
	glUniformBlockBinding(m_shaderProgram, m_uniformBlockIndex, 1);


	// �����N�����s
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

// �V�F�[�_�[�j��
void Shader::Delete()
{
	// �V�F�[�_�[�A�v���O�����̉��
	glDeleteProgram(m_shaderProgram);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragShader);
}

// �V�F�[�_�[���A�N�e�B�u�ɂ���
void Shader::SetActive()
{
	// �V�F�[�_�[�v���O�������A�N�e�B�u�ɂ���
	glUseProgram(m_shaderProgram);
}

// Bool�^Uniform�Z�b�^�[
void Shader::SetBool(const std::string& in_variableName, bool in_value) const
{
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_variableName.c_str());
	glUniform1i(loc, (int)in_value);
}

// Int�^Uniform�Z�b�^�[
void Shader::SetInt(const std::string& in_variableName, int in_value) const
{
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_variableName.c_str());
	glUniform1i(loc, in_value);
}

// Float�^Uniform�Z�b�^�[
void Shader::SetFloat(const std::string& in_variableName, float in_value) const
{
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_variableName.c_str());
	glUniform1f(loc, in_value);
}

// �s��̃V�F�[�_�[�ϐ� name�ɒl���Z�b�g����
void Shader::SetMatrixUniform(const char * in_name, const Matrix4 & in_matrix)
{
	// �V�F�[�_�[�ϐ�(uniform)�𖼑O�Ō�������
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_name);
	// �s��f�[�^���V�F�[�_�[�ϐ��ɑ���
	glUniformMatrix4fv(loc, 1, GL_TRUE, in_matrix.GetAsFloatPtr());
}

// �V�F�[�_�[�s��z�񖼂ɍs����Z�b�g����
void Shader::SetMatrixUniforms(const char * in_name, Matrix4 * in_matrices, unsigned in_count)
{
	// �V�F�[�_�[�ϐ�(uniform)�𖼑O�Ō�������
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_name);
	// �s��f�[�^���V�F�[�_�[�ϐ��ɑ���
	glUniformMatrix4fv(loc, in_count, GL_TRUE, in_matrices->GetAsFloatPtr());
}


void Shader::SetVectorUniform(const char * in_name, const Vector3 & in_vector)
{
	// �V�F�[�_�[�ϐ�(uniform)�𖼑O�Ō�������
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_name);
	// �x�N�g���l���V�F�[�_�[�ϐ��ɑ���
	glUniform3fv(loc, 1, in_vector.GetAsFloatPtr());
}

void Shader::SetVector2Uniform(const char* in_name, Vector2& in_vector)
{
	// �V�F�[�_�[�ϐ�(uniform)�𖼑O�Ō�������
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_name);
	// �x�N�g���l���V�F�[�_�[�ϐ��ɑ���
	glUniform2f(loc, in_vector.x, in_vector.y);
}


void Shader::SetFloatUniform(const char * in_name, float in_value)
{
	// �V�F�[�_�[�ϐ�(uniform)�𖼑O�Ō�������
	GLuint loc = glGetUniformLocation(m_shaderProgram, in_name);
	// �s��f�[�^���V�F�[�_�[�ϐ��ɑ���
	glUniform1f(loc, in_value);
}

bool Shader::CompileShader(const std::string & in_fileName, GLenum in_shaderType, GLuint & in_outShader)
{
	// �t�@�C�����J��
	std::ifstream shaderFile(in_fileName);

	if (shaderFile.is_open())
	{
		// �e�L�X�g��ǂݍ���ŁA������ɕϊ�����
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		// �V�F�[�_�[��shaderType�ɏ]���č쐬����
		in_outShader = glCreateShader(in_shaderType);
		// �\�[�X������̃R���p�C�������݂�
		glShaderSource(in_outShader, 1, &(contentsChar), nullptr);
		glCompileShader(in_outShader);

		if (!IsCompiled(in_outShader))
		{
			printf("�V�F�[�_�[�R���p�C���Ɏ��s : %s", in_fileName.c_str());
			return false;
		}
	}
	else
	{
		printf("�V�F�[�_�[�t�@�C����������܂���ł��� : %s", in_fileName.c_str());
		return false;
	}


	return true;
}

bool Shader::IsCompiled(GLuint in_shader)
{
	GLint status;

	// �R���p�C���̏�Ԃ�₢���킹��
	glGetShaderiv(in_shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(in_shader, 511, nullptr, buffer);
		printf("GLSL compile�Ɏ��s : \n%s", buffer);

		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{
	GLint status;

	// �����N��Ԃ�₢���킹��
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);

	// �����N��Ԃ̖₢���킹
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);

		glGetProgramInfoLog(m_shaderProgram, 511, nullptr, buffer);
		printf("GLSL Link ��� : \n%s", buffer);

		return false;
	}

	return true;
}
