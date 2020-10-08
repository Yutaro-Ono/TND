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

				//// 頂点座標を頂点配列にコピー
				//vertexVec[idx.vertex_index * attribStride + 0] = attrib.vertices[3 * idx.vertex_index + 0];
				//vertexVec[idx.vertex_index * attribStride + 1] = attrib.vertices[3 * idx.vertex_index + 1];
				//vertexVec[idx.vertex_index * attribStride + 2] = attrib.vertices[3 * idx.vertex_index + 2];

				//// 法線データを頂点配列にコピー
				//vertexVec[idx.vertex_index * attribStride + 3] = attrib.normals[3 * idx.normal_index + 0];
				//vertexVec[idx.vertex_index * attribStride + 4] = attrib.normals[3 * idx.normal_index + 1];
				//vertexVec[idx.vertex_index * attribStride + 5] = attrib.normals[3 * idx.normal_index + 2];

				//// uvデータを頂点配列にコピー
				//vertexVec[idx.vertex_index * attribStride + 6] = attrib.texcoords[2 * idx.texcoord_index + 0];
				//vertexVec[idx.vertex_index * attribStride + 7] = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1];

				// 頂点座標を頂点配列にコピー
				vertexVec[idx.vertex_index * attribStride + 0] = attrib.vertices[3 * idx.vertex_index + 0];
				vertexVec[idx.vertex_index * attribStride + 1] = attrib.vertices[3 * idx.vertex_index + 2];
				vertexVec[idx.vertex_index * attribStride + 2] = attrib.vertices[3 * idx.vertex_index + 1];

				// 法線データを頂点配列にコピー
				vertexVec[idx.vertex_index * attribStride + 3] = attrib.normals[3 * idx.normal_index + 0];
				vertexVec[idx.vertex_index * attribStride + 4] = attrib.normals[3 * idx.normal_index + 2];
				vertexVec[idx.vertex_index * attribStride + 5] = attrib.normals[3 * idx.normal_index + 1];

				// uvデータを頂点配列にコピー
				vertexVec[idx.vertex_index * attribStride + 6] = attrib.texcoords[2 * idx.texcoord_index + 1];
				vertexVec[idx.vertex_index * attribStride + 7] = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 0];


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
			calcTangent(tangent, destPos[0], destPos[2], destPos[1], uvPos[0], uvPos[2], uvPos[1]);
			// 格納した3つの頂点座標とUVから算出されたタンジェントを頂点配列に格納
			for (int i = 0; i < 3; i++)
			{
				setTangent(vertexVec, indexOffset + i, tangent);
				setTangent(vertexVec, indexOffset + i, tangent);
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



//----------------------------------------------------------------------------------------+
// タンジェントベクトル計算
// 三角形面のpos1～pos3, uv0～uv2 を使ってTangentベクトルをdestTangentに返す
// 
// inout destTangent Tangentを格納する変数
// in : pos0, pos1, pos2 三角形面の頂点
// in : uv0, uv1, uv2 三角形面の頂点に対応するUV座標
//----------------------------------------------------------------------------------------+
void MeshObj::calcTangent(Vector3& destTangent, const Vector3& pos1, const Vector3& pos2, const Vector3& pos3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3)
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
void MeshObj::getPosVec(Vector3& destPos, const std::vector<float> fv, int index)
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
void MeshObj::getUVVec(Vector2& destUV, const std::vector<float> fv, int index)
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
void MeshObj::setTangent(std::vector<float>& destfv, int index, const Vector3& tangent)
{
	if (destfv.size() > index * 11 + 8 + 2)
	{
		destfv[index * 11 + 8 + 0] = tangent.x;
		destfv[index * 11 + 8 + 1] = tangent.y;
		destfv[index * 11 + 8 + 2] = tangent.z;
	}

}
