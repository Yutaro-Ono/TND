#include "MissionUI.h"
#include "MissionBase.h"
#include "MissionManager.h"
#include "Font.h"
#include "Texture.h"
#include <iostream>
#include <sstream>

// �R���X�g���N�^
MissionUI::MissionUI(MissionBase* in_mission)
	:m_mission(in_mission)
	,m_uiPos(Vector2::Zero)
{

	// �����_�����O����t�H���g�̃J���[
	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	// �t�H���g�T�C�Y
	int fontSize = 32;
	// �������Ԃ𕶎���Ƃ��Ď擾
	std::stringstream timestream;
	//------------------------------------------------------------------+
	// �~�b�V�����T�v�t�H���g�̐���
	//------------------------------------------------------------------+

	for (int i = 0; i < 2; i++)
	{
		if (m_mission->GetMissionType() == MissionBase::DELIVERY)
		{
			// �~�b�V�����T�v�e�N�X�`���̐���
			m_detailTex[i] = m_font->RenderText("DELIVERY", color * i, fontSize);
		}

		if (m_mission->GetMissionType() == MissionBase::TAXI)
		{
			// �~�b�V�����T�v�e�N�X�`���̐���
			m_detailTex[i] = m_font->RenderText("TAXI", color * i, fontSize);
		}

		// �ϋv�l�̃t�H���g�e�N�X�`���𐶐�
		m_durableValTex[i] = m_font->RenderText("100%", color * i, fontSize);


		timestream << "TIME:" << m_mission->GetTimeLimit();
		// �������Ԃ̃t�H���g�e�N�X�`���𐶐�
		m_timeTex[i] = m_font->RenderText(timestream.str(), color * i, fontSize);

		m_distanceTex[i] = nullptr;
	}

}

// �f�X�g���N�^
MissionUI::~MissionUI()
{
	Close();
}

// �X�V����
void MissionUI::Update(float in_deltaTime)
{


	// �~�b�V�����I����UI�����
	if (m_mission->GetMissionState() == MissionBase::MISSION_STATE::FAILED ||
		m_mission->GetMissionState() == MissionBase::MISSION_STATE::SUCCESS)
	{
		m_state = CLOSE;
	}

	// �~�b�V�������X�V
	UpdateMissionInfo();

}

// �~�b�V�������̐����E�X�V����
void MissionUI::UpdateMissionInfo()
{
	// �ϋv�l�A���ԂȂǂ̎擾�p
	std::string str;
	std::stringstream timestream;
	std::stringstream durablestream;
	std::stringstream diststream;

	// �����_�����O����t�H���g�̃J���[
	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	// �t�H���g�T�C�Y
	int fontSize = 32;

	// �������Ԃ𕶎���Ƃ��Ď擾
	timestream << "TIME:" << m_mission->GetTimeLimit();
	// �ϋv�l�𕶎���Ƃ��Ď擾
	durablestream << "HP:" << m_mission->GetDurableValue();
	// �����𕶎���Ƃ��Ď擾
	diststream << (double)m_mission->GetPlayerDistance() / 20.0f << "m";

	//-----------------------------------------------------------------------+
    // �t�H���g�e�N�X�`����������
    //-----------------------------------------------------------------------+
	for (int i = 0; i < 2; i++)
	{
		//-------------------------------------------------+
		// ��������
		//-------------------------------------------------+
		// �������Ԃ̃e�N�X�`�����폜
		if (m_timeTex[i] != nullptr) m_timeTex[i]->Delete();
		// �������Ԃ̃t�H���g�e�N�X�`���𐶐�
		m_timeTex[i] = m_font->RenderText(timestream.str(), color * i, fontSize);

		//-------------------------------------------------+
		// �ϋv�l
		//-------------------------------------------------+
		// �ϋv�l�̃e�N�X�`�����폜
		if (m_durableValTex[i] != nullptr) m_durableValTex[i]->Delete();
		// �ϋv�l�̃t�H���g�e�N�X�`���𐶐�
		m_durableValTex[i] = m_font->RenderText(durablestream.str(), color * i, fontSize);

		//--------------------------------------------------+
		// ����
		//--------------------------------------------------+
		// �����̃e�N�X�`�����폜
		if (m_distanceTex[i] != nullptr) m_distanceTex[i]->Delete();
		// �����̃t�H���g�e�N�X�`���𐶐�
		m_distanceTex[i] = m_font->RenderText(diststream.str(), color * i, fontSize);
	}

}

// �`�揈��
void MissionUI::Draw(Shader* in_shader)
{
	
	// �摜�X�P�[��
	float scale = 0.5f;
	// �I�𒆂̃~�b�V������������X�P�[���g��
	if (m_mission->GetMissionManager()->GetSelectedMission() == m_mission->GetListNum())
	{
		scale = 1.0f;
	}

	for (int i = 0; i < 2; i++)
	{
		// �~�b�V�����T�v�e�N�X�`���̕`��
		DrawTexture(in_shader, m_detailTex[i], m_uiPos + Vector2(0.0f - 3.0f * i, -(30 * m_mission->GetListNum()) + 5.0f * i), scale);

		// �������Ԃ̕`��
		if (m_timeTex[i] != nullptr)
		{
			DrawTexture(in_shader, m_timeTex[i], m_uiPos + Vector2(300.0f - 3.0f * i, -(30 * m_mission->GetListNum()) + 5.0f * i), scale);
		}

		// �ϋv�l�̕`��
		if (m_durableValTex[i] != nullptr)
		{

			// �ϋv�x�̕`��
			DrawTexture(in_shader, m_durableValTex[i], m_uiPos + Vector2(500.0f - 3.0f * i, -(30 * m_mission->GetListNum()) + 5.0f * i), scale);
		}


		// �����̕`��
		if (m_distanceTex[i] != nullptr)
		{
			// �����̕`��
			DrawTexture(in_shader, m_distanceTex[i], m_uiPos + Vector2(900.0f - 3.0f * i, -(30 * m_mission->GetListNum()) + 5.0f * i), scale);
		}
	}




}


// UI�\�����W�̃Z�b�g
void MissionUI::SetUIPosition()
{
	// UI�\�����W (��ʂ̒��S : x = 0.0, ��ʏ�� : y = ��ʂ̍�����5
	Vector2 screenUIPos = Vector2(-500.0f, (GAME_CONFIG->GetScreenHeight() / 3));

	// UI�\���ʒu���X�N���[���̕��E��������ݒ�
	m_uiPos = screenUIPos;
}
