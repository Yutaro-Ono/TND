//-----------------------------------------------------------------------+
// �v���C���[�}�l�[�W���[�N���X
// �@�\�F��Ƀv���C���[�̐l�ԃ��[�h�E�ԗ��^�]���[�h�𐧌䂷��
// �����F�A�N�^�[�p��
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"

class PlayerManager : public Actor
{
public:

	enum PLAYER_MODE
	{
		MODE_HUMAN,
		MODE_CAR
	};


	PlayerManager();
	~PlayerManager();

	void UpdateActor(float in_deltaTime) override;


private:

	PLAYER_MODE m_playerMode;

	// �v���C���[�̐l�ԁE�ԃN���X
	class PlayerHuman* m_playerHuman;
	class PlayerCar* m_playerCar;




};