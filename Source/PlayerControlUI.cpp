#include "PlayerControlUI.h"
#include "PlayerManager.h"
#include "Font.h"
#include "Texture.h"
#include "GameMain.h"
#include "Renderer.h"
#include "WorldSpaceUI.h"
#include "GameWorld.h"

const Vector3 ADJUST_RIDETEX_POS(-20.0f, 0.0f, 70.0f);     // ���UI�̈ʒu�����p
const Vector2 ADJUST_ACCESSTEX_POS(30.0f, 0.0f);    // ��UI�̈ʒu�����p

// �R���X�g���N�^
PlayerControlUI::PlayerControlUI(GameWorld* in_world)
	:m_world(in_world)
	,m_rideTexture(nullptr)
	,m_chasing(nullptr)
	,m_accessTexPos(Vector2::Zero)
	,m_findPlayer(false)
{
	m_player = m_world->GetPlayer();

	// ��ԃe�N�X�`���̐���
	m_rideTexture = new WorldSpaceUI(m_player->GetPlayerCarPos() + ADJUST_RIDETEX_POS, "Data/Interface/TND/Control/Control_ride.png", 35.0f);

	// �~�b�V������UI
	m_accessTex = RENDERER->GetTexture("Data/Interface/TND/Control/Control_AccessMission.png");

	// "DANGER"�\���p�̃e�N�X�`������
	m_dangerFontTex = m_font->RenderText("DANGER", Vector3(0.0, 0.0, 1.0), 48);

	// "�ǐՒ�"�\��
	m_chasing = RENDERER->GetTexture("Data/Interface/TND/Control/Chasing_Helicopter.png");
}

// �f�X�g���N�^
PlayerControlUI::~PlayerControlUI()
{
}

// �X�V����
void PlayerControlUI::Update(float in_deltaTime)
{
	// ���UI�̍��W�X�V�E�\���t���O�؂�ւ�
	m_rideTexture->SetPosition(m_player->GetPlayerCarPos() + ADJUST_RIDETEX_POS);
	m_rideTexture->SetVisible(m_player->GetIsDetectedCar());
}

// �`�揈��
void PlayerControlUI::Draw(Shader* in_shader)
{
	float scale = 0.3f;

	if (m_player->GetIsDetectedClient())
	{
		DrawTexture(in_shader, m_accessTex, m_accessTexPos + ADJUST_ACCESSTEX_POS, scale);
	}

	if (m_findPlayer)
	{
		DrawTexture(in_shader, m_dangerFontTex, Vector2(0.0f, GAME_CONFIG->GetScreenHeight() / 2.5), 1.0);
	}
}
