#pragma once
#include "Actor.h"


class ResultCar : public Actor
{

public:

	enum RESULT_CAR_CAMERA
	{
		NORMAL,
		END
	};


	ResultCar();
	~ResultCar();

	void UpdateActor(float in_deltaTime) override;


private:



	// �Ԃ̃p�[�c�N���X
	class ResultCarBody* m_body;
	class ResultCarDoor* m_door[2];
	class ResultCarWheel* m_wheel[4];
	class ResultCarHandle* m_handle;

	// ��_�J����
	class PointCameraComponent* m_camera;
	// �J�����z�u
	RESULT_CAR_CAMERA m_cameraPosType;

	// �O�i���x
	float m_accel;


	// �J�����؂�ւ��p�^�C�}�[
	bool m_nextScene;

};