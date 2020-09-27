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


private:

	PLAYER_MODE m_playerMode;


};