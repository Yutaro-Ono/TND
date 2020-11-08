//-----------------------------------------------------------------------+
//
// Canvasクラス (UIクラスがワールドクラスを参照できるようにする)
//
//-----------------------------------------------------------------------+
#pragma once
#include <vector>

class Canvas
{
public:


	Canvas(class GameWorld* in_world);     // コンストラクタ
	~Canvas();    // デストラクタ

	void Update(float in_deltaTime);

	void AddUI(class UIScreen* in_ui) { m_interfaces.push_back(in_ui); }

	class GameWorld* GetWorld() { return m_world; }

	class PlayerControlUI* GetPlayerControlUI() { return m_pControlUI; }

	class ScoreUI* GetScoreUI() { return m_scoreUI; }

private:


	class GameWorld* m_world;      // ゲームワールド

	class PlayerControlUI* m_pControlUI;

	class ScoreUI* m_scoreUI;                       // スコアUI

	std::vector<class UIScreen*> m_interfaces;      // インターフェース

};