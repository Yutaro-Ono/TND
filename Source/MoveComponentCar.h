#pragma once
#include "MoveComponent.h"

class MoveComponentCar : public MoveComponent
{

public:



	MoveComponentCar(class PlayerCar* in_owner);
	~MoveComponentCar();

	void Update(float in_deltaTime) override;                      // コンポーネントの更新

	void ProcessInput(float in_deltaTime) override {}              // コンポーネントの入力処理

	void OnUpdateWorldTransform() override {}                      // ワールド座標変換用

	void MovementByController(float in_deltaTime);

	void MovementByKeyboard(float in_deltaTime);


	const float GetAccelValue() const { return m_accelValue; }
	const float GetBrakeValue() const { return m_brakeValue; }
	void SetAccel(const float in_accel) { m_accelValue = in_accel; }

	const float GetRadian() const { return m_radian; }                 // 弧度ゲッター

protected:


	class PlayerCar* m_playerCar;       // オーナーであるPlayerCarクラスへのポインタ

	// アクセル・ブレーキ加速度
	float m_accelValue;
	float m_brakeValue;
	// ハンドブレーキ
	float m_handBrake;
	// アクセル・ブレーキ限界値
	float m_accelLimit;
	float m_brakeLimit;

	// 弧度
	float m_radian;
	float m_pastRadian;              // 前フレームの弧度

	static const float HAND_BRAKE_VALUE;                                                     // ハンドブレーキによる減速量
    static const float ACCEL_LIMIT;
	static const float BRAKE_LIMIT;
};