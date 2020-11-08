#include "Canvas.h"
#include "UIScreen.h"
#include "GameWorld.h"
#include "PlayerManager.h"
#include "PlayerControlUI.h"
#include "ScoreUI.h"

// コンストラクタ
Canvas::Canvas(GameWorld* in_world)
	:m_world(in_world)
	,m_pControlUI(nullptr)
{
	// 操作説明用UIの生成
	m_pControlUI = new PlayerControlUI(m_world);

	// スコアUIを生成
	m_scoreUI = new ScoreUI(this);
}

// デストラクタ
Canvas::~Canvas()
{
	// 全てのUIを終了
	for (auto ui : m_interfaces)
	{
		ui->Close();
	}
	m_interfaces.clear();
}

void Canvas::Update(float in_deltaTime)
{

}
