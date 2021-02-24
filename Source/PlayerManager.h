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
#include "Collision.h"

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

	void SearchCar();                                    // �v���C���[�̃A�N�Z�X�͈͓��ɎԂ����邩�̌����E�Ԃ̏�ԁE�~�ԏ���
	void SearchClient(class ClientActor* in_client);     // �v���C���[�̃A�N�Z�X�͈͓��Ɉ˗��l�����邩������

	//----------------------------------------------------+
	// Getter/Setter
	//----------------------------------------------------+
	// �v���C���[(�l��)�̍��W�Z�b�^�[
	void SetPlayerHumanPos(const Vector3& in_pos) { m_playerHuman->SetPosition(in_pos); }
	// �v���C���[(��)�̍��W�Z�b�^�[
	void SetPlayerCarPos(const Vector3& in_pos) { m_playerCar->SetPosition(in_pos); }
	const Vector3& GetPlayerCarPos() { return m_playerCar->GetPosition(); }

	// ���[�h�̃Q�b�^�[�E�Z�b�^�[
	PLAYER_MODE GetPlayerMode() { return m_playerMode; }
	void SetPlayerMode(PLAYER_MODE in_mode) { m_playerMode = in_mode; }

	// ���G�͈͂̃Z�b�^�[�E�Q�b�^�[
	Sphere GetSearchSphere() { return m_searchSphere; }
	bool GetIsDetectedCar() { return m_isDetectedCar; }           // �v���C���[�̃A�N�Z�X�͈͓��ɎԂ����o�������̃Q�b�^�[
	bool GetIsDetectedClient() { return m_isDetectedClient; }     // �v���C���[�̃A�N�Z�X�͈͓��Ɉ˗��l�����o�������̃Q�b�^�[

	// �v���C���[�|�C���^�̃Q�b�^�[
	class PlayerHuman* GetPlayerHuman() { return m_playerHuman; }
	// �ԃN���X�ւ̃|�C���^�Q�b�^�[
	class PlayerCar* GetPlayerCar() { return m_playerCar; }

private:

	PLAYER_MODE m_playerMode;

	// �v���C���[�̐l�ԁE�ԃN���X
	class PlayerHuman* m_playerHuman;
	class PlayerCar* m_playerCar;

	bool m_isDetectedClient;     // �v���C���[�̃A�N�Z�X�͈͓��Ɉ˗��l�����邩
	bool m_isDetectedCar;        // �v���C���[�̃A�N�Z�X�͈͓��ɎԂ����邩

	Sphere m_searchSphere;       // �v���C���[�̃A�N�Z�X�͈͋���

};