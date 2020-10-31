//---------------------------------------------------------------------------+
// LevelManagerクラス.  マップのブロック配置、オブジェクト配置統合クラス
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//---------------------------------------------------------------------------+
#pragma once
#include "GameMain.h"
#include <vector>
#include <string>
#include "rapidjson/document.h"


class LevelManager
{


public:

	LevelManager(class GameWorld* in_world, int in_stageNum);                               // コンストラクタ
	~LevelManager();                                             // デストラクタ

	bool LoadLevel(const char* in_json, int in_layerNum);

	void SetCamera(class FollowCamera* in_camera) { m_camera = in_camera; }
	void SetViewRange(float in_width, float in_height);
	void Update();


private:

	bool ReadTiledJson(std::vector<std::vector<int>>& mapData, const char* in_fileName, const char* in_layerName);
	int FindLayerIndex(rapidjson::GenericArray<false, rapidjson::Value> &layer, std::string &layerName);

	Vector3 m_leftUpPos, m_rightDownPos;
	Vector3 m_nowRangeMin, m_nowRangeMax;
	
	class FollowCamera* m_camera;

	std::vector<class Mesh*> m_blockMeshes;          // ブロックメッシュ
	std::vector<class Mesh*> m_objectMeshes;         // オブジェクトメッシュ




	friend class PhysicsWorld;

	int m_mapNumX;
	int m_mapNumY;

	float m_blockSize;


	// Tiledエディタ上におけるオブジェクトの番号
	static const int MAP_INDEX_GROUND;
	static const int MAP_INDEX_BUILDING;
};