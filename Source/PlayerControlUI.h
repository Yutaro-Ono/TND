//-----------------------------------------------------------------------+
// �������UI�N���X
//-----------------------------------------------------------------------+

#pragma once
#include "UIScreen.h"



class PlayerControlUI : public UIScreen
{
public:

	PlayerControlUI(class GameWorld* in_world);
	~PlayerControlUI();

	void Update(float in_deltaTime) override;

	void Draw(class Shader* in_shader) override;

	void SetFindPlayer(bool in_find) { m_findPlayer = in_find; }


private:



	class PlayerManager* m_player;             // �v���C���[�ւ̃|�C���^

	class GameWorld* m_world;                  // ���[���h�ւ̃|�C���^

	class WorldSpaceUI* m_rideTexture;
	
	class Texture* m_accessTex;                // �~�b�V�����󒍂̑������
	class Texture* m_completeTex;              // �~�b�V���������̑������
	Vector2 m_accessTexPos;
	
	class Texture* m_controlPanel;             // ��������p�p�l��

	class Texture* m_land1;
	class Texture* m_land2;
	class Texture* m_selectMission;

	class Texture* m_dangerFontTex;            // �댯�ȏ�� (�w���Ɍ�������)�\��
	class Texture* m_chasing;                  // �w���ɂ��ǐՒ��ɕ\������A���[�g
	bool m_findPlayer;                         // �w���Ɍ����������ǂ���
};

