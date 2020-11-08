#include "PlayerControlUI.h"
#include "PlayerManager.h"
#include "Font.h"
#include "Texture.h"
#include "GameMain.h"
#include "Renderer.h"
#include "WorldSpaceUI.h"
#include "GameWorld.h"

const Vector3 ADJUST_RIDETEX_POS(-20.0f, 0.0f, 70.0f);     // 乗車UIの位置調整用
const Vector2 ADJUST_ACCESSTEX_POS(30.0f, 0.0f);    // 受注UIの位置調整用

// コンストラクタ
PlayerControlUI::PlayerControlUI(GameWorld* in_world)
	:m_world(in_world)
	,m_rideTexture(nullptr)
	,m_chasing(nullptr)
	,m_accessTexPos(Vector2::Zero)
	,m_findPlayer(false)
{
	m_player = m_world->GetPlayer();

	// 乗車テクスチャの生成
	m_rideTexture = new WorldSpaceUI(m_player->GetPlayerCarPos() + ADJUST_RIDETEX_POS, "Data/Interface/TND/Control/Control_ride.png", 35.0f);

	// ミッション受注UI
	m_accessTex = RENDERER->GetTexture("Data/Interface/TND/Control/Control_AccessMission.png");

	// "DANGER"表示用のテクスチャ生成
	m_dangerFontTex = m_font->RenderText("DANGER", Vector3(0.0, 0.0, 1.0), 48);

	// "追跡中"表示
	m_chasing = RENDERER->GetTexture("Data/Interface/TND/Control/Chasing_Helicopter.png");
}

// デストラクタ
PlayerControlUI::~PlayerControlUI()
{
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
		DrawTexture(in_shader, m_dangerFontTex, Vector2(0.0f, GAME_CONFIG->GetScreenHeight() / 2.5), 1.0);
	}
}
