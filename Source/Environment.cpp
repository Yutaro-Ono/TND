#include "Environment.h"
#include "GameMain.h"
#include "SkyBox.h"
#include "CubeMapComponent.h"
#include "GameWorld.h"
#include "PlayerManager.h"
#include "RenderBloom.h"

static Vector3 playerPos = Vector3::Zero;
//static const Vector3 ADJUST_POS = Vector3(2000.0f, 0.0f, 3000.0f);
static const Vector3 ADJUST_POS = Vector3(-600.0f, 50.0f, 700.0f);

Environment::Environment(GAME_TIME in_gameTime, const Vector3& in_pos)
	:m_gameTime(in_gameTime)
	, m_world(nullptr)
{
	// スカイボックス生成
	m_morningBox = new SkyBox(this, GAME_TIME::MORNING);
	m_eveningBox = new SkyBox(this, GAME_TIME::EVENING);
	m_nightBox = new SkyBox(this, GAME_TIME::NIGHT);
	// 設定された時間帯ごとにスカイボックスをセット
	SetSkyBox(m_gameTime);
	SetDirectionalLight(m_gameTime, in_pos);
}


Environment::Environment(GameWorld* in_world, GAME_TIME in_gameTime)
	:m_gameTime(in_gameTime)
	,m_world(in_world)
{
	// スカイボックス生成
	m_morningBox = new SkyBox(this, GAME_TIME::MORNING);
	m_eveningBox = new SkyBox(this, GAME_TIME::EVENING);
	m_nightBox = new SkyBox(this, GAME_TIME::NIGHT);

	playerPos = m_world->GetPlayer()->GetPosition();
	
	// 設定された時間帯ごとにスカイボックスをセット
	SetSkyBox(m_gameTime);

	// 環境光セット
	SetDirectionalLight(in_gameTime, m_world->GetPlayer()->GetPosition());

}

Environment::~Environment()
{

}

void Environment::Update()
{


	Vector3 framePlayerPos = m_world->GetPlayer()->GetPosition();

	// 影のちらつきを抑えるため4000以上の移動で平行ライトの位置をプレイヤーにスナップする
	if (framePlayerPos.x >= playerPos.x + 2000 || framePlayerPos.x <= playerPos.x - 1000 ||
		framePlayerPos.y >= playerPos.y + 2000 || framePlayerPos.y <= playerPos.y - 2000)
	{
		// プレイヤーの位置に応じて平行ライトの座標・向きを最適化
		RENDERER->GetDirectionalLight().position = Vector3((int)framePlayerPos.x, (int)framePlayerPos.y, 0.0f);
		RENDERER->GetDirectionalLight().position.x += (int)ADJUST_POS.x;
		RENDERER->GetDirectionalLight().position.y += (int)ADJUST_POS.y;
		RENDERER->GetDirectionalLight().position.z += (int)ADJUST_POS.z;

		RENDERER->GetDirectionalLight().target = framePlayerPos;

		RENDERER->GetDirectionalLight().direction = RENDERER->GetDirectionalLight().target - RENDERER->GetDirectionalLight().position;
		RENDERER->GetDirectionalLight().direction.Normalize();

		playerPos = framePlayerPos;
	}

}

void Environment::SetSkyBox(GAME_TIME in_gameTime)
{
	if (in_gameTime == GAME_TIME::MORNING || in_gameTime == GAME_TIME::AFTERNOON)
	{
		// レンダラーに日中のスカイボックスをセット
		RENDERER->SetActiveSkyBox(m_morningBox->GetCubeMapComp());
		m_morningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (in_gameTime == GAME_TIME::EVENING)
	{
		// レンダラーに夕方のスカイボックスをセット
		RENDERER->SetActiveSkyBox(m_eveningBox->GetCubeMapComp());
		m_eveningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (in_gameTime == GAME_TIME::NIGHT)
	{
		// レンダラーに夜のスカイボックスをセット
		RENDERER->SetActiveSkyBox(m_nightBox->GetCubeMapComp());
		m_nightBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

}

// 時間帯ごとに平行ライトを設定する
void Environment::SetDirectionalLight(GAME_TIME in_gameTime, const Vector3& in_pos)
{
	m_gameTime = in_gameTime;

	if (m_gameTime == GAME_TIME::MORNING)
	{
		// ライティング
		RENDERER->SetAmbientLight(Vector3(0.5f, 0.5f, 0.52f));
		dirLight& dir = RENDERER->GetDirectionalLight();
		//dir.position = Vector3(30000.0f, 15400.0f, 15000.0f);
		dir.position = playerPos + ADJUST_POS;
		dir.target = playerPos;
		dir.direction = dir.target - dir.position;
		dir.direction.Normalize();
		dir.ambient = Vector3(0.5f, 0.5f, 0.51f);
		dir.diffuse = Vector3(0.4f, 0.5f, 0.5f);
		dir.specular = Vector3(0.3f, 0.3f, 0.3f);

		RENDERER->GetBloom()->SetGamma(0.085f);
		RENDERER->GetBloom()->SetExposureVal(4.5f);

		m_morningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (m_gameTime == GAME_TIME::AFTERNOON)
	{
		// ライティング
		RENDERER->SetAmbientLight(Vector3(0.1f, 0.1f, 0.15f));
		dirLight& dir = RENDERER->GetDirectionalLight();
		dir.position = playerPos + ADJUST_POS;
		dir.target = playerPos;
		dir.direction = dir.target - dir.position;
		dir.direction.Normalize();
		dir.ambient = Vector3(0.7f, 0.7f, 0.75f);
		dir.diffuse = Vector3(0.4f, 0.4f, 0.45f);
		dir.specular = Vector3(0.5f, 0.5f, 0.6f);

		RENDERER->GetBloom()->SetGamma(1.0f);
		RENDERER->GetBloom()->SetExposureVal(4.5f);

		m_morningBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

	if (m_gameTime == GAME_TIME::EVENING)
	{
		// ライティング
		RENDERER->SetAmbientLight(Vector3(0.1f, 0.1f, 0.15f));
		dirLight& dir = RENDERER->GetDirectionalLight();
		dir.position = playerPos + ADJUST_POS;
		dir.target = playerPos;
		dir.direction = dir.target - dir.position;
		dir.direction.Normalize();
		dir.ambient = Vector3(0.4f, 0.4f, 0.4f);
		dir.diffuse = Vector3(0.07f, 0.05f, 0.3f);
		dir.specular = Vector3(0.01f, 0.0f, 0.1f);

		RENDERER->GetBloom()->SetGamma(1.0f);
		RENDERER->GetBloom()->SetExposureVal(4.5f);

		m_eveningBox->GetCubeMapComp()->SetIsVisible(true);
		return;

	}

	if (m_gameTime == GAME_TIME::NIGHT)
	{
		// ライティング
		RENDERER->SetAmbientLight(Vector3(0.15f, 0.15f, 0.2f));
		dirLight& dir = RENDERER->GetDirectionalLight();
		dir.position = playerPos + ADJUST_POS;
		dir.target = playerPos;
		dir.direction = dir.target - dir.position;
		dir.direction.Normalize();
		//dir.ambient = Vector3(0.2f, 0.2f, 0.2f);
		dir.ambient = Vector3(0.31f, 0.415f, 0.46f);
		dir.diffuse = Vector3(0.1f, 0.485f, 0.46f);
		//dir.diffuse = Vector3(0.1f, 0.25f, 0.37f);

		dir.specular = Vector3(0.1f, 0.35f, 0.4f);

		RENDERER->GetBloom()->SetGamma(0.085f);
		RENDERER->GetBloom()->SetExposureVal(5.5f);

		m_nightBox->GetCubeMapComp()->SetIsVisible(true);
		return;
	}

}

// ディレクショナルライトの光源と向きを再計算
void Environment::SetDirectionalLightPos(const Vector3& in_pos)
{
	// プレイヤーの位置に応じて平行ライトの座標・向きを最適化
	RENDERER->GetDirectionalLight().position = in_pos;
	RENDERER->GetDirectionalLight().position.x += (int)ADJUST_POS.x;
	RENDERER->GetDirectionalLight().position.y += (int)ADJUST_POS.y;
	RENDERER->GetDirectionalLight().position.z += (int)ADJUST_POS.z;

	RENDERER->GetDirectionalLight().target = in_pos;

	RENDERER->GetDirectionalLight().direction = RENDERER->GetDirectionalLight().target - RENDERER->GetDirectionalLight().position;
	RENDERER->GetDirectionalLight().direction.Normalize();
}
