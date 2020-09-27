//---------------------------------------------------------------------------+
// LevelManagerクラス.  マップのブロック配置、オブジェクト配置統合クラス
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//---------------------------------------------------------------------------+
#include "LevelManager.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "LevelBlock.h"
#include "RapidJsonHelper.h"
#include "FlameStatue.h"
#include "AngelStatue.h"
#include "Skydome.h"

#include <vector>
#include <string>
#include <sstream>


// コンストラクタ
LevelManager::LevelManager(int in_stageNum)
	:m_camera(nullptr)
{
	// マップの配置を保存したjsonへのファイルパスを生成
	std::stringstream ssMap;
	ssMap << "Data/Strings/map_" << in_stageNum << ".json";
	std::string mapPath = ssMap.str();

	// ステージメッシュ読み込み
	// m_blockMeshes.push_back(&emptySpace);
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Block_Wall/mapBlock1.gpmesh"));
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Block_Wall/mapBlock1.gpmesh"));
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Block_Floor/BLOCK_FLOOR.gpmesh"));
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Pillar/PILLAR.gpmesh"));
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Rock/Object_Rock.gpmesh"));

	// オブジェクト用の配列にメッシュを格納
	m_objectMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Pillar/PILLAR.gpmesh"));
	m_objectMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Rock/Object_Rock.gpmesh"));
	m_objectMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Objects/Angel_Statue/Statue_Ruins.gpmesh"));
	m_objectMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/FC/Skydome/Skydome_Sunny.gpmesh"));



	//-----------------------------------------------------------------------------------------+
	// ステージデータ読み込み
	//-----------------------------------------------------------------------------------------+
	// 地面
	std::vector<std::vector<int>> groundData;
	if (!ReadTiledJson(groundData, mapPath.c_str(), "layer_0"))
	{
		printf("<Level> Map Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}
	// オブジェクト
	std::vector<std::vector<int>> objData;
	if (!ReadTiledJson(objData, mapPath.c_str(), "layer_1"))
	{
		printf("<Level> Map Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}


	LevelBlock* block;
	const float blockSize = 200.0f;

	int sizeX, sizeY;
	sizeX = groundData[0].size();
	sizeY = groundData.size();
	float offsetY = sizeY * blockSize;

	// マップブロックを登録(地形)
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			if (groundData[iy][ix] == 0)
			{
				continue;
			}

			if (groundData[iy][ix] < 5)
			{
				block = new LevelBlock();
				block->SetMesh(m_blockMeshes[groundData[iy][ix]]);
				block->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, -100.0f));
			}

		}
	}

	// マップブロックを登録(オブジェクト)
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			if (objData[iy][ix] == 0)
			{
				continue;
			}

			// 柱オブジェクト
			if (objData[iy][ix] == 3)
			{
				block = new LevelBlock();
				block->SetMesh(m_blockMeshes[objData[iy][ix]]);
				block->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, 0.0f));

				continue;
			}

			// 岩オブジェクト
			if (objData[iy][ix] == 4)
			{
				block = new LevelBlock();
				block->SetMesh(m_blockMeshes[objData[iy][ix]]);
				block->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, -50.0f));

				continue;
			}

			// 壁ブロック
			if (objData[iy][ix] < 5)
			{
				block = new LevelBlock();
				block->SetMesh(m_blockMeshes[objData[iy][ix]]);
				block->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, 100.0f));
			}
		}
	}

}


// デストラクタ
LevelManager::~LevelManager()
{
	// 地形ブロックのクリア
	m_blockMeshes.clear();
	// オブジェクトのクリア
	m_objectMeshes.clear();

}


// レベルのロード処理 ※未実装
bool LevelManager::LoadLevel(const char * in_json, int in_layerNum)
{
	return true;
}


// ビューレンジのセッター
void LevelManager::SetViewRange(float in_width, float in_height)
{
	m_leftUpPos.x = -in_width * 0.5f;
	m_rightDownPos.x = in_width * 0.5f;
	m_leftUpPos.y = -in_height * 0.5f;
	m_rightDownPos.y = in_height * 0.5f;
}


// 更新処理
void LevelManager::Update()
{
	// 処理なし
}


// ともし火配置用のjsonを取得する
bool LevelManager::ReadTiledJson(std::vector<std::vector<int>>& mapData, const char * in_fileName, const char * in_layerName)
{
	//RapidJsonドキュメントとして開けるか？
	rapidjson::Document doc;
	if (!OpenJsonFile(doc, in_fileName))
	{
		return false;
	}

	// 開いたDocmentのtype は mapか？
	if (!IsExistMember(doc, "type", "map"))
	{
		return false;
	}

	// 開いたDocumentにlayerはあるか？
	if (!IsExistArrayName(doc, "layers"))
	{
		return false;
	}
	// layer数取得
	auto layer = doc["layers"].GetArray();
	int  layerSize = layer.Size();

	// レイヤー名から該当レイヤーの添え字値を調べる
	std::string layerNameString(in_layerName);
	int layerIndex = FindLayerIndex(layer, layerNameString);
	if (layerIndex < 0)
	{
		return false;
	}

	// layer内にdataはある？    
	rapidjson::Value::ConstMemberIterator itr = layer[layerIndex].FindMember("data");
	if (itr == layer[layerIndex].MemberEnd())
	{
		printf("レイヤー名:%sにマップデータがありません\n", in_layerName);
		return false;
	}

	// レイヤーよりrapidjson配列作成
	const rapidjson::Value& rapidArrayData = layer[layerIndex]["data"].GetArray();
	int width, height;
	width = layer[layerIndex]["width"].GetInt();
	height = layer[layerIndex]["height"].GetInt();

	// ユーザー配列確保 行方向、列方向の順番にサイズ確保
	mapData.resize(height);
	for (auto &mapIter : mapData)
	{
		mapIter.resize(width);
	}

	// ユーザー配列（2次元）にデータをコピーする
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			mapData[y][x] = rapidArrayData[y * width + x].GetInt();
		}
	}

	return true;
}


// json内のlayerを調べる
int LevelManager::FindLayerIndex(rapidjson::GenericArray<false, rapidjson::Value>& layer, std::string & layerName)
{
	int  layerSize = layer.Size();

	int i;
	std::string layerNameString(layerName);
	for (i = 0; i < layerSize; i++)
	{
		std::string currentLayerName = layer[i]["name"].GetString();
		printf("%s\n", currentLayerName.c_str());
		if (layerNameString == currentLayerName)
		{
			break;
		}
	}
	if (i == layerSize)
	{
		return -1;
	}
	return i;
}
