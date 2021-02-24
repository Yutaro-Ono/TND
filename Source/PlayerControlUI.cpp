#include "PlayerControlUI.h"
#include "PlayerManager.h"
#include "Font.h"
#include "Texture.h"
#include "GameMain.h"
#include "Renderer.h"
#include "WorldSpaceUI.h"
#include "GameWorld.h"
#include "ClientActor.h"

const Vector3 ADJUST_RIDETEX_POS(-20.0f, 0.0f, 70.0f);     // 乗車UIの位置調整用
const Vector2 ADJUST_ACCESSTEX_POS(30.0f, 0.0f);    // 受注UIの位置調整用

// コンストラクタ
PlayerControlUI::PlayerControlUI(GameWorld* in_world)
	:m_world(in_world)
	,m_rideTexture(nullptr)
	,m_chasing(nullptr)
	,m_accessTex(nullptr)
	,m_completeTex(nullptr)
	,m_controlPanel(nullptr)
	,m_land1(nullptr)
	,m_land2(nullptr)
	,m_selectMission(nullptr)
	,m_accessTexPos(Vector2::Zero)
	,m_findPlayer(false)
{
	m_player = m_world->GetPlayer();

	// 乗車テクスチャの生成
	m_rideTexture = new WorldSpaceUI(m_player->GetPlayerCarPos() + ADJUST_RIDETEX_POS, "Data/Interface/TND/Control/Control_ride.png", 35.0f);

	// ミッション受注UI
	m_accessTex = RENDERER->GetTexture("Data/Interface/TND/Control/Control_AccessMission.png");
	// ミッション完了UI
	m_completeTex = RENDERER->GetTexture("Data/Interface/TND/Control/Control_MissionComplete2.png");
	// 操作説明用パネル
	m_controlPanel = RENDERER->GetTexture("Data/Interface/TND/Control/Control_Panel2.png");

	// "DANGER"表示用のテクスチャ生成
	m_dangerFontTex = RENDERER->GetTexture("Data/Interface/TND/Control/Chasing_Helicopter.png");

	// "追跡中"表示
	m_chasing = RENDERER->GetTexture("Data/Interface/TND/Control/Chasing_Helicopter.png");

	m_land1 = RENDERER->GetTexture("Data/Interface/TND/Control/TASK_UI2.png");
	m_land2 = RENDERER->GetTexture("Data/Interface/TND/Control/TASK_UI3.png");
	m_selectMission = RENDERER->GetTexture("Data/Interface/TND/Control/Control_Order.png");

}

// デストラクタ
PlayerControlUI::~PlayerControlUI()
{
	//delete m_rideTexture;
}

// 更新処理
void PlayerControlUI::Update(float in_deltaTime)
{
	// 乗車UIの座標更新・表示フラグ切り替え
	m_rideTexture->SetPosition(m_player->GetPlayerCarPos() + ADJUST_RIDETEX_POS);
	m_rideTexture->SetVisible(m_player->GetIsDetectedCar());
}

// 描画処理
void PlayerControlUI::Draw(Shader* in_shader)
{
	float scale = 0.3f;

	if (m_player->GetIsDetectedClient())
	{
		DrawTexture(in_shader, m_accessTex, m_accessTexPos + ADJUST_ACCESSTEX_POS, scale);
	}

	if (m_findPlayer)
	{
		DrawTexture(in_shader, m_dangerFontTex, Vector2(0.0f, GAME_CONFIG->GetScreenHeight() / 2.5), 0.8);
	}

	// 車乗車時に操作パネルを表示する
	if (m_player->GetPlayerMode() == PlayerManager::MODE_CAR)
	{
		DrawTexture(in_shader, m_controlPanel, Vector2(-GAME_CONFIG->GetScreenWidth() / 2 + m_controlPanel->GetWidth(), 0.0f), 0.6f);
	}


	// 選択中のミッションが受注されているかどうかで、目標表示のテクスチャを切り替える
	if (m_state == UIScreen::ACTIVE)
	{
		bool task = false;
		for (auto clients : m_world->GetClients())
		{
			if (clients->GetIsSelected())
			{
				if (clients->GetIsAccepted())
				{
					task = true;
				}
			}
		}

		// 目標表示用のテクスチャ
		if (task)
		{
			DrawTexture(in_shader, m_land2, Vector2(0.0f, GAME_CONFIG->GetScreenHeight() / 3.0f), 0.6f);
		}
		else
		{
			DrawTexture(in_shader, m_land1, Vector2(0.0f, GAME_CONFIG->GetScreenHeight() / 3.0f), 0.6f);
		}
	}


	
	// ミッション選択のチュートリアル
	DrawTexture(in_shader, m_selectMission, Vector2(GAME_CONFIG->GetScreenWidth() / 2.0f - m_selectMission->GetHalfWidth() * 1.62f, -m_selectMission->GetHalfHeight() * 0.2f), 0.35f);

}
