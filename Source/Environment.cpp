#include "Environment.h"
#include "GameMain.h"
#include "SkyBox.h"
#include "CubeMapComponent.h"

Environment::Environment(GAME_TIME in_gameTime)
	:m_gameTime(in_gameTime)
{
	// スカイボックス生成
	m_morningBox = new SkyBox(this, GAME_TIME::MORNING);
	m_eveningBox = new SkyBox(this, GAME_TIME::EVENING);
	m_nightBox = new SkyBox(this, GAME_TIME::NIGHT);
	// 環境光セット
	SetDirectionalLight(in_gameTime);
}

Environment::~Environment()
{
}

void Environment::Update()
{


}

// 時間帯ごとに平行ライトを設定する
void Environment::SetDirectionalLight(GAME_TIME in_gameTime)
{
	m_gameTime = in_gameTime;

	if (m_gameTime == GAME_TIME::MORNING)
	{
		// ライティング
		RENDERER->SetAmbientLight(Vector3(0.8f, 0.8f, 0.9f));
		DirectionalLight& dir = RENDERER->GetDirectionalLight();
		dir.position = Vector3(30000.0f, 15400.0f, 15000.0f);
		dir.direction = Vector3(-200.0f, 500.0f, -600.0f);
		dir.direction.Normalize();
		dir.ambient = Vector3(0.4f, 0.4f, 0.4f);
		dir.diffuse = Vector3(1.0f, 1.0f, 1.0f);
		dir.specular = Vector3(0.8f, 0.8f, 0.8f);

		m_morningBox->GetCubeMapComp()->SetIsVisible(true);

		return;
	}

	if (m_gameTime == GAME_TIME::AFTERNOON)
	{
		// ライティング
		RENDERER->SetAmbientLight(Vector3(0.1f, 0.1f, 0.15f));
		DirectionalLight& dir = RENDERER->GetDirectionalLight();
		dir.position = Vector3(1800.0f, 2400.0f, 1000.0f);
		dir.direction = Vector3(0.0f, 0.5f, -1.0f);
		dir.direction.Normalize();
		dir.ambient = Vector3(0.5f, 0.5f, 0.5f);
		dir.diffuse = Vector3(0.07f, 0.05f, 0.3f);
		dir.specular = Vector3(0.5f, 0.5f, 0.6f);

		m_morningBox->GetCubeMapComp()->SetIsVisible(true);

		return;
	}

	if (m_gameTime == GAME_TIME::EVENING)
	{
		// ライティング
		RENDERER->SetAmbientLight(Vector3(0.1f, 0.1f, 0.15f));
		DirectionalLight& dir = RENDERER->GetDirectionalLight();
		dir.position = Vector3(1800.0f, 2400.0f, 1000.0f);
		dir.direction = Vector3(0.0f, 0.5f, -1.0f);
		dir.direction.Normalize();
		dir.ambient = Vector3(0.4f, 0.4f, 0.4f);
		dir.diffuse = Vector3(0.07f, 0.05f, 0.3f);
		dir.specular = Vector3(0.01f, 0.0f, 0.1f);

		m_eveningBox->GetCubeMapComp()->SetIsVisible(true);

		return;

	}

	if (m_gameTime == GAME_TIME::NIGHT)
	{
		// ライティング
		RENDERER->SetAmbientLight(Vector3(0.1f, 0.1f, 0.15f));
		DirectionalLight& dir = RENDERER->GetDirectionalLight();
		dir.position = Vector3(1800.0f, 2400.0f, 1000.0f);
		dir.direction = Vector3(0.0f, 0.5f, -1.0f);
		dir.direction.Normalize();
		dir.ambient = Vector3(0.2f, 0.2f, 0.2f);
		dir.diffuse = Vector3(0.07f, 0.05f, 0.3f);
		dir.specular = Vector3(0.01f, 0.0f, 0.1f);

		m_nightBox->GetCubeMapComp()->SetIsVisible(true);

		return;
	}

}
