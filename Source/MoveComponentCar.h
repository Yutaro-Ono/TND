#pragma once
#include "MoveComponent.h"

class MoveComponentCar : public MoveComponent
{

public:

	enum DRIVE_STATE
	{
		DRIVE_IDLE,
		DRIVE_ACCEL,
		DRIVE_BRAKE
	};

	MoveComponentCar(class Actor* in_owner);
	~MoveComponentCar();

	void Update(float in_deltaTime) override;                          // �R���|�[�l���g�̍X�V

	void ProcessInput() override {}                                    // �R���|�[�l���g�̓��͏���

	void OnUpdateWorldTransform() override {}                      // ���[���h���W�ϊ��p

	void MovementByController(float in_deltaTime);

	void MovementByKeyboard(float in_deltaTime);


	const float GetAccelValue() const { return m_accelValue; }
	void SetAccel(const float in_accel) { m_accelValue = in_accel; }

	void SetDriveState(DRIVE_STATE in_state) { m_driveState = in_state; }

protected:


	DRIVE_STATE m_driveState;

	bool m_padIsActive;                 // �R���g���[�����A�N�e�B�u���ǂ���

	// �A�N�Z���E�u���[�L�����x
	float m_accelValue;
	float m_brakeValue;
	// �n���h�u���[�L
	float m_handBrake;
	// �A�N�Z���E�u���[�L���E�l
	float m_accelLimit;
	float m_brakeLimit;

	static const float HAND_BRAKE_VALUE;                                                     // �n���h�u���[�L�ɂ�錸����
    static const float ACCEL_LIMIT;
	static const float BRAKE_LIMIT;
};