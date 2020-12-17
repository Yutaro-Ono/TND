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

// コンストラクタ
ShaderManager::ShaderManager()
{
	// 全シェーダのロード
	for (int i = 0; i < (int)SHADER_TYPE::ALL_SHADER_NUM; i++)
	{
		LoadShaders((SHADER_TYPE)i);
	}
}

ShaderManager::~ShaderManager()
{
}

// タイプに合わせたシェーダのロード
void ShaderManager::LoadShaders(SHADER_TYPE in_type)
{


	// Jsonファイルをドキュメントにロード
	rapidjson::Document doc;
	if (!OpenJsonFile(doc, "Data/Strings/ShaderList/ShaderList.json"))
	{
		SDL_Log("Failed to Load Score %s", "Data/Strings/ShaderList/ShaderList.json");
	}

	std::string key;
	// IDを設定
	key = std::to_string((int)in_type);

	// スコア情報を返す
	OutputInt(doc, key.c_str());
	Shader* shader = new Shader();
	
	m_shaders.emplace(in_type, shader);
}

const char* GetShaderTypeName(SHADER_TYPE in_type)
{
	return nullptr;
}
