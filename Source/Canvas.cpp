#include "Canvas.h"
#include "UIScreen.h"

// コンストラクタ
Canvas::Canvas(GameWorld* in_world)
	:m_world(in_world)
{

}

// デストラクタ
Canvas::~Canvas()
{
	// 全てのUIを終了
	for (auto ui : m_interfaces)
	{
		ui->Close();
	}
	m_interfaces.clear();
}

void Canvas::Update(float in_deltaTime)
{

}
