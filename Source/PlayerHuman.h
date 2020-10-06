#pragma once
#include "Actor.h"

class PlayerHuman : public Actor
{

public:

	// プレイヤーの視点モード
	typedef enum PLAYER_POV
	{
		FIRST_PERSON,
		THIRD_PERSON
	};

	// アニメーションタイプ
	typedef enum PLAYER_ANIM
	{
		ANIM_IDLE = 0,
		ANIM_WALKING,
		ANIM_RUNNING,
		ANIM_JUMPSTART,
		ANIM_JUMPLOOP,
		ANIM_LANDING,
		ANIM_ALL_NUM
	};

	PlayerHuman(class PlayerManager* in_manager);           // コンストラクタ
	~PlayerHuman();          // デストラクタ

	void UpdateActor(float in_deltaTime) override;     // 更新処理

	void ChangeState();                                // ステートの変更処理

	void FixCollision(class BoxCollider* in_hitPlayerBox, class BoxCollider* in_hitBox);


	//-------------------------------------------+
	// Getter/Setter
	//-------------------------------------------+
	// POVモードのセッター
	void SetPlayerPov(PLAYER_POV in_pov) { m_pov = in_pov; }


private:

	PLAYER_POV m_pov;       // 視点モード

	class HumanCamera* m_camera;        // カメラ

	class PlayerManager* m_manager;     // ステート制御のマネージメントクラス

	class SkeletalMeshComponent* m_skelMeshComp;         // スケルタルメッシュ
	
	std::vector<const class Animation*> m_animTypes;     // アニメーションのタイプ
	PLAYER_ANIM m_animState;                             // アニメーションごとの状態

	// 当たり判定ボックス(部位ごと(体・足・頭))
	class BoxCollider* m_hitBox;
	class BoxCollider* m_hitGroundBox;
	class BoxCollider* m_hitHeadBox;


	bool m_isJump;           // ジャンプしたか
	bool m_onGround;         // 地面に足がついているか

	Vector3 m_jumpVec;       // ジャンプ用ベクトル
	Vector3 m_velocityVec;   // 加速用ベクトル
};