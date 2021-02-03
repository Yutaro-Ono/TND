#pragma once
#include "Actor.h"


class TitleCar : public Actor
{

public:

	enum TITLE_CAR_CAMERA
	{
		NONE,
		FRONT_LEFT,
		BACK_LEFT,
		SIDE,
		FAR_FRONT,
		FRONT_RIGHT,
		BACK_RIGHT,
		TOP,
		UNDER
	};


	TitleCar();
	~TitleCar();

	void UpdateActor(float in_deltaTime) override;


private:



	// �Ԃ̃p�[�c�N���X
	class TitleCarBody* m_body;
	class TitleCarDoor* m_door[2];
	class TitleCarWheel* m_wheel[4];
	class TitleCarHandle* m_handle;

	// ��_�J����
	class PointCameraComponent* m_camera;
	// �J�����z�u
	TITLE_CAR_CAMERA m_cameraPosType;

	// �O�i���x
	float m_accel;


	// �J�����؂�ւ��p�^�C�}�[
	unsigned int m_nowTime;

};