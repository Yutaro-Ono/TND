//-----------------------------------------------------------------------+
// �v���C���[�}�l�[�W���[�N���X
// �@�\�F��Ƀv���C���[�̐l�ԃ��[�h�E�ԗ��^�]���[�h�𐧌䂷��
// �����F�A�N�^�[�p��
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"
#include "PlayerHuman.h"
#include "PlayerCar.h"

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


	//----------------------------------------------------+
	// Getter/Setter
	//----------------------------------------------------+
	// �ԃN���X�ւ̃|�C���^�Q�b�^�[
	class PlayerCar* GetPlayerCar() { return m_playerCar; }
	// �v���C���[(�l��)�̍��W�Z�b�^�[
	void SetPlayerHumanPos(const Vector3& in_pos) { m_playerHuman->SetPosition(in_pos); }
	// �v���C���[(��)�̍��W�Z�b�^�[
	void SetPlayerCarPos(const Vector3& in_pos) { m_playerCar->SetPosition(in_pos); }

	// ���[�h�̃Q�b�^�[�E�Z�b�^�[
	PLAYER_MODE GetPlayerMode() { return m_playerMode; }
	void SetPlayerMode(PLAYER_MODE in_mode) { m_playerMode = in_mode; }

private:

	PLAYER_MODE m_playerMode;

	// �v���C���[�̐l�ԁE�ԃN���X
	class PlayerHuman* m_playerHuman;
	class PlayerCar* m_playerCar;




};