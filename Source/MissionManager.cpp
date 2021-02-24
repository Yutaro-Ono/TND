#include "MissionManager.h"
#include "GameWorld.h"
#include "Canvas.h"
#include "MissionBase.h"
#include "MissionUI.h"
#include "ClientActor.h"
#include "Input.h"
#include "InputController.h"
#include "ScoreUI.h"
#include "PlayerManager.h"
#include "AudioManager.h"
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

	// ミッション開始地点と終了地点
	// 乱数で生成し、開始・終了点で被りがないかつ他のミッションと被らないこと
	int startPos[MISSION_ALL_NUM], goalPos[MISSION_ALL_NUM];
	
	// 依頼人アクタ配列をシャッフル
	m_world->ShuffleClientActor();

	// ミッションリストの生成
	for (int i = 0; i < MISSION_ALL_NUM; i++)
	{
		// ミッション種類の乱数
		int missionType = rand() % 2 + 1;
		
		startPos[i] = -1;
		// 依頼人アクタの中から選択状態でない依頼人を探し、スタート地点アクタの番号を取得する
		for (int j = 0; j < m_world->GetClients().size(); j++)
		{

			if (m_world->GetClients()[j]->GetClientSetting() == ClientActor::NONE)
			{
				startPos[i] = j;

				m_world->GetClients()[j]->SetClientSetting(ClientActor::START);
			}
			// スタート地点が変更されていたらループを抜ける
			if (startPos[i] >= 0)
			{
				break;
			}
		}

		goalPos[i] = -1;
		// 依頼人アクタの中から選択状態でない依頼人を探し、ゴール地点アクタの番号を取得する
		for (int j = 0; j < m_world->GetClients().size(); j++)
		{

			if (m_world->GetClients()[j]->GetClientSetting() == ClientActor::NONE)
			{
				goalPos[i] = j;

				m_world->GetClients()[j]->SetClientSetting(ClientActor::GOAL);
			}
			// ゴール地点が変更されていたらループを抜ける
			if (goalPos[i] >= 0)
			{
				break;
			}
		}

		// プレイヤーの現在地・スタート地点とゴール地点の距離からスコアを算出
		float score = CalcScoreForDistance(m_player->GetPosition(),
			                               m_world->GetClients()[startPos[i]]->GetPosition(), 
			                               m_world->GetClients()[goalPos[i]]->GetPosition());

		// ミッションタイプごとにミッション内容を設定
		if (missionType == 1)
		{
			// 配達ミッションの追加
			m_missions.push_back(new MissionBase(this, MissionBase::DELIVERY, i));
			// ミッション概要セット
			m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos[i]],
				m_world->GetClients()[goalPos[i]], score, 60);
		}
		else
		{
			// 護送ミッションの追加
			m_missions.push_back(new MissionBase(this, MissionBase::TAXI, i));
			// ミッション概要セット
			m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos[i]],
				m_world->GetClients()[goalPos[i]], score, 50);
		}

		// 依頼人アクタ配列をシャッフル
		m_world->ShuffleClientActor();
	}


	// 効果音のロード
	m_selectSE = "Data/Music/SE/TND/System/Select/decide14.wav";
	AUDIO->GetSound(m_selectSE);

}

// デストラクタ
MissionManager::~MissionManager()
{
	m_missions.clear();
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
			m_world->GetCanvas()->GetScoreUI()->SetAddScore((*mission)->GetCalcScore());
			// 依頼人を非セレクト状態に
			auto itr = std::find(m_world->GetClients().begin(), m_world->GetClients().end(), (*mission)->GetStartActor());
			(*itr)->SetIsSelected(false);
			(*itr)->SetClientSetting(ClientActor::CLIENT_SETTING::NONE);
			itr = std::find(m_world->GetClients().begin(), m_world->GetClients().end(), (*mission)->GetGoalActor());
			(*itr)->SetIsSelected(false);
			(*itr)->SetClientSetting(ClientActor::CLIENT_SETTING::NONE);

			// ミッションシャッフル
			m_world->ShuffleClientActor();

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


	// ミッション開始地点と終了地点
    // 乱数で生成し、開始・終了点で被りがないかつ他のミッションと被らないこと
	int startPos, goalPos;
	startPos = goalPos = 0;
	// ミッションリストは常時3つ。減っていたら作成する
	int addMissionNum = MISSION_ALL_NUM - m_missions.size();
	if (addMissionNum > 0)
	{

		for (int i = 0; i < addMissionNum; i++)
		{
			InitRandom();
			// ミッション種類の乱数
			int missionType = rand() % 2 + 1;
			// 依頼人の座標を新規のミッションゴール座標としてセット
			for (int j = m_world->GetClients().size() - 1; j >= 0; j--)
			{
				// 選択されていなかったらスタート地点として設定
				if (m_world->GetClients()[j]->GetClientSetting() == ClientActor::NONE)
				{
					startPos = j;
					break;
				}
			}

			// 依頼人の座標を新規のミッションスタート座標としてセット
			for (int j = 0; j < m_world->GetClients().size(); j++)
			{
				// 選択されていなかったらスタート地点として設定
				if (m_world->GetClients()[j]->GetClientSetting() == ClientActor::NONE)
				{
					goalPos = j;
					break;
				}
			}

			// プレイヤーの現在地・スタート地点とゴール地点の距離からスコアを算出
			float score = CalcScoreForDistance(m_player->GetPosition(), m_world->GetClients()[startPos]->GetPosition(),
				                               m_world->GetClients()[goalPos]->GetPosition());

			// ミッションタイプごとにミッション内容を設定
			if (missionType == 1)
			{
				// 配達ミッションの追加
				m_missions.push_back(new MissionBase(this, MissionBase::DELIVERY, i));
				// ミッション概要セット
				m_missions.back()->SetMissionDetail(m_world->GetClients()[startPos],
					m_world->GetClients()[goalPos], score, 60);
				// クライアントをセレクト状態に
				m_world->GetClients()[startPos]->SetClientSetting(ClientActor::START);
				m_world->GetClients()[goalPos]->SetClientSetting(ClientActor::GOAL);

			}
			else
			{
				// 護送ミッションの追加
				m_missions.push_back(new MissionBase(this, MissionBase::TAXI, i));
				// ミッション概要セット
				m_missions.back()->SetMissionDetail(m_world->GetClients()[startPos],
					m_world->GetClients()[goalPos], score, 50);
				// クライアントをセレクト状態に
				m_world->GetClients()[startPos]->SetClientSetting(ClientActor::START);
				m_world->GetClients()[goalPos]->SetClientSetting(ClientActor::GOAL);

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

		// 選択番号プラス
		m_selectNum++;

		// 効果音再生
		AUDIO->PlaySoundTND(m_selectSE);

		// ミッション数を超えたら0番目に折り返し
		if (m_selectNum >= MISSION_ALL_NUM)
		{
			m_selectNum = 0;
		}
	}
	
	// カーソル上移動
	if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_DPAD_UP))
	{

		// 選択番号マイナス
		m_selectNum--;

		// 効果音再生
		AUDIO->PlaySoundTND(m_selectSE);

		// 1だったら折り返し
		if (m_selectNum < 0)
		{
			m_selectNum = 2;
		}
	}

}

// 距離に応じたスコア計算
float MissionManager::CalcScoreForDistance(const Vector3& in_pPos, const Vector3& in_cPos1, const Vector3& in_cPos2)
{
	// ベーススコアポイント
	float baseScore = 100.0f;
	float score = 0.0f;

	// 1.プレイヤーとスタート地点クライアントの距離 + スタート地点とゴール地点の距離の合計からボーナススコアを算出
	score = (Vector3::Distance(in_pPos, in_cPos1) / 20) + (Vector3::Distance(in_cPos1, in_cPos2) / 20);

	return baseScore + score;
}
