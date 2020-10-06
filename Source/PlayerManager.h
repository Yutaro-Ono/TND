//-----------------------------------------------------------------------+
// プレイヤーマネージャークラス
// 機能：主にプレイヤーの人間モード・車両運転モードを制御する
// 特性：アクター継承
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"

class PlayerManager : public Actor
{
public:

	enum PLAYER_MODE
	{
		MODE_HUMAN,
		MODE_CAR
	};


	PlayerManager();
	~PlayerManager();

	void UpdateActor(float in_deltaTime) override;


	//----------------------------------------------------+
	// Getter/Setter
	//----------------------------------------------------+
	// プレイヤー(人間)の座標セッター
	void SetPlayerHumanPos(const Vector3& in_pos) { m_playerHuman->SetPosition(in_pos); }
	// プレイヤー(車)の座標セッター
	void SetPlayerCarPos(const Vector3& in_pos) { m_playerCar->SetPosition(in_pos); }

	// モードの取得
	PLAYER_MODE GetPlayerMode() { return m_playerMode; }

private:

	PLAYER_MODE m_playerMode;

	// プレイヤーの人間・車クラス
	class PlayerHuman* m_playerHuman;
	class PlayerCar* m_playerCar;




};