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
#include "LevelTerrain.h"
#include "BridgeObject.h"
#include "ClientActor.h"
#include "PatrolPoint.h"
#include "RapidJsonHelper.h"
#include "GameWorld.h"
#include "PointLight.h"
#include "LampObject.h"
#include "LoadScreen.h"
#include <vector>
#include <string>
#include <sstream>


const int LevelManager::MAP_INDEX_GROUND = 16;


// コンストラクタ
LevelManager::LevelManager(GameWorld* in_world, int in_stageNum)
	:m_camera(nullptr)
{
	// マップの配置を保存したjsonへのファイルパスを生成
	std::stringstream ssMap;
	ssMap << "Data/Strings/map_" << in_stageNum << ".json";
	std::string mapPath = ssMap.str();

	// ステージメッシュ読み込み
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Objects/GroundBase/Ground.OBJ"));
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Objects/Street/Street.OBJ"));
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Objects/Building/0/Build11.OBJ"));
	//m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Objects/Bridge/Bridge_Lamps/Bridge_Lamps.OBJ"));
	m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Objects/Lamp/SM_Lamp.OBJ"));
	//m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Body_Internal.OBJ"));
	//m_blockMeshes.push_back(GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Objects/Sphere/Sphere.OBJ"));


	
	//-----------------------------------------------------------------------------------------+
    // ステージデータ読み込み
    //-----------------------------------------------------------------------------------------+
    // 地面
	std::vector<std::vector<int>> groundData;
	if (!ReadTiledJson(groundData, mapPath.c_str(), "layer_Ground"))
	{
		printf("<Level> Map Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}

	// 壁
	std::vector<std::vector<int>> wallData;
	if (!ReadTiledJson(wallData, mapPath.c_str(), "layer_wall"))
	{
		printf("<Level> Street Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}


	// 建物
	std::vector<std::vector<int>> buildingData;
	if (!ReadTiledJson(buildingData, mapPath.c_str(), "layer_Building"))
	{
		printf("<Level> Building Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}


	// 依頼人
	std::vector< std::vector<int>> clientData;
	if (!ReadTiledJson(clientData, mapPath.c_str(), "layer_human"))
	{
		printf("<Level> client Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}


	// ヘリ巡回ポイント
	std::vector<std::vector<int>> patrolPointData;
	if (!ReadTiledJson(patrolPointData, mapPath.c_str(), "layer_patrolPoint"))
	{
		printf("<Level> patrolPoint Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}


	// ポイントライト
	std::vector<std::vector<int>> pointLightData;
	if (!ReadTiledJson(pointLightData, mapPath.c_str(), "layer_pointLight"))
	{
		printf("<Level> pointLight Data Load : Failed\n");
		GAME_INSTANCE.SetShutDown();
		return;
	}


	LevelBlock* block;
	LevelTerrain* terrain;
	const float blockSize = 200.0f;
	const float floorZoffset = -20.0f;

	int sizeX, sizeY;
	sizeX = groundData[0].size();
	sizeY = groundData.size();
	float offsetY = sizeY * blockSize;
	// マップブロックを登録(地形)
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			// 地面オブジェクトはTiled上で16で登録されている
			if (groundData[iy][ix] == MAP_INDEX_GROUND)
			{
				terrain = new LevelTerrain(m_blockMeshes[0], LevelTerrain::TERRAIN_TYPE::TYPE_GLASS);
				terrain->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, floorZoffset));
			}

			// 道路オブジェクトはTiled上で1～11で登録されている
			if (groundData[iy][ix] >= 1 && groundData[iy][ix] <= 11)
			{
				terrain = new LevelTerrain(m_blockMeshes[1], LevelTerrain::TERRAIN_TYPE::TYPE_STREET);
				terrain->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, floorZoffset));
			}
		}
	}
	groundData.clear();

	// ロード処理
	GAME_INSTANCE.GetLoadScreen()->AddGauge();

	// マップブロックを登録(壁)
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			// オブジェクトはTiled上で18で登録されている
			if (wallData[iy][ix] == 19)
			{
				block = new LevelBlock();
				block->SetMesh(m_blockMeshes[0]);
				block->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, floorZoffset + 25.0f));
				block->SetScale(1.1f);
				block->SetMeshVisible();
			}
		}
	}
	wallData.clear();

	// ロード処理
	GAME_INSTANCE.GetLoadScreen()->AddGauge();

	// マップブロックを登録(建物)
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			if (buildingData[iy][ix] == 19)
			{
				// ビルとしてインスタンスを生成
				block = new LevelBlock();
				block->SetMesh(m_blockMeshes[2]);
				block->SetPosition(Vector3(ix * blockSize, offsetY - iy * blockSize, floorZoffset + 5.0f));
				block->SetScale(5.0f);

				block->GetMeshComponent()->SetIntensityVal(5.0f);

			}
		}
	}
	buildingData.clear();

	// マップに登録(依頼人)
	int count = 0;
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{

			if (clientData[iy][ix] == 33)
			{
				ClientActor* client = new ClientActor(Vector3(ix * blockSize, offsetY - iy * blockSize, -10.0f), count);
				count++;
				in_world->AddClientActor(client);

				// ロード処理
				GAME_INSTANCE.GetLoadScreen()->AddGauge();
			}
		}
	}
	clientData.clear();


	// マップに登録 (ヘリの巡回ポイント)
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			if (patrolPointData[iy][ix] >= 34)
			{
				PatrolPoint* patrol = new PatrolPoint(Vector3(ix * blockSize, offsetY - iy * blockSize, 1200.0f));

				in_world->AddPatrolPoint(patrol);

				// ロード処理
				GAME_INSTANCE.GetLoadScreen()->AddGauge();

			}
		}
	}
	patrolPointData.clear();


	// マップに登録 (ポイントライト)
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			if (pointLightData[iy][ix] == 32)
			{
				//PointLight* pLight = new PointLight();
				//pLight->SetPosition(Vector3(ix* blockSize, offsetY - iy * blockSize, 0.0f));

				LampObject* lamp = new LampObject(Vector3(ix * blockSize, offsetY - iy * blockSize, -10.0f), m_blockMeshes[3]);

				// ロード処理
				if (ix % 4 == 0)
				{
					GAME_INSTANCE.GetLoadScreen()->AddGauge();
				}

			}

		}
	}
	pointLightData.clear();

	//BridgeObject* bridge = new BridgeObject(1, Vector3::Zero);

	m_blockMeshes.clear();
	m_objectMeshes.clear();
}


// デストラクタ
LevelManager::~LevelManager()
{

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
