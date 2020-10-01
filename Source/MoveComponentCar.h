#pragma once
#include "MoveComponent.h"

class MoveComponentCar : public MoveComponent
{

public:



	MoveComponentCar(class PlayerCar* in_owner);
	~MoveComponentCar();

	void Update(float in_deltaTime) override;                      // �R���|�[�l���g�̍X�V

	void ProcessInput(float in_deltaTime) override {}              // �R���|�[�l���g�̓��͏���

	void OnUpdateWorldTransform() override {}                      // ���[���h���W�ϊ��p

	void MovementByController(float in_deltaTime);

	void MovementByKeyboard(float in_deltaTime);


	const float GetAccelValue() const { return m_accelValue; }
	const float GetBrakeValue() const { return m_brakeValue; }
	void SetAccel(const float in_accel) { m_accelValue = in_accel; }

	const float GetRadian() const { return m_radian; }                 // �ʓx�Q�b�^�[

protected:


	class PlayerCar* m_playerCar;       // �I�[�i�[�ł���PlayerCar�N���X�ւ̃|�C���^

	// �A�N�Z���E�u���[�L�����x
	float m_accelValue;
	float m_brakeValue;
	// �n���h�u���[�L
	float m_handBrake;
	// �A�N�Z���E�u���[�L���E�l
	float m_accelLimit;
	float m_brakeLimit;

	// �ʓx
	float m_radian;
	float m_pastRadian;              // �O�t���[���̌ʓx

	static const float HAND_BRAKE_VALUE;                                                     // �n���h�u���[�L�ɂ�錸����
    static const float ACCEL_LIMIT;
	static const float BRAKE_LIMIT;
};