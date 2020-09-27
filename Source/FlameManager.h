//-----------------------------------------------------------------------+
// ともし火の管理クラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

#include "GameMain.h"
#include "Actor.h"
#include <vector>
#include <string>
#include "rapidjson/document.h"


class FlameManager : public Actor
{


public:

	FlameManager(class Player* in_target, int in_stageNum);
	~FlameManager();

	bool LoadMap(const char* in_json);

	void Delete();

	void UpdateActor(float in_deltaTime) override;

	void CheckAlive();

	void CollisionCheck();

	void ResetFlame();               // ともし火配置のリセット


private:

	bool ReadTiledJson(std::vector<std::vector<int>>& in_mapData, const char* in_fileName, const char* in_layerName);
	int FindLayerIndex(rapidjson::GenericArray<false, rapidjson::Value>& in_layer, std::string& in_layerName);
	
	std::vector<std::vector<std::vector<int>>> m_mapData;          // ともし火配置データ格納用

	std::vector<class FlameActor*> m_flameActor;                   // ともし火アクター配列

	class Player* m_target;

	int m_getRank;                                                 // プレイヤーはいくつともし火を取得したか
	int m_rank;                                                    // ランク


	bool m_toNext;                                                 // ともし火再配置フラグ

	static const int MAP_ALL_NUM;
	static const float FLAME_SIZE;


};