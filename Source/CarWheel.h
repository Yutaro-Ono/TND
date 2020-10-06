#pragma once
#include "Actor.h"
#include "PlayerCar.h"
#include <string>

class CarWheel : public Actor
{

public:

	// �ԑ̂ɂ�����^�C���̈ʒu
	typedef enum WHEEL_POSITION
	{
		FRONT_LEFT = 0,
		FRONT_RIGHT,
		BACK_LEFT,
		BACK_RIGHT,
		ALL_WHEEL_NUM
	};

	CarWheel(class PlayerCar* in_owner, const std::string& in_meshPath, WHEEL_POSITION in_enumPos);
	~CarWheel();
	void UpdateActor(float in_deltaTime) override;

	class MeshComponent* GetMeshComponent() { return m_meshComp; }     // ���b�V���ւ̃|�C���^�Q�b�^�[


	//-------------------------------------------------+
	// Getter/Setter
	//-------------------------------------------------+


private:

	WHEEL_POSITION m_wheelPosition;          // �^�C�����ԑ̂̂ǂ̈ʒu�ɂ��邩

	class PlayerCar* m_owner;                // �I�[�i�[�N���X(�Ǐ]����N���X)

	class MeshComponent* m_meshComp;         // ���b�V���R���|�[�l���g

	float m_playerRadian;                    // �v���C���[�̌ʓx�Ď��p

	Vector3 m_adjustPos;                     // �^�C���p�ʒu�����x�N�g��
};

