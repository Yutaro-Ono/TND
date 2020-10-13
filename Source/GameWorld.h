#pragma once
#include <vector>
//---------------------------------------------------------+
//
// ゲームワールド統括クラス
//
//---------------------------------------------------------+

class GameWorld
{

public:

	GameWorld();             // コンストラクタ
	~GameWorld();            // デストラクタ



private:

	std::vector<class Actor*> m_clients;          // 依頼者アクター


};