#pragma once
#include "Actor.h"

class PlayerHuman : public Actor
{

public:

	// �v���C���[�̎��_���[�h
	enum PLAYER_POV
	{
		FIRST_PERSON,
		THIRD_PERSON
	};

	PlayerHuman();           // �R���X�g���N�^
	~PlayerHuman();          // �f�X�g���N�^



	//-------------------------------------------+
	// Getter/Setter
	//-------------------------------------------+
	// POV���[�h�̃Z�b�^�[
	void SetPlayerPov(PLAYER_POV in_pov) { m_pov = in_pov; }


private:

	PLAYER_POV m_pov;       // ���_���[�h

	class HumanCamera* m_camera;

};