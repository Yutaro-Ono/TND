#include "MeshObj.h"

MeshObj::MeshObj()
{
}

MeshObj::~MeshObj()
{
}

bool MeshObj::Load(const std::string& in_filePath, Renderer* in_renderer)
{
	// 頂点レイアウトをセット(基本メッシュタイプに固定)
	VertexArray::Layout layout = VertexArray::POS_NORMAL_TEX_TAN;

	tinyobj::attrib_t attrib;                       // 座標コンテナ
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;     // マテリアルコンテナ
	std::string warning, error;                     // 警告・エラー内容の格納

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, in_filePath.c_str()))
	{
		std::cerr << "ERROR: Category: Mesh: LoadObj " << std::endl;
		return false;
	}

	const int attribVertexNum = 3;       // 頂点座標の個数(x, y, z = 3個)
	const int attribNormalNum = 3;       // 法線ベクトルの個数(nx, ny, nz = 3個)
	const int attribTexCoordNum = 2;     // テクスチャ座標の個数(u, v = 2個)
	const int attribTangentNum = 3;

	const int attribStride = attribVertexNum + attribNormalNum + attribTexCoordNum + attribTangentNum;     // 全座標の個数

	int vertexNum = attrib.vertices.size() / attribVertexNum;     // 頂点数

	// インデックスバッファの取得
	std::vector<unsigned int> indices;
	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			indices.push_back(index.vertex_index);
		}
	}

	// タンジェント空間計算用
	Vector3 tangent;
	// 頂点データを確保
	std::vector<float> vertexVec(vertexNum * attribStride);

	// 頂点座標・テクスチャUV格納用配列
	std::vector<Vector3> destPos;
	std::vector<Vector2> uvPos;

	// 形状ループ
	for (const auto& shape : shapes)
	{
		size_t indexOffset = 0;

		// 面(三角形or四角形)のループ
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
		{

			size_t num_vertices = shape.mesh.num_face_vertices[f];

			// 頂点座標・テクスチャUV格納用配列
			std::vector<Vector3> destPos;
			std::vector<Vector2> uvPos;

			// 頂点数ループ(基本3回)
			for (size_t v = 0; v < num_vertices; v++)
			{
				// 面を構成するインデックスを取得
				tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];

				// 頂点座標を頂点配列にコピー
				vertexVec[idx.vertex_index * attribStride + 0] = attrib.vertices[3 * idx.vertex_index + 0];
				vertexVec[idx.vertex_index * attribStride + 1] = attrib.vertices[3 * idx.vertex_index + 2];
				vertexVec[idx.vertex_index * attribStride + 2] = attrib.vertices[3 * idx.vertex_index + 1];

				// 法線データを頂点配列にコピー
				vertexVec[idx.vertex_index * attribStride + 3] = attrib.normals[3 * idx.normal_index + 0];
				vertexVec[idx.vertex_index * attribStride + 4] = attrib.normals[3 * idx.normal_index + 2];
				vertexVec[idx.vertex_index * attribStride + 5] = attrib.normals[3 * idx.normal_index + 1];

				// uvデータを頂点配列にコピー
				vertexVec[idx.vertex_index * attribStride + 6] = attrib.texcoords[2 * idx.texcoord_index + 0];
				vertexVec[idx.vertex_index * attribStride + 7] = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1];


				//-------------------------------------------------------------------------------------------------+
				// 当たり判定計算
				//-------------------------------------------------------------------------------------------------+
				// 頂点位置から　バウンディングスフィアの半径を計算する（原点からの距離が最大のものを半径とする）
				Vector3 pos(vertexVec[idx.vertex_index * attribStride + 0],
					vertexVec[idx.vertex_index * attribStride + 1],
					vertexVec[idx.vertex_index * attribStride + 2]);
				m_radius = Math::Max(m_radius, pos.LengthSq());
				// バウンディングボックスも計算
				if (idx.vertex_index * attribStride + 0 == 0)
				{
					m_box.InitMinMax(pos);
				}
				m_box.UpdateMinMax(pos);


				// ポリゴンを構成する頂点座標を一時保存
				destPos.push_back(Vector3(vertexVec[idx.vertex_index * attribStride + 0],
					vertexVec[idx.vertex_index * attribStride + 1],
					vertexVec[idx.vertex_index * attribStride + 2]));
				// テクスチャ座標を一時保存
				uvPos.push_back(Vector2(vertexVec[idx.vertex_index * attribStride + 6],
					vertexVec[idx.vertex_index * attribStride + 7]));
			}



			// タンジェント計算
			calcTangent(tangent, destPos[0], destPos[1], destPos[2], uvPos[0], uvPos[1], uvPos[2]);
			// 格納した3つの頂点座標とUVから算出されたタンジェントを頂点配列に格納
			for (int i = 0; i < 3; i++)
			{
				setTangent(vertexVec, indexOffset + i, tangent);
			}
			destPos.clear();
			uvPos.clear();

			indexOffset += num_vertices;
		}
	}

	// 頂点配列をGPUに登録
	m_vertexArray = new VertexArray(static_cast<float*>(vertexVec.data()), static_cast<unsigned>(vertexNum), layout, indices.data(), static_cast<unsigned>(indices.size()));


	// テクスチャの追加処理
	AddTexture(in_filePath, in_renderer);


	return true;
}
