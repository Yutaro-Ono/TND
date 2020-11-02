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


private:


	class GameWorld* m_world;      // ゲームワールド

	std::vector<class UIScreen*> m_interfaces;      // インターフェース

};