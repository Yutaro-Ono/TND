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



	// 車のパーツクラス
	class TitleCarBody* m_body;
	class TitleCarDoor* m_door[2];
	class TitleCarWheel* m_wheel[4];
	class TitleCarHandle* m_handle;

	// 定点カメラ
	class PointCameraComponent* m_camera;
	// カメラ配置
	TITLE_CAR_CAMERA m_cameraPosType;

	// 前進速度
	float m_accel;


	// カメラ切り替え用タイマー
	unsigned int m_nowTime;

};