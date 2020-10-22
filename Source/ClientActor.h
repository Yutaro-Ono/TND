//--------------------------------------------------------+
// 依頼人アクタ (ミッション発生ポイント)
//--------------------------------------------------------+
#pragma once
#include "Actor.h"

class ClientActor : public Actor
{

public:


	enum CLIENT_ANIM
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

	ClientActor(const Vector3& in_pos);
	~ClientActor();

	void UpdateActor(float in_deltaTime) override;

	//--------------------------------------------------+
	// Getter / Setter
	//--------------------------------------------------+
	bool GetIsSelected() { return m_isSelected; }
	void SetIsSelected(bool in_select) { m_isSelected = in_select; }


private:

	bool m_isSelected;                           // 現在アクティブなミッションの座標として設定されているか

	CLIENT_ANIM m_animState;                     // アニメーションのステート
	std::vector<const class Animation*> m_anim;  // アニメーション

	class SkeletalMeshComponent* m_skelComp;     // スケルタルメッシュ

	class BoxCollider* m_hitBox;                 // 当たり判定ボックス

	class SpriteComponent* m_landMark;           // ランドマーク

};