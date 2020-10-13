//-----------------------------------------------------------------------+
// �X�s�[�h���[�^�[UI�N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "SpeedMeterUI.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Texture.h"
#include "Font.h"
#include <string>
#include <sstream>


// �R���X�g���N�^
SpeedMeterUI::SpeedMeterUI(Player * in_target)
	:m_player(in_target)
	,m_fontSize(68)
	,m_currentSpeedRate(0)
{

	//// �v���C���[�̑��x��int�^�Ŏ擾
	//int tempSpeed = (int)m_player->GetAccel();
	//std::string strSpeed;          // string
	//std::stringstream ssSpeed;     // stringstream

	//strSpeed = std::to_string(tempSpeed);     // int�^�̑��x��string�ɕϊ�
	//ssSpeed << strSpeed << "km/h";             // sstream�ő��x�P�ʂ�ǉ�
	//strSpeed = ssSpeed.str();                 // �ŏI�o��

	//// �`�悷��t�H���g�𐶐�
	////m_speedTex = m_font->RenderText(strSpeed, Vector3(1.0f, 1.0f, 0.0f), m_fontSize);
	//// �\�����W�̐ݒ�
	//m_speedTexPos = Vector2(0.0f, (-RENDERER->GetScreenHeight() / 1.7f) + (m_fontSize * 2));

	//// �Q�[�W�̃��[�h
	//for (int i = 0; i < 20; i++)
	//{
	//	// �t�@�C���p�X�擾�p
	//	std::stringstream ssGauge;
	//	std::stringstream ssShadow;

	//	// �Q�[�W
	//	ssGauge << "Data/Interface/HUD/SpeedMeter/Meter/SpeedMeter_" << (i + 1) << ".png";
	//	m_gaugeTex[i] = RENDERER->GetTexture(ssGauge.str());
	//}

	//// �e
	//m_gaugeShadowTex = RENDERER->GetTexture("Data/Interface/HUD/SpeedMeter/Shadow/SpeedMeterShadow.png");

}

// �f�X�g���N�^
SpeedMeterUI::~SpeedMeterUI()
{
}

// �X�V����
void SpeedMeterUI::Update(float in_deltaTime)
{


	//int tempSpeed = (int)m_player->GetAccel();
	//std::string strSpeed;

	//std::stringstream ssSpeed;

	//// int
	//strSpeed = std::to_string(tempSpeed);     // int�^�̑��x��string�ɕϊ�
	//ssSpeed << strSpeed << "km/h";             // sstream�ő��x�P�ʂ�ǉ�
	//strSpeed = ssSpeed.str();                 // �ŏI�o��

	//// �`�悷��t�H���g�̐���
	//// m_speedTex = m_font->RenderText(strSpeed, Vector3(1.0f, 1.0f, 0.0f), m_fontSize);


	//// �v���C���[�̍ō����x�ƌ��ݑ��x�̔䗦���i�[
	//m_currentSpeedRate = m_player->GetAccel() / m_player->GetAccelMax() * 20;
	//if (m_currentSpeedRate > 20)
	//{
	//	m_currentSpeedRate = 20;
	//}


}

// �`�揈��
void SpeedMeterUI::Draw(Shader * in_shader)
{
	// �X�s�[�h���l�̕`��
	// DrawTexture(in_shader, m_speedTex, m_speedTexPos, 1.0f);

	//// �X�s�[�h���[�^�[�̉e�̕`��
	//DrawTexture(in_shader, m_gaugeShadowTex, Vector2(0.0f, -RENDERER->GetScreenHeight() / 3.5f), 0.7f);

	//// �X�s�[�h���[�^�[�̕`��
	//for (int i = 0; i < m_currentSpeedRate; i++)
	//{
	//	DrawTexture(in_shader, m_gaugeTex[i], Vector2(0.0f, -RENDERER->GetScreenHeight() / 3.5f), 0.7f);
	//}


}
