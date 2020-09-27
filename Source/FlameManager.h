//-----------------------------------------------------------------------+
// �Ƃ����΂̊Ǘ��N���X.
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

	void ResetFlame();               // �Ƃ����Δz�u�̃��Z�b�g


private:

	bool ReadTiledJson(std::vector<std::vector<int>>& in_mapData, const char* in_fileName, const char* in_layerName);
	int FindLayerIndex(rapidjson::GenericArray<false, rapidjson::Value>& in_layer, std::string& in_layerName);
	
	std::vector<std::vector<std::vector<int>>> m_mapData;          // �Ƃ����Δz�u�f�[�^�i�[�p

	std::vector<class FlameActor*> m_flameActor;                   // �Ƃ����΃A�N�^�[�z��

	class Player* m_target;

	int m_getRank;                                                 // �v���C���[�͂����Ƃ����΂��擾������
	int m_rank;                                                    // �����N


	bool m_toNext;                                                 // �Ƃ����΍Ĕz�u�t���O

	static const int MAP_ALL_NUM;
	static const float FLAME_SIZE;


};