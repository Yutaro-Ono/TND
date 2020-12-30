//-----------------------------------------------------------------------+
// メッシュクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "ShadowMap.h"
#include <SDL_log.h>
#include "Math.h"

Mesh::Mesh()
	:m_vertexArray(nullptr)
	,m_radius(0.0f)
	,m_specValue(100.0f)
	,m_diffuseMap(nullptr)
	,m_specularMap(nullptr)
	,m_normalMap(nullptr)
	,m_depthMap(nullptr)
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
	std::string fileName = pngPath + "_DiffuseMap.png";
	Texture* t = in_renderer->GetTexture(fileName);

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
	fileName = pngPath + "_SpecularMap.png";
	t = in_renderer->GetTexture(fileName);

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
	fileName = pngPath + "_NormalMap.png";
	t = in_renderer->GetTexture(fileName);

	if (t != nullptr)
	{
		m_normalMap = t;
		m_textures.emplace_back(t);
	}
	else
	{
		std::cout << "Obj Mesh : Load [Normal] Texture Error\n";
	}

	// デプスマップをレンダラー経由で取得
	m_depthMap = new Texture();
	m_depthMap->SetTextureID(in_renderer->GetShadowMap()->GetDepthMap());

}

//----------------------------------------------------------------------------------------+
// タンジェントベクトル計算
// 三角形面のpos1～pos3, uv0～uv2 を使ってTangentベクトルをdestTangentに返す
// 
// inout destTangent Tangentを格納する変数
// in : pos0, pos1, pos2 三角形面の頂点
// in : uv0, uv1, uv2 三角形面の頂点に対応するUV座標
//----------------------------------------------------------------------------------------+
void Mesh::calcTangent(Vector3& destTangent, const Vector3& pos1, const Vector3& pos2, const Vector3& pos3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3)
{
	Vector3 edge1, edge2;
	edge1 = pos2 - pos1;
	edge2 = pos3 - pos1;
	Vector2 deltaUV1 = uv2 - uv1;
	Vector2 deltaUV2 = uv3 - uv1;
	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	destTangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	destTangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	destTangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	destTangent.Normalize();
}

//----------------------------------------------------------------------------------------+
// 頂点配列から頂点座標ベクトルだけを取り出す
// inout : destUV 頂点のUV座標を格納する変数
// in : fv 頂点&UVデータ配列の先頭アドレスが格納されたポインタ
// index 頂点&UVデータ配列のインデックス値
//----------------------------------------------------------------------------------------+
void Mesh::getPosVec(Vector3& destPos, const std::vector<float> fv, int index)
{
	destPos.x = fv[index * 11 + 0];
	destPos.y = fv[index * 11 + 1];
	destPos.z = fv[index * 11 + 2];
}

//----------------------------------------------------------------------------------------+
// 頂点配列からUV値だけを取り出す
// inout : destUV 頂点のUV座標を格納する変数
// in : fv 頂点&UVデータ配列の先頭アドレスが格納されたポインタ
// index 頂点&UVデータ配列のインデックス値
//----------------------------------------------------------------------------------------+
void Mesh::getUVVec(Vector2& destUV, const std::vector<float> fv, int index)
{
	destUV.x = fv[index * 11 + 6 + 0];
	destUV.y = fv[index * 11 + 6 + 1];
}

//----------------------------------------------------------------------------------------+
// 頂点にTangentを書き込む
// inout : destfv 頂点＆UV&Tangent 配列の先頭アドレス
// in : index 頂点＆UV&Tangent 配列の書き込み先インデックス
// in tangent 頂点＆UV&Tangent 配列に書き込みたい tangent値
//----------------------------------------------------------------------------------------+
void Mesh::setTangent(std::vector<float>& destfv, int index, const Vector3& tangent)
{
	if (destfv.size() > index * 11 + 8 + 2)
	{
		destfv[index * 11 + 8 + 0] = tangent.x;
		destfv[index * 11 + 8 + 1] = tangent.y;
		destfv[index * 11 + 8 + 2] = tangent.z;
	}

}