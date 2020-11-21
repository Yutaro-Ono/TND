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
	,m_windowTex(nullptr)
	,m_generalWindow(nullptr)
	,m_pointTex(nullptr)
	,m_anim(0.0f)
{

	// �����_�����O����t�H���g�̃J���[
	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	// �t�H���g�T�C�Y
	int fontSize = 32;
	// �������ԁE�X�R�A�𕶎���Ƃ��Ď擾
	std::stringstream timestream;
	std::stringstream scoreStream;
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

		timestream << m_mission->GetTimeLimit();
		// �������Ԃ̃t�H���g�e�N�X�`���𐶐�
		m_timeTex[i] = m_font->RenderText(timestream.str(), color * i, fontSize);

		// �X�R�A�p�t�H���g�e�N�X�`������
		scoreStream << "$" << "0";
		m_scoreTex[i] = m_font->RenderText(scoreStream.str(), color * i, fontSize);

		m_distanceTex[i] = nullptr;

		if (m_mission->GetListNum() == 0)
		{
			m_missionID[i] = m_font->RenderText("A", color * i, fontSize);
		}
		else if (m_mission->GetListNum() == 1)
		{
			m_missionID[i] = m_font->RenderText("B", color * i, fontSize);
		}
		else
		{
			m_missionID[i] = m_font->RenderText("C", color * i, fontSize);
		}
	}



	// �w�i�E�B���h�E�e�N�X�`��
	m_windowTex = RENDERER->GetTexture("Data/Interface/TND/Control/Mission_Index.png");
	// �z�B�˗��e�N�X�`�� (���X�gA�̂�)
	m_generalWindow = RENDERER->GetTexture("Data/Interface/TND/Control/Mission_General.png");
	// �I���|�C���g�e�N�X�`��
	m_pointTex = RENDERER->GetTexture("Data/Interface/TND/Control/Mission_CursolDot.png");
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
	std::stringstream scoreStream;


	// �����_�����O����t�H���g�̃J���[
	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	// �t�H���g�T�C�Y
	int fontSize = 32;

	// �������Ԃ𕶎���Ƃ��Ď擾
	timestream << m_mission->GetTimeLimit();
	// �ϋv�l�𕶎���Ƃ��Ď擾
	durablestream << "HP:" << m_mission->GetDurableValue();
	// �����𕶎���Ƃ��Ď擾
	diststream << (int)m_mission->GetPlayerDistance() / 20 << "m";
	// �X�R�A�𕶎���Ƃ��Ď擾
	int score = m_mission->GetBaseScore();
	Vector3 scoreColor = Vector3(1.0f, 1.0f, 1.0f);
	if (score >= 2000)
	{
		scoreColor = Vector3(0.5f, 0.6f, 1.0f);
	}
	scoreStream << "$" << score;

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

		//--------------------------------------------------+
		// �X�R�A
		//--------------------------------------------------+
		// �X�R�A�p�t�H���g�e�N�X�`������
		if (m_scoreTex[i] != nullptr) m_scoreTex[i]->Delete();
		m_scoreTex[i] = m_font->RenderText(scoreStream.str(), scoreColor * i, fontSize);

		//--------------------------------------------------+
        // �~�b�V����ID
        //--------------------------------------------------+
		if (m_missionID != nullptr) m_missionID[i]->Delete();
		if (m_mission->GetListNum() == 0)
		{
			m_missionID[i] = m_font->RenderText("A", color * i, fontSize);
		}
		else if (m_mission->GetListNum() == 1)
		{
			m_missionID[i] = m_font->RenderText("B", color * i, fontSize);
		}
		else
		{
			m_missionID[i] = m_font->RenderText("C", color * i, fontSize);
		}
	}

}

// �`�揈��
void MissionUI::Draw(Shader* in_shader)
{
	glEnable(GL_BLEND);
	// �摜�X�P�[��
	float scale = 0.5f;
	// �I�����̃X�N���[�����W����
	Vector2 adjustPos = m_uiPos + Vector2(0.0f, -m_windowTex->GetHalfHeight() / 3 * (m_mission->GetListNum() + 1)) * scale;

	
	// �I�𒆂̃~�b�V������������UI�E�B���h�E����������
	// �������邽�߂Ƀ��[�v������p���č��W���X�N���[�������ֈړ����� (�I�𒆂���Ȃ��ꍇ�����[�v�����Ŗ߂�)
	if (m_mission->GetMissionManager()->GetSelectedMission() == m_mission->GetListNum())
	{
		m_anim += 10.0f * GAME_INSTANCE.GetDeltaTime();

		if (m_anim >= 1.0f)
		{
			m_anim = 1.0f;
		}

		adjustPos = Vector2::Lerp(adjustPos, m_uiPos + Vector2(-m_windowTex->GetHalfWidth(), -m_windowTex->GetHalfHeight() / 3 * (m_mission->GetListNum() + 1)) * scale, 1.0f * m_anim);
	}
	else
	{
		if (m_anim > 0.0f)
		{
			m_anim -= 10.0f * GAME_INSTANCE.GetDeltaTime();
		}
		else
		{
			m_anim = 0.0f;
		}

		adjustPos = Vector2::Lerp(adjustPos, m_uiPos + Vector2(-m_windowTex->GetHalfWidth(), -m_windowTex->GetHalfHeight() / 3 * (m_mission->GetListNum() + 1)) * scale, 1.0f * m_anim);
	}
	

	// �w�i�E�B���h�E�`��
	if (m_windowTex != nullptr)
	{
		DrawTexture(in_shader, m_windowTex, adjustPos, scale);
	}


	for (int i = 0; i < 2; i++)
	{
		// �~�b�V�����T�v�e�N�X�`���̕`��
		//DrawTexture(in_shader, m_detailTex[i], m_uiPos + Vector2(-3.0f * i, -(30 * m_mission->GetListNum()) + 5.0f * i), scale);

		// �������Ԃ̕`��
		if (m_timeTex[i] != nullptr)
		{
			DrawTexture(in_shader, m_timeTex[i], adjustPos + Vector2(m_windowTex->GetWidth() / 3 + m_timeTex[i]->GetHalfWidth(), -5.0f * i) * scale, 1.0f);
		}

		// �ϋv�l�̕`��
		if (m_durableValTex[i] != nullptr)
		{

			// �ϋv�x�̕`��
			//DrawTexture(in_shader, m_durableValTex[i], m_uiPos + Vector2(500.0f - 3.0f * i, scale);
		}


		// �����̕`��
		if (m_distanceTex[i] != nullptr)
		{
			DrawTexture(in_shader, m_distanceTex[i], adjustPos - Vector2(m_windowTex->GetWidth() / 2 + m_distanceTex[i]->GetWidth(), m_distanceTex[i]->GetWidth() / 3 + -5.0f * i) * scale, 1.0f);
		}

		// �X�R�A�̕`��
		if (m_scoreTex[i] != nullptr)
		{
			DrawTexture(in_shader, m_scoreTex[i], adjustPos - Vector2(m_windowTex->GetHalfWidth() / 2, -5.0f * i) * scale, 1.0f);
		}

		// �~�b�V����ID�`��
		DrawTexture(in_shader, m_missionID[i], adjustPos + Vector2(-m_windowTex->GetHalfWidth() + m_missionID[i]->GetHalfWidth() * 4, -5.0f * i) * scale, 1.0f);
	}

	// ���̃~�b�V�������I������Ă��鎞�̂݁A�I���|�C���g��`��
	if (m_mission->GetMissionManager()->GetSelectedMission() == m_mission->GetListNum())
	{
		DrawTexture(in_shader, m_pointTex, adjustPos + Vector2(-m_windowTex->GetHalfWidth() * 1.1, m_pointTex->GetHeight() * 0.04f) * scale, 0.04f);
	}

	// �u�z�B�˗��v
	if (m_mission->GetListNum() == 1)
	{
		DrawTexture(in_shader, m_generalWindow, m_uiPos + Vector2(0.0f, -m_windowTex->GetHalfHeight() / 3 * (m_mission->GetListNum() + 1)) * scale + Vector2(-m_generalWindow->GetHalfWidth(), m_generalWindow->GetHeight() * 2.0 * 0.275f) * 0.3f, 0.3f);
	}
	

}


// UI�\�����W�̃Z�b�g
void MissionUI::SetUIPosition()
{
	// UI�\�����W (��ʂ̒��S : x = 0.0, ��ʏ�� : y = 0)
	Vector2 screenUIPos = Vector2(GAME_CONFIG->GetScreenWidth() / 2, -GAME_CONFIG->GetScreenHeight() / 20);

	// UI�\���ʒu���X�N���[���̕��E��������ݒ�
	m_uiPos = screenUIPos;
}
