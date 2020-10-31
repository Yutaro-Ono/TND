#pragma once
#include "GameMain.h"


class PhysicsWorld
{
public:

	typedef enum
	{
		TYPE_PLAYER_CAR,
		TYPE_PLAYER_HUMAN,
		TYPE_CAMERA,
		TYPE_ENEMY,
		TYPE_FLAME,
		TYPE_TERRAIN,
		TYPE_BACK_GROUND,
		TYPE_HIT_CHECK
	}PhysicsType;

	PhysicsWorld();
	~PhysicsWorld();

	void AddBoxCollider(PhysicsType in_type, class BoxCollider* in_box);      // BoxCollider(AABB)追加
	void RemoveBoxCollider(class BoxCollider* in_box);                        // BoxColliderの削除

	 

	void DebugShowBoxLists();                                                 // ボックスリスト表示
	void Collision();                                                         // コリジョン
	
	void DebugShowBox();                                                      // デバッグ用ボックス表示(未実装)
	void ToggleDebugMode() { m_boolDebugMode = !m_boolDebugMode; }            // デバッグモード

private:

	bool m_boolDebugMode;
	std::vector<class BoxCollider*> m_bgBoxes;                                // 背景当たり判定
	std::vector<class BoxCollider*> m_playerCarBoxes;                         // プレイヤー当たり判定
	std::vector<class BoxCollider*> m_playerHumanBoxes;                       // プレイヤー(人間)当たり判定ボックス
	std::vector<class BoxCollider*> m_cameraBoxes;                            // カメラ当たり判定
	std::vector<class BoxCollider*> m_terrain;                                // 地形当たり判定


	void PlayerAndBGTest();                                                   // プレイヤーと壁の当たり判定テスト

};