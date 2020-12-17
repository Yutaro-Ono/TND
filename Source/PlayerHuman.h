#pragma once
#include "Actor.h"

class PlayerHuman : public Actor
{

public:

	// プレイヤーの視点モード
	enum PLAYER_POV
	{
		POV_FIRST_PERSON,
		POV_THIRD_PERSON
	};

	// アニメーションタイプ
	enum PLAYER_ANIM
	{
		ANIM_IDLE = 0,
		ANIM_IDLE_LOOKAROUND,
		ANIM_WALKING_FWD,
		ANIM_WALKING_BWD,
		ANIM_WALKING_LEFT,
		ANIM_WALKING_RIGHT,
		ANIM_RUNNING,
		ANIM_JUMPSTART,
		ANIM_JUMPLOOP,
		ANIM_LANDING,
		ANIM_ALL_NUM
	};

	PlayerHuman(class PlayerManager* in_manager);           // コンストラクタ
	~PlayerHuman();          // デストラクタ

	void UpdateActor(float in_deltaTime) override;     // 更新処理

	void ChangeState(float in_deltaTime);                                // ステートの変更処理

	void CollisionFix(class BoxCollider* in_hitPlayerBox, class BoxCollider* in_hitBox);

	void SetActive(bool in_active);

	//-------------------------------------------+
	// Getter/Setter
	//-------------------------------------------+
	// POVモードのセッター
	void SetPlayerPov(PLAYER_POV in_pov) { m_pov = in_pov; }
	PLAYER_POV GetPlayerPOV() { return m_pov; }
	// アニメーションステートのセッター
	PLAYER_ANIM SetPlayerAnimState(PLAYER_ANIM in_animState) { m_animState = in_animState; }
	// カメラクラス(三人称視点)のポインタゲッター
	class ThirdPersonCamera* GetCamera() { return m_cameraComp; }
	// マネージャークラスポインタの取得
	class PlayerManager* GetPlayerManager() { return m_manager; }

private:

	PLAYER_POV m_pov;                                    // 視点モード

	class ThirdPersonCamera* m_cameraComp;               // カメラ
	class MoveComponentHuman* m_moveComp;                // 移動用コンポーネント

	class PointLight* m_faceLight;                       // プレイヤーフェイス用ライト


	class PlayerManager* m_manager;                      // ステート制御のマネージメントクラス

	class SkeletalMeshComponent* m_skelMeshComp;         // スケルタルメッシュ
	
	std::vector<const class Animation*> m_animTypes;     // アニメーションのタイプ
	PLAYER_ANIM m_animState;                             // アニメーションごとの状態

	// 当たり判定ボックス(部位ごと(体・足・頭))
	class BoxCollider* m_hitBox;
	class BoxCollider* m_hitGroundBox;
	class BoxCollider* m_hitHeadBox;

	bool m_isActive;         // 人間の操作が有効かどうか
	bool m_isJump;           // ジャンプしたか
	bool m_onGround;         // 地面に足がついているか

	Vector3 m_jumpVec;       // ジャンプ用ベクトル
	Vector3 m_velocityVec;   // 加速用ベクトル
};