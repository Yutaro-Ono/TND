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



	// 車のパーツクラス
	class ResultCarBody* m_body;
	class ResultCarDoor* m_door[2];
	class ResultCarWheel* m_wheel[4];
	class ResultCarHandle* m_handle;

	// 定点カメラ
	class PointCameraComponent* m_camera;
	// カメラ配置
	RESULT_CAR_CAMERA m_cameraPosType;

	// 前進速度
	float m_accel;


	// カメラ切り替え用タイマー
	bool m_nextScene;

};