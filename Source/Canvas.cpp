#include "Canvas.h"
#include "UIScreen.h"
#include "GameWorld.h"
#include "PlayerManager.h"
#include "PlayerControlUI.h"

// �R���X�g���N�^
Canvas::Canvas(GameWorld* in_world)
	:m_world(in_world)
	,m_pControlUI(nullptr)
{
	// ��������pUI�̐���
	m_pControlUI = new PlayerControlUI(m_world->GetPlayer());
}

// �f�X�g���N�^
Canvas::~Canvas()
{
	// �S�Ă�UI���I��
	for (auto ui : m_interfaces)
	{
		ui->Close();
	}
	m_interfaces.clear();
}

void Canvas::Update(float in_deltaTime)
{

}