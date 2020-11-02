#include "MissionBase.h"
#include "ClientActor.h"
#include "MissionUI.h"
#include "MissionManager.h"
#include "PlayerManager.h"
#include "GameMain.h"
#include "GameConfig.h"

// コンストラクタ
MissionBase::MissionBase(MissionManager* in_manager, MISSION_TYPE in_type, int in_listNum)
	:m_manager(in_manager)
	,m_startActor(nullptr)
	,m_goalActor(nullptr)
	,m_missionType(in_type)
	,m_missionState(HOLD)
	,m_durableVal(100)
	, m_listNum(in_listNum)
{
	// 初期化
	m_startPos = m_goalPos = Vector3::Zero;
	m_currentTime = m_lastTime = SDL_GetTicks();

	// UI生成
	m_missionUI = new MissionUI(this);
	m_missionUI->SetUIPosition();
}

// デストラクタ
MissionBase::~MissionBase()
{
	m_missionUI->Close();
}

// 更新処理
void MissionBase::Update(float in_deltaTime)
{

	// プレイヤー座標を取得しておく
	Vector3 playerPos = m_manager->GetPlayer()->GetPosition();
	

	bool isSelected = false;
	// ミッションがカーソルで選択されている時
	if (m_listNum == m_manager->GetSelectedMission())
	{
		m_startActor->SetIsSelected(true);
		isSelected = true;
	}
	else
	{
		m_startActor->SetIsSelected(false);
		m_goalActor->SetIsSelected(false);
	}

	//---------------------------------------------------+
	//
	// ステートごとの更新処理
	//
	//---------------------------------------------------+
	// ミッション保留中
	if (m_missionState == HOLD)
	{

		// 距離更新
		CheckDistPlayer(playerPos, m_startPos);

		// UI上のカーソルでこの任務が選択されている場合、ランドマークを表示
		if (isSelected)
		{
			m_startActor->SetIsSelected(true);
		}


		// 最終時刻を更新し続ける
		m_lastTime = SDL_GetTicks();


		// プレイヤーのアクセス範囲球に依頼人が入った状態で、ボタンを押したらミッション開始
		if (m_startActor->GetIsAccepted())
		{
			// 承諾ボタンを押したらtrue
			if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_X) || INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_E))
			{
				m_startActor->SetClientSetting(ClientActor::CLIENT_SETTING::NONE);
				m_startActor->SetIsSelected(false);
				// 任務を受諾したとしてカウントダウン開始
				m_missionState = ACTIVE;
			}
		}



	}

	// ミッション受注時
	if (m_missionState == ACTIVE)
	{

		// 距離更新
		CheckDistPlayer(playerPos, m_goalPos);

		// UI上のカーソルでこの任務が選択されている場合、ランドマークを表示
		if (isSelected)
		{
			m_goalActor->SetIsSelected(true);
		}

		// 現在時刻を更新
		m_currentTime = SDL_GetTicks();

		//-----------------------------------------+
		// カウントダウン処理
		//-----------------------------------------+
		// 一秒経過後
		if (m_currentTime > m_lastTime + 1000)
		{
			// 制限時間を1減らす
			m_timeLimit -= 1;
			// 最終時刻更新
			m_lastTime = m_currentTime;
		}

		// 制限時間が切れたらミッション失敗
		if (m_timeLimit <= 0)
		{
			// 失敗状態にする
			m_missionState = FAILED;
		}

		// 耐久値が0になったらミッション失敗
		if (m_durableVal <= 0)
		{
			m_missionState = FAILED;
		}

		// プレイヤーのアクセス範囲球に依頼人が入った状態で、ボタンを押したらミッション開始
		if (m_goalActor->GetIsAccepted())
		{
			// 承諾ボタンを押したらtrue
			if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_X) || INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_E))
			{
				m_goalActor->SetClientSetting(ClientActor::CLIENT_SETTING::NONE);
				m_goalActor->SetIsSelected(false);
				// 任務を受諾したとしてカウントダウン開始
				m_missionState = SUCCESS;
			}
		}


	}

}


// ミッション詳細設定の定義
void MissionBase::SetMissionDetail(ClientActor* in_start, ClientActor* in_goal, unsigned int in_baseScore, unsigned int in_timeLimit)
{
	m_startActor = in_start;
	m_goalActor = in_goal;

	m_startPos = m_startActor->GetPosition();          // 開始地点
	m_goalPos = m_goalActor->GetPosition();            // ゴール地点

	m_baseScore = in_baseScore;     // ベーススコア
	m_timeLimit = in_timeLimit;     // 制限時間

	// 依頼人にthisポインタを渡す
	in_start->SetMission(this);
	in_goal->SetMission(this);
}

// プレイヤーの座標～開始地点or終了地点座標の距離を求め、一定以上接近してボタンを押したら真)
void MissionBase::CheckDistPlayer(const Vector3& in_playerPos, const Vector3& in_missionPos)
{
	// プレイヤーとの距離を測定する
	m_playerDistance = Vector3::Distance(in_playerPos, in_missionPos);
}

// 耐久値減少処理
void MissionBase::DecraseDurableValue()
{
	m_durableVal -= 3;
}


// スコアを成功か失敗かによって計算し、更新したスコア値を返す
int MissionBase::GetCalcScore()
{
	if (m_missionState == MISSION_STATE::FAILED)
	{
		return 0;
	}

	return m_baseScore;
}

