#pragma once
#include "Actor.h"
#include <string>

class Helicopter : public Actor
{

public:

	Helicopter(const Vector3& in_pos);              // コンストラクタ
	~Helicopter();             // デストラクタ

	void UpdateActor(float in_deltaTime) override;

private:


	// 機体の部品クラス
	class HeliBody* m_body;                // ボディ
	class HeliRotorMain* m_mainRotor;      // メインローター
	class HeliRotorBack* m_backRotor;      // バックローター


};