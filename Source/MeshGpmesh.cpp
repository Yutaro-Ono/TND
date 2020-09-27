#include "MeshGpmesh.h"



MeshGpmesh::MeshGpmesh()
{
}

MeshGpmesh::~MeshGpmesh()
{
}

bool MeshGpmesh::Load(const std::string& in_filePath, Renderer* in_renderer)
{
	std::ifstream file(in_filePath);
	if (!file.is_open())
	{
		printf("File Not Found : Mesh %s\n", in_filePath.c_str());
		return false;
	}

	// JSONの解析を行う
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	// JSONオブジェクトか？
	if (!doc.IsObject())
	{
		printf("Mesh %s is not valid json", in_filePath.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// バージョンチェック
	if (ver != 1)
	{
		printf("Mesh %s not version 1", in_filePath.c_str());
		return false;
	}

	m_shaderName = doc["shader"].GetString();

	// 頂点レイアウトとサイズをファイルからセット
	VertexArray::Layout layout = VertexArray::POS_NORMAL_TEX;
	size_t vertSize = 8;
	// size_t vertSize = 14;     // 法線マップを入れる場合 vx,vy,vz,nx,ny,nz,u,v,tx,ty,tz,bitx,bity,bitz

	std::string vertexFormat = doc["vertexformat"].GetString();
	if (vertexFormat == "PosNormSkinTex")
	{
		layout = VertexArray::POS_NORMAL_SKIN_TEX;
		// This is the number of "Vertex" unions, which is 8 + 2 (for skinning)s　1個の頂点の集合の数　８　＋　２（スキニング分）
		// 3 (位置xyz) + 3(法線xyz) + 2(Boneと重み）＋　2(UV)  の計　10個分（40byte) 　
		vertSize = 10;
	}

	// テクスチャのロード
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		printf("Mesh %s has no textures, there should be at least one", in_filePath.c_str());
		return false;
	}

	// スペキュラ強度
	m_specValue = static_cast<float>(doc["specularPower"].GetDouble());

	// テクスチャ読み込み
	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		// このテクスチャを既に読みこんでるか
		std::string texName = textures[i].GetString();
		Texture* t = in_renderer->GetTexture(texName);
		if (t == nullptr)
		{
			// テクスチャ読み込みのトライ
			t = in_renderer->GetTexture(texName);
			if (t == nullptr)
			{
				printf("Mesh %s has no textures, there should be at least one", in_filePath.c_str());
			}
		}
		m_textures.emplace_back(t);
	}

	// 頂点読み込み
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		printf("Mesh %s has no vertices", in_filePath.c_str());
		return false;
	}

	// 頂点データ
	std::vector<Vertex> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	m_radius = 0.0f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// 現時点で、８つの要素(位置xyz 法線xyz テクスチャuvの要素）が入っている
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray())
		{
			printf("Unexpected vertex format for %s", in_filePath.c_str());
			return false;
		}

		// 頂点位置から　バウンディングスフィアの半径を計算する（原点からの距離が最大のものを半径とする）
		Vector3 pos(static_cast<float>(vert[0].GetDouble()),
			static_cast<float>(vert[1].GetDouble()),
			static_cast<float>(vert[2].GetDouble()));
		m_radius = Math::Max(m_radius, pos.LengthSq());
		// バウンディングボックスも計算
		if (i == 0)
		{
			m_box.InitMinMax(pos);
		}
		m_box.UpdateMinMax(pos);

		// 頂点レイアウトが PosNormTexなら（ボーンが無い）
		if (layout == VertexArray::POS_NORMAL_TEX)
		{
			Vertex v;
			// Add the floats　float値を追加
			for (rapidjson::SizeType j = 0; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}
		}
		else // ボーンデータ入りなら　PosNormSkinTexなら
		{
			Vertex v;
			// Add pos/normal　頂点と法線を追加　6個分
			for (rapidjson::SizeType j = 0; j < 6; j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}

			// Add skin information　スキン情報追加（ボーン番号:unsigned charの1バイト分）
			for (rapidjson::SizeType j = 6; j < 14; j += 4)  //ループとしては2回転する　1回転目はボーン番号、2回転目はボーンウェイトをintとして取ってくる（使用時に浮動小数化するっぽい）
			{
				v.b[0] = vert[j].GetUint();
				v.b[1] = vert[j + 1].GetUint();
				v.b[2] = vert[j + 2].GetUint();
				v.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(v);
			}

			// Add tex coords　テクスチャ座標
			for (rapidjson::SizeType j = 14; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}
		}
	}

	// We were computing length squared earlier　バウンディングスフィアの半径を計算
	m_radius = Math::Sqrt(m_radius);

	// Load in the indices　頂点インデックスをロード
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		printf("Mesh %s has no indices", in_filePath.c_str());
		return false;
	}

	//頂点インデックスを引っ張ってくる
	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			printf("Invalid indices for %s", in_filePath.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	// Now create a vertex array　頂点配列を作成する
	m_vertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
		layout, indices.data(), static_cast<unsigned>(indices.size()));
	return true;
}
