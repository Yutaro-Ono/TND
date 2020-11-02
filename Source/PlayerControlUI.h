//-----------------------------------------------------------------------+
// �������UI�N���X
//-----------------------------------------------------------------------+

#pragma once
#include "UIScreen.h"



class PlayerControlUI : public UIScreen
{
public:

	PlayerControlUI(class PlayerManager* in_player);
	~PlayerControlUI();

	void Update(float in_deltaTime) override;

	void Draw(class Shader* in_shader) override;

private:



	class PlayerManager* m_player;      // �v���C���[�ւ̃|�C���^

	class WorldSpaceUI* m_rideTexture;
	
	class Texture* m_accessTex;            // �A�N�Z�X�̑������
	Vector2 m_accessTexPos;
	
	class Texture* m_dangerFontTex;            // �댯�ȏ�� (�w���Ɍ�������)�\��

};

