#pragma once
#include "Actor.h"
#include "PlayerCar.h"
#include <string>

class CarWheel : public Actor
{

public:

	// �ԑ̂ɂ�����^�C���̈ʒu
	enum WHEEL_POSITION
	{
		FRONT_LEFT = 0,
		FRONT_RIGHT,
		BACK_LEFT,
		BACK_RIGHT,
		ALL_WHEEL_NUM
	};

	CarWheel(class PlayerCar* in_owner, WHEEL_POSITION in_enumPos);
	~CarWheel();
	void UpdateActor(float in_deltaTime) override;



	//-------------------------------------------------+
	// Getter/Setter
	//-------------------------------------------------+
	class PlayerCar* GetCarPtr() { return m_owner; }

	void SetSpin(bool in_spin) { m_spin = in_spin; }
	bool GetSpin() { return m_spin; }

private:

	WHEEL_POSITION m_wheelPosition;          // �^�C�����ԑ̂̂ǂ̈ʒu�ɂ��邩

	class PlayerCar* m_owner;                // �I�[�i�[�N���X(�Ǐ]����N���X)

	class SandSmoke* m_smoke;                // �����N���X (�p�[�e�B�N��)

	float m_playerRadian;                    // �v���C���[�̌ʓx�Ď��p

	Vector3 m_adjustPos;                     // �^�C���p�ʒu�����x�N�g��

	bool m_spin;                             // ��]���邩���Ȃ���

	static const std::string CAR_WHEEL_RUBBER_MESH_PATH;
	static const std::string CAR_WHEEL_STEEL_MESH_PATH;
};

