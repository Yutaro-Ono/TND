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
	,m_timeTex(nullptr)
	,m_detailTex(nullptr)
	,m_distanceTex(nullptr)
	,m_uiPos(Vector2::Zero)
{

	// �����_�����O����t�H���g�̃J���[
	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	// �t�H���g�T�C�Y
	float fontSize = 32.0f;
	//------------------------------------------------------------------+
	// �~�b�V�����T�v�t�H���g�̐���
	//------------------------------------------------------------------+
	if (m_mission->GetMissionType() == MissionBase::DELIVERY)
	{
		// �~�b�V�����T�v�e�N�X�`���̐���
		m_detailTex = m_font->RenderText("DELIVERY", color, fontSize);
	}

	if (m_mission->GetMissionType() == MissionBase::TAXI)
	{
		// �~�b�V�����T�v�e�N�X�`���̐���
		m_detailTex = m_font->RenderText("TAXI", color, fontSize);
	}

	// �ϋv�l�̃t�H���g�e�N�X�`���𐶐�
	m_durableValTex = m_font->RenderText("100%", color, fontSize);
}

// �f�X�g���N�^
MissionUI::~MissionUI()
{
}

// �X�V����
void MissionUI::Update(float in_deltaTime)
{

	// �ϋv�l�A���ԂȂǂ̎擾�p
	std::string str;
	std::stringstream sstream;
	// �����_�����O����t�H���g�̃J���[
	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	// �t�H���g�T�C�Y
	float fontSize = 32.0f;

	// �~�b�V�����I����UI�����
	if (m_mission->GetMissionState() == MissionBase::MISSION_STATE::FAILED ||
		m_mission->GetMissionState() == MissionBase::MISSION_STATE::SUCCESS)
	{
		m_state = CLOSE;
	}

	//-----------------------------------------------------------------------+
	// �t�H���g�e�N�X�`����������
	//-----------------------------------------------------------------------+
	// �z�B�~�b�V������
	if (m_mission->GetMissionType() == MissionBase::DELIVERY)
	{
		
		//-------------------------------------------------+
		// ��������
		//-------------------------------------------------+
		// �������Ԃ𕶎���Ƃ��Ď擾
		sstream << "TIME:" << m_mission->GetTimeLimit();
		// �������Ԃ̃e�N�X�`�����폜
		if (m_timeTex != nullptr) m_timeTex->Delete();
		// �������Ԃ̃t�H���g�e�N�X�`���𐶐�
		m_timeTex = m_font->RenderText(sstream.str(), color, fontSize);
		sstream.str("");
		//-------------------------------------------------+
		// �ϋv�l
		//-------------------------------------------------+
		// �ϋv�l�𕶎���Ƃ��Ď擾
		sstream << m_mission->GetDurableValue() << "%";
		// �ϋv�l�̃e�N�X�`�����폜
		if (m_durableValTex != nullptr) m_durableValTex->Delete();
		// �ϋv�l�̃t�H���g�e�N�X�`���𐶐�
		m_durableValTex = m_font->RenderText(sstream.str(), color, fontSize);
		sstream.str("");

		//--------------------------------------------------+
		// ����
		//--------------------------------------------------+
		// �����𕶎���Ƃ��Ď擾
		sstream << m_mission->GetPlayerDistance() / 100.0f << "m";
		// �����̃e�N�X�`�����폜
		if (m_distanceTex != nullptr) m_distanceTex->Delete();
		// �����̃t�H���g�e�N�X�`���𐶐�
		m_distanceTex = m_font->RenderText(sstream.str(), color, fontSize);
		sstream.str("");
	}

	// �^�N�V�[�~�b�V������
	else if (m_mission->GetMissionType() == MissionBase::TAXI)
	{
		//--------------------------------------------------+
		// ��������
		//--------------------------------------------------+
		// �������Ԃ𕶎���Ƃ��Ď擾
		sstream << "TIME:" << m_mission->GetTimeLimit();
		// �������Ԃ̃e�N�X�`�����폜
		if (m_timeTex != nullptr) m_timeTex->Delete();
		// �������Ԃ̃t�H���g�e�N�X�`���𐶐�
		m_timeTex = m_font->RenderText(sstream.str(), color, fontSize);
		sstream.str("");
		
		//-------------------------------------------------+
        // �ϋv�l
        //-------------------------------------------------+
        // �ϋv�l�𕶎���Ƃ��Ď擾
		sstream << m_mission->GetDurableValue() << "%";
		// �ϋv�l�̃e�N�X�`�����폜
		if (m_durableValTex != nullptr) m_durableValTex->Delete();
		// �ϋv�l�̃t�H���g�e�N�X�`���𐶐�
		m_durableValTex = m_font->RenderText(sstream.str(), color, fontSize);
		sstream.str("");

		//--------------------------------------------------+
		// ����
		//--------------------------------------------------+
		// �����𕶎���Ƃ��Ď擾
		sstream << m_mission->GetPlayerDistance() / 100.0f << "m";
		// �����̃e�N�X�`�����폜
		if (m_distanceTex != nullptr) m_distanceTex->Delete();
		// �����̃t�H���g�e�N�X�`���𐶐�
		m_distanceTex = m_font->RenderText(sstream.str(), color, fontSize);
		sstream.str("");
	}

}

// �`�揈��
void MissionUI::Draw(Shader* in_shader)
{
	// �摜�X�P�[��
	float scale = 1.0f;

	// �~�b�V�����T�v�e�N�X�`���̕`��
	DrawTexture(in_shader, m_detailTex, m_uiPos, scale);

	// �������Ԃ̕`��
	if (m_timeTex != nullptr)
	{
		DrawTexture(in_shader, m_timeTex, m_uiPos + Vector2(200.0f, +(30 * m_mission->GetListNum())), scale);
	}

	// �ϋv�l�̕`��
	if (m_durableValTex != nullptr)
	{
	
		// �ϋv�x�̕`��
		DrawTexture(in_shader, m_durableValTex, m_uiPos + Vector2(500.0f, +(30 * m_mission->GetListNum())), scale);
	}


	// �����̕`��
	if (m_distanceTex != nullptr)
	{
		// �����̕`��
		DrawTexture(in_shader, m_distanceTex, m_uiPos + Vector2(900.0f, +(30 * m_mission->GetListNum())), scale);
	}


}


// UI�\�����W�̃Z�b�g
void MissionUI::SetUIPosition()
{
	// UI�\�����W (��ʂ̒��S : x = 0.0, ��ʏ�� : y = ��ʂ̍�����5
	Vector2 screenUIPos = Vector2(-600.0f, (GAME_CONFIG->GetScreenHeight() / 5));

	// UI�\���ʒu���X�N���[���̕��E��������ݒ�
	m_uiPos = screenUIPos;
}
