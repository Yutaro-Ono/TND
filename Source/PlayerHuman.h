#pragma once
#include "Actor.h"

class PlayerHuman : public Actor
{

public:

	// プレイヤーの視点モード
	enum PLAYER_POV
	{
		FIRST_PERSON,
		THIRD_PERSON
	};

	PlayerHuman();           // コンストラクタ
	~PlayerHuman();          // デストラクタ



	//-------------------------------------------+
	// Getter/Setter
	//-------------------------------------------+
	// POVモードのセッター
	void SetPlayerPov(PLAYER_POV in_pov) { m_pov = in_pov; }


private:

	PLAYER_POV m_pov;       // 視点モード

	class HumanCamera* m_camera;

};