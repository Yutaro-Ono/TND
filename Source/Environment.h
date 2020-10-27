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

	Environment(GAME_TIME in_gameTime);     // コンストラクタ
	~Environment();                         // デストラクタ

	void Update();

	void SetDirectionalLight(GAME_TIME in_gameTime);             // 時間帯ごとに平行ライトを設定する

	// ゲームタイムのゲッター・セッター
	GAME_TIME GetGameTime() { return m_gameTime; }
	void SetGameTime(GAME_TIME in_gameTime) { m_gameTime = in_gameTime; }


private:


	GAME_TIME m_gameTime;

	DirectionalLight m_dirLight;

	class SkyBox* m_morningBox;
	class SkyBox* m_eveningBox;
	class SkyBox* m_nightBox;
};