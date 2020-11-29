#include "PlayerManager.h"
#include "Input.h"
#include "InputController.h"
#include "ThirdPersonCamera.h"
#include "ThirdPersonCarCamera.h"
#include "PhysicsWorld.h"
#include "ClientActor.h"

PlayerManager::PlayerManager()
	:m_playerMode(PLAYER_MODE::MODE_HUMAN)
	,m_searchSphere(Vector3::Zero, 150.0f)
	,m_isDetectedCar(false)
	,m_isDetectedClient(false)
{
	m_playerHuman = new PlayerHuman(this);
	m_playerHuman->SetPosition(Vector3(1800.0f, 2400.0f, 0.0f));
	m_playerHuman->SetScale(0.265f);

	m_playerCar = new PlayerCar();
	m_playerCar->SetPlayerManager(this);
	m_playerCar->SetPosition(Vector3(1700.0f, 2400.0f, -15.0f));
	m_playerCar->SetScale(0.3f);

	m_playerHuman->SetActive(true);
}

PlayerManager::~PlayerManager()
{
}

// 更新処理
void PlayerManager::UpdateActor(float in_deltaTime)
{

	// 人間プレイヤーの当たり判定更新
	m_searchSphere.m_center = m_playerHuman->GetPosition();

    // プレイヤーと車の乗車・降車処理
	SearchCar();

	// 毎フレーム依頼人発見フラグはオフにする(true時の上書きを防ぐため)
	m_isDetectedClient = false;

	if (m_playerMode == PLAYER_MODE::MODE_HUMAN)
	{
		m_playerHuman->SetState(STATE_ACTIVE);
		m_playerCar->SetActive(false);
	}

	if (m_playerMode == PLAYER_MODE::MODE_CAR)
	{
		m_playerCar->SetActive(true);
		m_playerHuman->SetPosition(m_playerCar->GetPosition());
	}

	// プレイヤー(人間)の座標を自身の座標とする
	m_position = m_playerHuman->GetPosition();
}



// プレイヤーのアクセス範囲内に車があるかの検索
// 車の乗車・降車処理
void PlayerManager::SearchCar()
{

	// プレイヤーのアクセス範囲に車が入っていたらフラグを立てる
	if (m_searchSphere.Contains(m_playerCar->GetPosition()) && m_playerMode == MODE_HUMAN)
	{
		m_isDetectedCar = true;
	}
	else
	{
		m_isDetectedCar = false;
	}

	// アクセス範囲内に車が入っていたら
	// 人間が車に近づいてYボタンを押すと乗る
	// 乗車中だったら降車
	if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_Y) || INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_E))
	{
		if (m_isDetectedCar)
		{
			m_playerCar->OnChange();
			SetPlayerMode(MODE_CAR);
		}
		// 車から降りる条件は一定以下の速度であること
		else if (m_playerMode == MODE_CAR && m_playerCar->GetAccelValue() <= 35.0f)
		{
			m_playerHuman->GetCamera()->SetCameraForward(m_playerCar->GetCamera()->GetForward());
			m_playerHuman->SetPosition(m_playerHuman->GetPosition() + Vector3(40.0f, 40.0f, 0.0f));
			SetPlayerMode(MODE_HUMAN);
		}
	}

}

// プレイヤーのアクセス範囲内に依頼人がいるかを検索
void PlayerManager::SearchClient(ClientActor* in_client)
{
	// そもそもミッション対象の依頼人か・依頼人をすでに発見していたら無視
	if (in_client->GetClientSetting() == ClientActor::NONE || m_isDetectedClient == true)
	{
		return;
	}

	in_client->SetDistancePlayer(Vector3::Distance(m_position, in_client->GetPosition()));   // 距離をセット

	// 渡された依頼人の座標がプレイヤーのアクセス範囲内に入っているか
	if (m_searchSphere.Contains(in_client->GetPosition()))
	{
		in_client->SetAcceptedPlayer();
		m_isDetectedClient = in_client->GetIsAccepted();
	}
	else
	{
		in_client->SetIsAccepted(false);
	}
	
}
