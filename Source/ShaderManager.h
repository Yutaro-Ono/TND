//--------------------------------------------------+
// シェーダマネージャクラス
// シェーダデータの管理クラス
//--------------------------------------------------+
#pragma once
#include <unordered_map>

// シェーダタイプ (連想配列のキー)
enum class SHADER_TYPE : unsigned char
{
	BASIC_MESH = 0,
	BASIC_SKINMESH,

	PHONG_MESH,
	PHONG_SKINMESH,

	NORMAL_MESH,
	NORMAL_SKINMESH,

	SHADOW_DEPTH,
	SHADOW_MESH,
	SHADOW_SKINMESH,
	SHADOW_NORMAL_MESH,
	SHADOW_NORMAL_SKINMESH,

	CUBE_SKYBOX,

	ENVIRONMENT_MAP,

	SPRITE,
	BILLBOARD_SPRITE,
	BILLBOARD_PARTICLE,

	BLOOM,
	BLOOM_GAUSSBLUR,
	BLOOM_DOWNSAMPLING,

	GBUFFER_MESH,
	GBUFFER_SKINMESH,
	GBUFFER_SHADOW_MESH,
	GBUFFER_SHADOW_SKINMESH,

	LIGHT_POINT,
	LIGHT_DIRECTIONAL,

	ALL_SHADER_NUM,
};
// タイプ名取得用関数
const char* GetShaderTypeName(SHADER_TYPE in_type);

class ShaderManager
{
public:

	ShaderManager();
	~ShaderManager();

	void LoadShaders(SHADER_TYPE in_type);

private:

	std::unordered_map<SHADER_TYPE, class Shader*> m_shaders;

};