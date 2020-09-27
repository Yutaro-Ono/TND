//-----------------------------------------------------------------------+
// プレイヤークラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"

#include <vector>
#include <unordered_map>


class Player : public Actor
{


public:

	enum AnimState
	{
		IDLE,
		SPIN,
		CHARGE
	};

	enum MoveState
	{
		DRIVE_IDLE,
		DRIVE_ACCEL,
		DRIVE_BRAKE
	};

	Player(bool in_isTitle);
	~Player();

	void UpdateActor(float in_deltaTime) override;

	void CollisionFix(class BoxCollider *in_hitPlayerBox, class BoxCollider *in_hitBox);

	void UpdateMovement(float in_deltaTime);          // プレイヤーの移動処理(アクセル・ブレーキ・回転)

	void UpdateSpeedRank();         // 速度ごとのランク (これを基にスコアの倍率を変更)

	void SpinTitle(float in_deltaTime);               // タイトル画面の回転処理

	// 追加項目
	void ComputeWorldTransform2();

	// Setter / Getter
	void SetAccelLimit(const float& in_limitVal) { m_accelLimit += in_limitVal; }           // アクセル最大値のセッター
	void SetBrakeLimit(const float& in_limitVal) { m_brakeLimit = in_limitVal; }            // ブレーキ最大値のセッター

	MoveState GetMoveState() { return m_moveState; }
	void SetMoveState(MoveState in_state) { m_moveState = in_state; }                       // 移動状態ステートセッター

	// チェイン系関数
	void AddFlameChain();                                                                   // チェインの加算
	void SubFlameChain();                                                                   // チェインの減算処理
	void RemoveFlameChain() { m_flameChain = 0; }                                           // チェインの終了
	const int& GetFlameChain() const { return m_flameChain; }                               // チェインのゲッター
	const float& GetChainTimer() const { return m_chainTimer; }                             // チェインタイマーのゲッター
	const float& GetChainTimerLimit() const { return CHAIN_TIMER; }                         // チェインタイマーの規定値のゲッター

	const float& GetAccel() const { return m_accel; }                                       // アクセル値(速度)のゲッター
	const float& GetAccelLimit() const { return m_accelLimit; }                             // アクセル値(制限速度)のゲッター
	const float& GetAccelMax() const { return ACCEL_LIMIT_MAX; }                            // アクセル値(ゲーム中での最高速度)のゲッター
	const float& GetBrake() const { return m_brake; }                                       // ブレーキ値(速度)のゲッター
	const int& GetSpeedRank() const { return m_maxSpeedRank; }                              // スピードランクのゲッター
	
	void AllStopSound();

	void InitRadian() { m_radian = 0.0f; }                                                  // Z軸回転値の初期化
	void InitStatus();                                                                      // プレイヤーのステータス初期化(主にフリーモードで取得した分の初期化)

	void SetTomosuFire();
	void SetTitlePlayer();

private:

	class MeshComponent* m_meshComp;

	// 当たり判定ボックス
	class BoxCollider* m_hitBox;
	class BoxCollider* m_hitGroundBox;
	class BoxCollider* m_hitHeadBox;

	// ともし火
	class FlameActor* m_flame;
	std::vector<class FlameActor*> m_tomosuFire;


	// 砂煙
	class SandSmoke* m_smoke;

	// 火花
	class FlameSpark* m_spark;

	// ステート
	AnimState m_animState;                                                                  // アニメーションステート
	MoveState m_moveState;                                                                  // 移動ステート(アクセル・バック・待機)

	bool m_isJump;
	bool m_onGround;

	// アクセル・ブレーキ加速度
	float m_accel;
	float m_brake;
	// ハンドブレーキ
	float m_handBrake;
	// アクセル・ブレーキ限界値
	float m_accelLimit;
	float m_brakeLimit;

	// 最高速度のランク(炎ゲージに反映)
	int m_maxSpeedRank;
	// 炎チェインシステム用(仮実装)
	int m_flameChain;
	// 炎チェインシステム用タイムカウンター
	float m_chainTimer;


	// 回転用
	float m_radian;                                                                          // Z軸
	float m_spinRadian;                                                                      // Y軸

	float m_spinAngle;                                                                       // 回転モーションの回転値

	bool m_isTitle;                                                                          // タイトル用かどうか

	Vector3 m_jumpVec;
	Vector3 m_velocityVec;

	static const int CHAIN_LIMIT;                                                            // チェイン数の限界
	static const int RANK_MAX;                                                               // ランクの最大値
	static const float CHAIN_TIMER;                                                          // チェイン時タイマー
	static const float HAND_BRAKE_VALUE;                                                     // ハンドブレーキの減速させる速度
	static const float ACCEL_LIMIT_MAX;                                                      // 加速度のリミット

    std::unordered_map<std::string, std::string> m_sound;                                    // プレイヤー関連の効果音

};