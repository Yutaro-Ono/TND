//-----------------------------------------------------------------------+
// プレイヤーマネージャークラス
// 機能：主にプレイヤーの人間モード・車両運転モードを制御する
// 特性：アクター継承
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"
#include "PlayerHuman.h"
#include "PlayerCar.h"
#include "Collision.h"

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

	void SearchCar();                                    // プレイヤーのアクセス範囲内に車があるかの検索・車の乗車・降車処理
	void SearchClient(class ClientActor* in_client);     // プレイヤーのアクセス範囲内に依頼人がいるかを検索

	//----------------------------------------------------+
	// Getter/Setter
	//----------------------------------------------------+
	// プレイヤー(人間)の座標セッター
	void SetPlayerHumanPos(const Vector3& in_pos) { m_playerHuman->SetPosition(in_pos); }
	// プレイヤー(車)の座標セッター
	void SetPlayerCarPos(const Vector3& in_pos) { m_playerCar->SetPosition(in_pos); }
	const Vector3& GetPlayerCarPos() { return m_playerCar->GetPosition(); }

	// モードのゲッター・セッター
	PLAYER_MODE GetPlayerMode() { return m_playerMode; }
	void SetPlayerMode(PLAYER_MODE in_mode) { m_playerMode = in_mode; }

	// 索敵範囲のセッター・ゲッター
	Sphere GetSearchSphere() { return m_searchSphere; }
	bool GetIsDetectedCar() { return m_isDetectedCar; }           // プレイヤーのアクセス範囲内に車を検出したかのゲッター
	bool GetIsDetectedClient() { return m_isDetectedClient; }     // プレイヤーのアクセス範囲内に依頼人を検出したかのゲッター

	// プレイヤーポインタのゲッター
	class PlayerHuman* GetPlayerHuman() { return m_playerHuman; }
	// 車クラスへのポインタゲッター
	class PlayerCar* GetPlayerCar() { return m_playerCar; }

private:

	PLAYER_MODE m_playerMode;

	// プレイヤーの人間・車クラス
	class PlayerHuman* m_playerHuman;
	class PlayerCar* m_playerCar;

	bool m_isDetectedClient;     // プレイヤーのアクセス範囲内に依頼人がいるか
	bool m_isDetectedCar;        // プレイヤーのアクセス範囲内に車があるか

	Sphere m_searchSphere;       // プレイヤーのアクセス範囲球体

};