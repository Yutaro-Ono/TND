//-----------------------------------------------------------------------+
// メッシュクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
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

// テクスチャタイプ
enum class TEXTURE_TYPE
{
	DIFFUSE_MAP,     // ディフューズ (ベースカラ―)
	SPECULAR_MAP,    // スペキュラ (反射光)
	NORMAL_MAP,      // ノーマル (法線)
	EMISSIVE_MAP     // エミッシブ (放射光)
};

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
	class Texture* GetTexture(TEXTURE_TYPE in_type);

	const std::string& GetShaderName() const { return m_shaderName; }             // シェーダー名を取得
	const AABB& GetCollisionBox() const { return m_box; }                         // 当たり判定ボックスを取得

	float GetRadius() const { return m_radius; }                                  // バウンディングスフィアの半径を取得


	// テクスチャ配列のサイズ取得
	int GetTextureArraySize() { return m_textureStages.size(); }

	// 指定したタイプのテクスチャ番号を取得 (描画時に使用)
	int GetTextureID(TEXTURE_TYPE in_type) { return m_textureStages[in_type]; }


	// 指定したタイプのテクスチャを取得し、テクスチャ番号を返す
	int CreateTextureStage(TEXTURE_TYPE in_type, std::string& in_filePath);

	// タンジェントスペース計算用関数
	void calcTangent(Vector3& destTangent, const Vector3& pos1, const Vector3& pos2, const Vector3& pos3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3);
	void getPosVec(Vector3& destPos, const std::vector<float> fv, int index);
	void getUVVec(Vector2& destUV, const std::vector<float> fv, int index);
	void setTangent(std::vector<float>& destfv, int index, const Vector3& tangent);


protected:

	AABB m_box;

	std::unordered_map<TEXTURE_TYPE, int> m_textureStages;      // key:テクスチャタイプ  val:テクスチャステージ(GL上で割り振られた番号)

	class VertexArray* m_vertexArray;                           // メッシュの頂点配列

	std::string m_shaderName;
	float m_radius;
	float m_specValue;

};