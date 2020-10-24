//-----------------------------------------------------------------------+
// メッシュクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"

#include <SDL_log.h>
#include "Math.h"





Mesh::Mesh()
	:m_vertexArray(nullptr)
	,m_radius(0.0f)
	,m_specValue(100.0f)
	,m_diffuseMap(nullptr)
	,m_specularMap(nullptr)
	,m_normalMap(nullptr)
{
}

Mesh::~Mesh()
{
}


void Mesh::Delete()
{
	delete m_vertexArray;
	m_vertexArray = nullptr;
}

Texture * Mesh::GetTexture(size_t in_index)
{

	if (in_index < m_textures.size())
	{
		return m_textures[in_index];
	}
	else
	{
		return nullptr;
	}

}

// 入力されたメッシュファイル名からディフューズマップ、スペキュラーマップ、法線マップをロード・追加する
void Mesh::AddTexture(const std::string& in_meshName, class Renderer* in_renderer)
{
	std::string pngPath = in_meshName;

	int extNum = pngPath.find_last_of(".");
	pngPath = pngPath.substr(0, extNum);            // 拡張子以外のメッシュファイル名を取得

	// ディフューズ読み込み
	Texture* t = in_renderer->GetTexture(pngPath + "_DiffuseMap.png");

	if (t != nullptr)
	{
		m_diffuseMap = t;
		m_textures.emplace_back(t);
	}
	else
	{
		std::cout << "Obj Mesh : Load [Diffuse] Texture Error\n";
	}

	// スペキュラ読み込み
	t = nullptr;
	t = in_renderer->GetTexture(pngPath + "_SpecularMap.png");

	if (t != nullptr)
	{
		m_specularMap = t;
		m_textures.emplace_back(t);
	}
	else
	{
		std::cout << "Obj Mesh : Load [Specular] Texture Error\n";
	}

	// 法線マップ読み込み
	t = nullptr;
	t = in_renderer->GetTexture(pngPath + "_NormalMap.png");

	if (t != nullptr)
	{
		m_normalMap = t;
		m_textures.emplace_back(t);
	}
	else
	{
		std::cout << "Obj Mesh : Load [Normal] Texture Error\n";
	}


}
