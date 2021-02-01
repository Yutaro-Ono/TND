#include "LoadScreen.h"
#include "Texture.h"
#include "Font.h"
#include "GameMain.h"
#include "AudioManager.h"
#include <Windows.h>
#include <sstream>

const int LoadScreen::GAUGE_NUM = 64;

// �R���X�g���N�^
LoadScreen::LoadScreen()
	:m_state(DISABLE)
	,m_gaugeNum(0)
	,m_isGame(false)
{

	// "Loading"����
	m_loading = m_font->RenderText("Loading", Vector3(1.0f, 1.0f, 1.0f), 64);

	// ���[�h�p�Q�[�W
	for (int i = 0; i < GAUGE_NUM; i++)
	{
		std::stringstream ssGauge;
		// �t�@�C���p�X
		ssGauge << "Data/Interface/TND/Load/load_" << i << ".png";
		m_loadGauges.push_back(RENDERER->GetTexture(ssGauge.str()));
	}

	// �w�i
	m_bgTexture = RENDERER->GetTexture("Data/Interface/System/Load/LoadScreen_BackGround.png");

	// �`���[�g���A�����b�Z�[�W
	// m_tutorialMsg = RENDERER->GetTexture("Data/Interface/System/Load/LoadScreen_tuto_1.png");

	// ���[�h���̌��ʉ������[�h
	m_sound["Load"] = "Data/Music/SE/TND/System/Load/decision-6 (mp3cut.net).wav";
	AUDIO->GetSound(m_sound["Load"]);
	AUDIO->SetSoundVolume(m_sound["Load"], 2);

	// ���W
	m_loadingPos = Vector2(0.0f, -RENDERER->GetScreenHeight() / 2 + m_loading->GetHeight() + 180.0f);
	m_loadGaugePos = Vector2(0.0f, -RENDERER->GetScreenHeight() / 2 + 100.0f);
}

// �f�X�g���N�^
LoadScreen::~LoadScreen()
{
	// �����Ȃ�
}

// ����������
void LoadScreen::Initialize()
{
	m_gaugeNum = 1;
	m_state = ENABLE;
}

void LoadScreen::Update(float in_deltaTime)
{
}

void LoadScreen::Draw(Shader * in_shader)
{

	if (m_state == ENABLE)
	{
		// �w�i
		if (m_bgTexture)
		{
			glDisable(GL_BLEND);
			DrawTexture(in_shader, m_bgTexture, Vector2(0.0f, 0.0f), 1.0f);
			glEnable(GL_BLEND);
		}

		// �������
		if (m_tutorialMsg && m_isGame == true)
		{
			//DrawTexture(in_shader, m_tutorialMsg, Vector2(0.0f, 0.0f), 1.0f);
		}

		if (m_loading)
		{
			DrawTexture(in_shader, m_loading, m_loadingPos, 1.0f);
		}

		// ���[�h�Q�[�W�̕`��
		for (int i = 0; i < m_gaugeNum; i++)
		{
			DrawTexture(in_shader, m_loadGauges[i], m_loadGaugePos, 1.0f);
		}
	}

}


void LoadScreen::AddGauge()
{
	if (m_gaugeNum < GAUGE_NUM)
	{
		// ���ʉ��Đ�
		AUDIO->PlaySoundTND(m_sound["Load"]);

		// �Q�[�W���𑝂₷
		m_gaugeNum++;
	}
	// �`��
	RENDERER->Draw();
	// �E�F�C�g
	Sleep(80);
}
