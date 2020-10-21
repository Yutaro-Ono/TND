#include "MissionManager.h"
#include "GameWorld.h"
#include "MissionBase.h"
#include "MissionUI.h"
#include "ClientActor.h"
#include "Input.h"
#include "InputController.h"
#include "ScoreUI.h"
#include <stdio.h>
#include <time.h>

const int MissionManager::MISSION_ALL_NUM = 3;    // 同時進行する任務の限界数


// コンストラクタ
MissionManager::MissionManager(GameWorld* in_world)
	:m_player(in_world->GetPlayer())
	,m_world(in_world)
	,m_scoreUI(nullptr)
	,m_selectNum(0)
{
	InitRandom();

	// ミッション開始地点と終了地点
	// 乱数で生成し、開始・終了点で被りがないかつ他のミッションと被らないこと
	int startPos[MISSION_ALL_NUM], goalPos[MISSION_ALL_NUM];
	

	// ミッションリストの生成
	for (int i = 0; i < MISSION_ALL_NUM; i++)
	{
		// ミッション種類の乱数
		int missionType = rand() % 2 + 1;
		// 依頼人番号の乱数
		startPos[i] = rand() % m_world->GetClients().size();
		goalPos[i] = rand() % m_world->GetClients().size();

		// 座標の被りがないようにする
		do
		{
			startPos[i] = rand() % m_world->GetClients().size();
		} while (m_world->GetClients()[startPos[i]]->GetIsSelected());
		m_world->GetClients()[startPos[i]]->SetIsSelected(true);
		do
		{
			goalPos[i] = rand() % m_world->GetClients().size();

		} while (m_world->GetClients()[goalPos[i]]->GetIsSelected());
		m_world->GetClients()[goalPos[i]]->SetIsSelected(true);
		


		// ミッションタイプごとにミッション内容を設定
		if (missionType == 1)
		{
			// 配達ミッションの追加
			m_missions.push_back(new MissionBase(this, MissionBase::DELIVERY, i));
			// ミッション概要セット
			m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos[i]],
				m_world->GetClients()[goalPos[i]], 1000, 30);
		}
		else
		{
			// 護送ミッションの追加
			m_missions.push_back(new MissionBase(this, MissionBase::TAXI, i));
			// ミッション概要セット
			m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos[i]],
				m_world->GetClients()[goalPos[i]], 1000, 30);
		}
	}


	// スコアUIを生成
	m_scoreUI = new ScoreUI();
}

// デストラクタ
MissionManager::~MissionManager()
{
	while (!m_missions.empty())
	{
		delete m_missions.back();
	}
}

// 更新処理
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

		// ミッション失敗時または成功時
		// スコアを加算 (失敗時は0)
		// 削除用配列へ
		if ((*mission)->GetMissionState() == MissionBase::FAILED || (*mission)->GetMissionState() == MissionBase::SUCCESS)
		{
			// スコアUIにスコアをセット
			m_scoreUI->AddScore((*mission)->GetCalcScore());
			// 依頼人を非セレクト状態に
			auto itr = std::find(m_world->GetClients().begin(), m_world->GetClients().end(), (*mission)->GetStartActor());
			(*itr)->SetIsSelected(false);
			itr = std::find(m_world->GetClients().begin(), m_world->GetClients().end(), (*mission)->GetGoalActor());
			(*itr)->SetIsSelected(false);

			endMissions.emplace_back(*mission);
			mission = m_missions.erase(mission);
		}
		else
		{
			++mission;
		}

		listNum++;
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
				m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos],
					m_world->GetClients()[goalPos], 1000, 30);
			}
			else
			{
				// 護送ミッションの追加
				m_missions.push_back(new MissionBase(this, MissionBase::TAXI, i));
				// ミッション概要セット
				m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos],
					m_world->GetClients()[goalPos], 1000, 30);
			}
		}
	}

	// ミッションカーソルの更新
	ChangeSelectNum();
}

// 乱数の初期化
void MissionManager::InitRandom()
{
	srand((unsigned int)time(NULL));
}

// 何番目のミッションを選択しているかを切り替える
// 選択中のミッションはランドマークとしてUIに表示される
void MissionManager::ChangeSelectNum()
{
	// カーソル下移動
	if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
	{
		m_selectNum++;
		// ミッション数を超えたら0番目に折り返し
		if (m_selectNum >= MISSION_ALL_NUM)
		{
			m_selectNum = 0;
		}
	}
	
	// カーソル上移動
	if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_DPAD_UP))
	{
		m_selectNum--;
		// 1だったら折り返し
		if (m_selectNum < 0)
		{
			m_selectNum = 2;
		}
	}

}

bool MissionManager::GetNotWearPos(int* in_start, int* in_goal)
{
	// 座標の重複がないか調べる
	for (int i = 0; i < MISSION_ALL_NUM; i++)
	{
		// 
		for (int j = i + 1; j < MISSION_ALL_NUM; j++)
		{
			if (in_start[i] == in_start[j])
			{
				return false;
			}

			if (in_goal[i] == in_goal[j])
			{
				return false;
			}
		}
	}

	return true;
}
