//-----------------------------------------------------------------------+
// HUD�N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "HUD.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Font.h"
#include "Shader.h"
#include "Player.h"

#include <sstream>

// �R���X�g���N�^
HUD::HUD(class Player* in_player)
	:m_targetPlayer(in_player)
	,m_isImpact(false)
	,m_dispAccel(0.0f)
	,m_speedTexPos(600.0f, 340.0f)
{

	// �W����
	for (int i = 0; i < 2; i++)
	{
		std::stringstream ssImpact;
		ssImpact << "Data/Interface/HUD/Impact/Impact_" << (i + 1) << ".png";

		m_impactTexture[i] = RENDERER->GetTexture(ssImpact.str());
	}

}

// �f�X�g���N�^
HUD::~HUD()
{
	// �W�����X�^�b�N�̉��
	for (int i = 0; i < 2; i++)
	{
		m_impactTexture[i]->Delete();
	}

}

void HUD::Update(float in_deltaTime)
{

	// �v���C���[�̑��x��100km/h�ȏ�Ȃ�W������\��
	if (m_targetPlayer->GetAccel() >= 100.0f)
	{
		m_isImpact = true;
	}
	else
	{
		m_isImpact = false;
	}

}

void HUD::Draw(Shader * in_shader)
{

	// �W�����̕`��
	if (m_impactTexture && m_isImpact == true)
	{

		int pic = rand();

		if (pic % 2 == 0)
		{
			pic = 1;
		}
		else
		{
			pic = 0;
		}

		// �`��
		DrawTexture(in_shader, m_impactTexture[pic], Vector2::Zero, 1.0f);
	}

}
