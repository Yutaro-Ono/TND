#include "MissionManager.h"
#include "GameWorld.h"
#include "MissionBase.h"
#include "MissionUI.h"
#include "ClientActor.h"
#include "Input.h"
#include "InputController.h"
#include <stdio.h>
#include <time.h>

const int MissionManager::MISSION_ALL_NUM = 3;    // 同時進行する任務の限界数

// コンストラクタ
MissionManager::MissionManager(GameWorld* in_world)
	:m_player(in_world->GetPlayer())
	,m_world(in_world)
	,m_selectNum(0)
{
	InitRandom();
	// ミッションリストの生成
	for (int i = 0; i < MISSION_ALL_NUM; i++)
	{
		// ミッション種類の乱数
		int missionType = rand() % 2 + 1;
		// 依頼人番号の乱数
		int startPos = rand() % m_world->GetClients().size();
		int goalPos = rand() % m_world->GetClients().size();

		

		// ミッションタイプごとにミッション内容を設定
		if (missionType == 1)
		{
			// 配達ミッションの追加
			m_missions.push_back(new MissionBase(this, MissionBase::DELIVERY, i));
			// ミッション概要セット
			m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos]->GetPosition(),
				m_world->GetClients()[goalPos]->GetPosition(), 1000, 30);
		}
		else
		{
			// 護送ミッションの追加
			m_missions.push_back(new MissionBase(this, MissionBase::TAXI, i));
			// ミッション概要セット
			m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos]->GetPosition(),
				m_world->GetClients()[goalPos]->GetPosition(), 1000, 30);
		}
	}
}

// デストラクタ
MissionManager::~MissionManager()
{
	while (!m_missions.empty())
	{
		delete m_missions.back();
	}
}

void MissionManager::Update(float in_deltaTime)
{
	
	// リスト番号更新用
	int listNum = 0;
	// 終了したミッションをミッションリストから削除する
	std::vector<MissionBase*> endMissions;
	for (auto mission = m_missions.begin(); mission != m_missions.end();)
	{
		(*mission)->SetListNum(listNum);
		(*mission)->Update(in_deltaTime);
		listNum++;

		// ミッション失敗時または成功時に削除用配列へ
		if ((*mission)->GetMissionState() == MissionBase::FAILED || (*mission)->GetMissionState() == MissionBase::SUCCESS)
		{
			endMissions.emplace_back(*mission);
			mission = m_missions.erase(mission);
		}
		else
		{
			++mission;
		}
	}
	// 削除処理
	for (auto end : endMissions)
	{
		delete end;
	}
	endMissions.clear();



	// ミッションリストは常時3つ。減っていたら作成する
	int addMissionNum = MISSION_ALL_NUM - m_missions.size();
	if (addMissionNum > 0)
	{

		for (int i = 0; i < addMissionNum; i++)
		{
			InitRandom();
			// ミッション種類の乱数
			int missionType = rand() % 2 + 1;
			// 依頼人番号の乱数
			int startPos = rand() % m_world->GetClients().size();
			int goalPos = rand() % m_world->GetClients().size();

			// ミッションタイプごとにミッション内容を設定
			if (missionType == 1)
			{
				// 配達ミッションの追加
				m_missions.push_back(new MissionBase(this, MissionBase::DELIVERY, i));
				// ミッション概要セット
				m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos]->GetPosition(),
					m_world->GetClients()[goalPos]->GetPosition(), 1000, 30);
			}
			else
			{
				// 護送ミッションの追加
				m_missions.push_back(new MissionBase(this, MissionBase::TAXI, i));
				// ミッション概要セット
				m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos]->GetPosition(),
					m_world->GetClients()[goalPos]->GetPosition(), 1000, 30);
			}
		}
	}


	// ミッションカーソル
	ChangeSelectNum();

	
}

void MissionManager::InitRandom()
{
	srand((unsigned int)time(NULL));
}

void MissionManager::ChangeSelectNum()
{
	// カーソル下移動
	if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
	{
		m_selectNum++;
		// ミッション数を超えたら0番目に折り返し
		if (m_selectNum >= MISSION_ALL_NUM)
		{
			m_selectNum = 0;
		}
	}
	
	// カーソル上移動
	if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_DPAD_UP))
	{
		m_selectNum--;
		// 1だったら折り返し
		if (m_selectNum < 0)
		{
			m_selectNum = 2;
		}
	}

	for (auto mission : m_missions)
	{
		
	}
}
