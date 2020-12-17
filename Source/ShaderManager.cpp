#include "ShaderManager.h"
#include "Shader.h"
#include "RapidJsonHelper.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <SDL.h>

const char* ShaderTypeName[static_cast<int>(SHADER_TYPE::ALL_SHADER_NUM)] =
{
	"BASIC_MESH"
	"BASIC_SKINMESH"
	"PHONG_MESH"
	"PHONG_SKINMESH"
	"NORMAL_MESH"
	"NORMAL_SKINMESH"
	"SHADOW_DEPTH"
	"SHADOW_MESH"
	"SHADOW_SKINMESH"
	"SHADOW_NORMAL_MESH"
	"SHADOW_NORMAL_SKINMESH"
	"CUBE_SKYBOX"
	"ENVIRONMENT_MAP"
	"SPRITE"
	"BILLBOARD_SPRITE"
	"BILLBOARD_PARTICLE"
	"BLOOM"
	"BLOOM_GAUSSBLUR"
	"BLOOM_DOWNSAMPLING"
	"ALL_SHADER_NUM"
};

// �R���X�g���N�^
ShaderManager::ShaderManager()
{
	// �S�V�F�[�_�̃��[�h
	for (int i = 0; i < (int)SHADER_TYPE::ALL_SHADER_NUM; i++)
	{
		LoadShaders((SHADER_TYPE)i);
	}
}

ShaderManager::~ShaderManager()
{
}

// �^�C�v�ɍ��킹���V�F�[�_�̃��[�h
void ShaderManager::LoadShaders(SHADER_TYPE in_type)
{


	// Json�t�@�C�����h�L�������g�Ƀ��[�h
	rapidjson::Document doc;
	if (!OpenJsonFile(doc, "Data/Strings/ShaderList/ShaderList.json"))
	{
		SDL_Log("Failed to Load Score %s", "Data/Strings/ShaderList/ShaderList.json");
	}

	std::string key;
	// ID��ݒ�
	key = std::to_string((int)in_type);

	// �X�R�A����Ԃ�
	OutputInt(doc, key.c_str());
	Shader* shader = new Shader();
	
	m_shaders.emplace(in_type, shader);
}

const char* GetShaderTypeName(SHADER_TYPE in_type)
{
	return nullptr;
}
