//-----------------------------------------------------------------------+
// メッシュクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include "VertexArray.h"
#include "tiny_obj_loader.h"
#include "Collision.h"
#include "Renderer.h"

namespace
{
	union Vertex
	{
		float f;
		uint8_t b[4];
	};
}

class Mesh
{
public:

	Mesh();
	~Mesh();

	virtual bool Load(const std::string& in_fileName, class Renderer* in_renderer) = 0;       // メッシュのロード
	void Delete();                                                                // メッシュの解放

	// テクスチャの追加 (メッシュファイルと同階層のディフューズ・スペキュラ・ノーマルを取得)
	void AddTexture(const std::string& in_meshName, class Renderer* in_renderer);

	class VertexArray* GetVertexArray() { return m_vertexArray; }                 // メッシュの頂点配列の取得
	class Texture* GetTexture(size_t in_index);                                      // 指定されたインデックスからテクスチャ取得

	const std::string& GetShaderName() const { return m_shaderName; }             // シェーダー名を取得
	const AABB& GetCollisionBox() const { return m_box; }                         // 当たり判定ボックスを取得

	float GetRadius() const { return m_radius; }                                  // バウンディングスフィアの半径を取得

	// 各種テクスチャの取得
	class Texture* GetDiffuseMap() { return m_diffuseMap; }
	class Texture* GetSpecularMap() { return m_specularMap; }
	class Texture* GetNormalMap() { return m_normalMap; }
	class Texture* GetDepthMap() { return m_depthMap; }

	// テクスチャ配列のサイズ取得
	int GetTextureArraySize() { return m_textures.size(); }

	// タンジェントスペース計算用関数
	void calcTangent(Vector3& destTangent, const Vector3& pos1, const Vector3& pos2, const Vector3& pos3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3);
	void getPosVec(Vector3& destPos, const std::vector<float> fv, int index);
	void getUVVec(Vector2& destUV, const std::vector<float> fv, int index);
	void setTangent(std::vector<float>& destfv, int index, const Vector3& tangent);


protected:

	AABB m_box;

	std::vector<class Texture*> m_textures;
	class Texture* m_diffuseMap;                                // ディフューズマップ
	class Texture* m_specularMap;                               // スペキュラマップ
	class Texture* m_normalMap;                                 // ノーマルマップ
	class Texture* m_depthMap;                                 // シャドウ描画用深度マップ

	class VertexArray* m_vertexArray;                                       // メッシュの頂点配列

	std::string m_shaderName;
	float m_radius;
	float m_specValue;

};