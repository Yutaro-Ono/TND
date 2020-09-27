//-----------------------------------------------------------------------+
// ともし火の管理クラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "FlameManager.h"
#include "FlameActor.h"
#include "Player.h"
#include "RapidJsonHelper.h"
#include <vector>
#include <string>
#include <sstream>
#include "stdio.h"

const int FlameManager::MAP_ALL_NUM = 4;                   // 全てのともし火配置マップ
const float FlameManager::FLAME_SIZE = 120.0f;             // ともし火の基本サイズ


// コンストラクタ
FlameManager::FlameManager(class Player* in_target, int in_stageNum)
	:m_target(in_target)
	,m_getRank(0)
	,m_rank(0)
	,m_toNext(false)
{
	// ステージ配置ファイルのパスを生成
	std::stringstream ssMap;
	ssMap << "Data/Strings/map_" << in_stageNum << ".json";
	std::string mapPath;
	mapPath = ssMap.str();


	// ともし火の配置データ分、スタックを確保
	while (m_mapData.size() != MAP_ALL_NUM)
	{
		m_mapData.emplace_back();
	}

	//-------------------------------------------------------------------+
	// ステージデータ読み込み (各4パターン)
	//-------------------------------------------------------------------+
	for (int i = 0; i < MAP_ALL_NUM; i++)
	{
		if (i == 0)
		{
			if (!ReadTiledJson(m_mapData[i], mapPath.c_str(), "layerFlame_A"))
			{
				printf("<Flame> Map Load : Failed\n");
				GAME_INSTANCE.SetShutDown();
				return;
			}
		}
		if (i == 1)
		{
			if (!ReadTiledJson(m_mapData[i], mapPath.c_str(), "layerFlame_B"))
			{
				printf("<Flame> Map Load : Failed\n");
				GAME_INSTANCE.SetShutDown();
				return;
			}
		}
		if (i == 2)
		{
			if (!ReadTiledJson(m_mapData[i], mapPath.c_str(), "layerFlame_C"))
			{
				printf("<Flame> Map Load : Failed\n");
				GAME_INSTANCE.SetShutDown();
				return;
			}
		}
		if (i == 3)
		{
			if (!ReadTiledJson(m_mapData[i], mapPath.c_str(), "layerFlame_D"))
			{
				printf("<Flame> Map Load : Failed\n");
				GAME_INSTANCE.SetShutDown();
				return;
			}
		}

	}

	// 設置する際の間隔用サイズ
	const float setSize = 200.0f;
	int sizeX, sizeY;

	sizeX = m_mapData[0][0].size();
	sizeY = m_mapData[0].size();

	float offsetY = sizeY * setSize;

	// ともし火の配置 (パターンA)
	for (int layer = 0; layer < 1; layer++)
	{
		for (int iy = 0; iy < sizeY; iy++)
		{
			for (int ix = 0; ix < sizeX; ix++)
			{
				if (m_mapData[layer][iy][ix] == 0)
				{
					continue;
				}

				if (m_mapData[layer][iy][ix] >= 5)
				{
					m_flameActor.push_back(new FlameActor);
					m_flameActor.back()->SetFlamePosition(Vector3(ix * setSize, offsetY - iy * setSize, 50.0f));
					m_flameActor.back()->SetSphereCollider(m_flameActor.back()->GetPosition(), FLAME_SIZE);
				}
			}
		}
	}
}

// デストラクタ
FlameManager::~FlameManager()
{
	Delete();
}

// マップのロード
bool FlameManager::LoadMap(const char* in_mapData)
{
	return false;
}

// 解放処理
void FlameManager::Delete()
{
	// 全てのともし火を削除
	for (auto iter : m_flameActor)
	{
		iter->SetState(Actor::STATE_DEAD);
	}

}

// 更新処理
void FlameManager::UpdateActor(float in_deltaTime)
{
	// 当たり判定と生存しているかどうかをチェック
	CollisionCheck();
	CheckAlive();
}

// ともし火が生存しているかどうかのチェック
void FlameManager::CheckAlive()
{

	// ともし火スタックのサイズが0(全てのともし火が消えていたなら)、次のパターンを配置する
	if (m_flameActor.size() == 0)
	{
		// ランクアップ
		m_rank++;

		// 全てのステージを回り切ったら最初(パターンA)に戻る
		if (m_rank == MAP_ALL_NUM)
		{
			m_rank = 0;
		}

		m_toNext = true;
	}

	// 全て死んでいたら、次のパターンを配置
	if (m_toNext == true)
	{
		const float setSize = 200.0f;
		int sizeX, sizeY;

		sizeX = m_mapData[0][0].size();
		sizeY = m_mapData[0].size();

		float offsetY = sizeY * setSize;

		// 生きていなければ次のともし火を配置
		for (int iy = 0; iy < sizeY; iy++)
		{
			for (int ix = 0; ix < sizeX; ix++)
			{
				if (m_mapData[m_rank][iy][ix] == 0)
				{
					continue;
				}

				if (m_mapData[m_rank][iy][ix] >= 5)
				{
					m_flameActor.push_back(new FlameActor);
					m_flameActor.back()->SetFlamePosition(Vector3(ix * setSize, offsetY - iy * setSize, 50.0f));
					m_flameActor.back()->SetSphereCollider(m_flameActor.back()->GetPosition(), FLAME_SIZE);
				}
			}
		}

		m_toNext = false;
	}
}

// 当たり判定チェック
void FlameManager::CollisionCheck()
{
	// ともし火全体の当たり判定チェック
	for (int i = 0; i < m_flameActor.size(); i++)
	{

		m_flameActor[i]->CollisionCheck(m_target);

		// 死んでいたら消す
		if (m_flameActor[i]->GetState() == Actor::STATE_DEAD)
		{
			m_getRank++;
			m_flameActor.erase(m_flameActor.begin() + i);
		}
	}
}

// ともし火のリセット(フリーモード→ゲームモード移行時の処理)
void FlameManager::ResetFlame()
{
	for (auto iter : m_flameActor)
	{
		iter->SetState(Actor::STATE_DEAD);
	}

	for (int i = 0; i < m_flameActor.size(); i++)
	{

		// 死んでいたら消す
		if (m_flameActor[i]->GetState() == Actor::STATE_DEAD)
		{
			m_flameActor.erase(m_flameActor.begin() + i);
		}
	}

	const float setSize = 200.0f;
	int sizeX, sizeY;

	sizeX = m_mapData[0][0].size();
	sizeY = m_mapData[0].size();

	float offsetY = sizeY * setSize;

	// 生きていなければ次のともし火を配置
	for (int iy = 0; iy < sizeY; iy++)
	{
		for (int ix = 0; ix < sizeX; ix++)
		{
			if (m_mapData[0][iy][ix] == 0)
			{
				continue;
			}

			if (m_mapData[0][iy][ix] >= 5)
			{
				m_flameActor.push_back(new FlameActor);
				m_flameActor.back()->SetFlamePosition(Vector3(ix * setSize, offsetY - iy * setSize, 50.0f));
				m_flameActor.back()->SetSphereCollider(m_flameActor.back()->GetPosition(), FLAME_SIZE);
			}
		}
	}
}


// Jsonの読み込み
bool FlameManager::ReadTiledJson(std::vector<std::vector<int>>& in_mapData, const char * in_fileName, const char * in_layerName)
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
	in_mapData.resize(height);
	for (auto &mapIter : in_mapData)
	{
		mapIter.resize(width);
	}

	// ユーザー配列（2次元）にデータをコピーする
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			in_mapData[y][x] = rapidArrayData[y * width + x].GetInt();
		}
	}

	return true;
}


int FlameManager::FindLayerIndex(rapidjson::GenericArray<false, rapidjson::Value>& in_layer, std::string & in_layerName)
{
	int  layerSize = in_layer.Size();

	int i;
	std::string layerNameString(in_layerName);
	for (i = 0; i < layerSize; i++)
	{
		std::string currentLayerName = in_layer[i]["name"].GetString();
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
