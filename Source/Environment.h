#pragma once
#include "Renderer.h"

class Environment
{
public:

	// ゲームの時間帯 (これに応じてスカイボックスが変化)
	enum GAME_TIME
	{
		MORNING = 0,
		AFTERNOON = 1,
		EVENING = 2,
		NIGHT = 3
	};

	Environment(GAME_TIME in_gameTime, const Vector3& in_pos);     // コンストラクタ1
	Environment(class GameWorld* in_world, GAME_TIME in_gameTime);     // コンストラクタ2
	~Environment();                         // デストラクタ

	void Update();

	void SetSkyBox(GAME_TIME in_gameTime);
	void SetDirectionalLight(GAME_TIME in_gameTime, const Vector3& in_pos);             // 時間帯ごとに平行ライトを設定する

	void SetDirectionalLightPos(const Vector3& in_pos);

	// ゲームタイムのゲッター・セッター
	GAME_TIME GetGameTime() { return m_gameTime; }
	void SetGameTime(GAME_TIME in_gameTime) { m_gameTime = in_gameTime; }


private:


	GAME_TIME m_gameTime;

	class GameWorld* m_world;

	DirectionalLight* m_dirLight;

	// 各種スカイボックス
	class SkyBox* m_morningBox;
	class SkyBox* m_eveningBox;
	class SkyBox* m_nightBox;
};