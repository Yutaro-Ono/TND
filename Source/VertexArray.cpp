//------------------------------------------------------------------------------+
// 頂点クラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//------------------------------------------------------------------------------+
#include "VertexArray.h"
#include <glad/glad.h>

// 標準コンストラクタ
VertexArray::VertexArray()
{
}

//-------------------------------------------------------------------------------------------------------------------+
// 頂点レイアウト
// PosNormTex = 8 * sizeof(float) = 32 bytes
// | 位置      | 法線      | Texture|
// | x | y | z | x | y | z | u | v | 
//
// posNormSkinTex
// PosNormSkinTex = (8 * sizeof(float)) + (8 * sizeof(char)) = 40 bytes
// | 位置      | 法線       | Bones   | weight  |Texture|
// | x | y | z | x | y | z | char[4] | char[4] | u | v |
//                                    ※weightの確保はcharだが、精度が必要ないので8bit固定小数として使用する
//-------------------------------------------------------------------------------------------------------------------+
VertexArray::VertexArray(const void * in_verts, unsigned int in_vertsNum, Layout in_layout, const unsigned int * in_inDices, unsigned int in_numInDices)
	:m_vertsNum(in_vertsNum)
	,m_numInDices(in_numInDices)
{
	// 頂点配列の作成
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// 頂点レイアウトが スケルタルモデルなら　ボーンID、影響度分をサイズ増やす
	unsigned vertexSize = 8 * sizeof(float);
	if (in_layout == POS_NORMAL_SKIN_TEX)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
	}
	if (in_layout == POS_NORMAL_TEX_TAN)
	{
		vertexSize = 11 * sizeof(float);
	}
	if (in_layout == POS_NORMAL_SKIN_TEX_TAN)
	{
		vertexSize = 11 * sizeof(float) + 8 * sizeof(char);
	}
	// 頂点バッファの作成
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, in_vertsNum * vertexSize, in_verts, GL_STATIC_DRAW);

	// インデックスバッファの作成
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, in_numInDices * sizeof(unsigned int), in_inDices, GL_STATIC_DRAW);

	// 頂点属性
	if (in_layout == POS_NORMAL_TEX)
	{
		// float 3個分　→　位置 x,y,z　位置属性をセット
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		// 次のfloat 3個分 → 法線 nx, ny, nz　法線属性をセット
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));
		// 次のfloat 2個分 u, v  テクスチャ座標属性をセット
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
	}
	else if (in_layout == POS_NORMAL_SKIN_TEX)
	{
		// float 3個分　→　位置 x,y,z　位置属性をセット
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		// 次のfloat 3個分 → 法線 nx, ny, nz　法線属性をセット
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));
		//　影響受けるボーンインデックス番号  (int型をキープ)
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
		// ボーンウェイト情報 (float に変換)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4));
		// 次のfloat 2個分 u, v  テクスチャ座標属性をセット
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8));
	}
	else if (in_layout == POS_NORMAL_TEX_TAN)
	{
		// 0 : 頂点座標
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);
		// 1 : 法線座標
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(3 * sizeof(float)));
		// 2 : テクスチャ座標
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(6 * sizeof(float)));
		// 3 : 接ベクトル (タンジェント)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(8 * sizeof(float)));
	}
	else if (in_layout == POS_NORMAL_SKIN_TEX_TAN)
	{
		//// float 3個分　→　位置 x,y,z　位置属性をセット
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		//// 次のfloat 3個分 → 法線 nx, ny, nz　法線属性をセット
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
		//	reinterpret_cast<void*>(sizeof(float) * 3));
		//// 次のfloat 2個分 u, v  テクスチャ座標属性をセット
		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
		//	reinterpret_cast<void*>(sizeof(float) * 6));
		//// 3 : 接ベクトル (タンジェント)
		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertexSize, 
		//	reinterpret_cast<void*>(sizeof(float) * 8));
		////　影響受けるボーンインデックス番号  (int型をキープ)
		//glEnableVertexAttribArray(4);
		//glVertexAttribIPointer(4, 4, GL_UNSIGNED_BYTE, vertexSize,
		//	reinterpret_cast<void*>(sizeof(float) * 11));
		//// ボーンウェイト情報 (float に変換)
		//glEnableVertexAttribArray(5);
		//glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
		//	reinterpret_cast<void*>(sizeof(float) * 11 + sizeof(char) * 4));

		// float 3個分　→　位置 x,y,z　位置属性をセット
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		// 次のfloat 3個分 → 法線 nx, ny, nz　法線属性をセット
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));
		//　影響受けるボーンインデックス番号  (int型をキープ)
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
		// ボーンウェイト情報 (float に変換)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4));
		// 次のfloat 2個分 u, v  テクスチャ座標属性をセット
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8));
		// 3 : 接ベクトル (タンジェント)
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 8 + sizeof(char) * 8));
	}
}

VertexArray::VertexArray(const float * in_verts, unsigned int in_vertsNum, const unsigned int * in_inDices, unsigned int in_numInDices)
	:m_vertsNum(in_vertsNum)
	,m_numInDices(in_numInDices)
{

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, in_vertsNum * 8 * sizeof(float), in_verts, GL_STATIC_DRAW);

	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, in_numInDices * sizeof(unsigned int), in_inDices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		reinterpret_cast<void*>(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		reinterpret_cast<void*>(sizeof(float) * 6));
}

// キューブマップ用頂点配列
void VertexArray::CreateCubeVerts()
{
	float cubeVertices[] =
	{

		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int indices[] =
	{
		 0,  1,  2,  0,  2,  3,    // 前面
		 4,  5,  6,  4,  6,  7,    // 背面
		 8,  9, 10,  8, 10, 11,    // 上面
		12, 13, 14, 12, 14, 15,    // 底面
		16, 17, 18, 16, 18, 19,    // 右側面
		20, 21, 22, 20, 22, 23     // 左側面
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 36 * 8 * sizeof(float), cubeVertices, GL_STATIC_DRAW);
	// アトリビュートへのセット(頂点座標のみ)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

// スクリーン描画用頂点配列
void VertexArray::CreateScreenVerts()
{
	// スクリーン全体の四角形用頂点配列
	float quadVertices[] = 
	{
		// ポジション   // テクスチャ座標
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		 -1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// スクリーン全体を描く四角形用 VAO
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

// デストラクタ
VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_indexBuffer);
	glDeleteVertexArrays(1, &m_VAO);
}

void VertexArray::SetActive()
{
	glBindVertexArray(m_VAO);
}
