//--------------------------------------------------------+
// 依頼人アクタ (ミッション発生ポイント)
//--------------------------------------------------------+
#pragma once
#include "Actor.h"

class ClientActor : public Actor
{

public:

	// 依頼人の設定
	enum CLIENT_SETTING
	{
		NONE = 0,        // 何も割り当てられていない
		START = 1,       // 開始地点として割り当て
		GOAL = 2         // ゴール地点として割り当て
	};

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

	ClientActor(const Vector3& in_pos, int in_chara = 0);
	~ClientActor();

	void UpdateActor(float in_deltaTime) override;
	
	void SetAcceptedPlayer();      // プレイヤーにアクセスされた時の処理

	void LoadMeshEachChara(int in_chara = 0);      // コンストラクタで指定された引数に応じたメッシュのロード

	//--------------------------------------------------+
	// Getter / Setter
	//--------------------------------------------------+
	bool GetIsSelected() { return m_isSelected; }
	void SetIsSelected(bool in_select) { m_isSelected = in_select; }

	bool GetIsAccepted() { return m_isAccepted; }
	void SetIsAccepted(bool in_accept) { m_isAccepted = in_accept; }

	// 設定
	CLIENT_SETTING GetClientSetting() { return m_setting; }
	void SetClientSetting(CLIENT_SETTING in_setting) { m_setting = in_setting; }

	// ミッションのセット
	void SetMission(class MissionBase* in_mission) { m_mission = in_mission; }

	void SetDistancePlayer(float in_distance) { m_distancePlayer = in_distance; }

private:

	CLIENT_SETTING m_setting;                    // 依頼人が任務にどの役で割り当てられているか

	float m_distancePlayer;                      // プレイヤーとの距離

	bool m_isSelected;                           // 現在アクティブなミッションの座標として設定されているか
	bool m_isAccepted;                           // プレイヤーによりミッションが受諾されたか

	class MissionBase* m_mission;                // 自身に設定されたミッション

	CLIENT_ANIM m_animState;                     // アニメーションのステート
	std::vector<const class Animation*> m_anim;  // アニメーション
	class SkeletalMeshComponent* m_skelComp;     // スケルタルメッシュ

	class BoxCollider* m_hitBox;                 // 当たり判定ボックス

	class WorldSpaceUI* m_landMark;              // 位置を示すランドマークテクスチャ

	class Texture* m_distanceTex;
	class WorldSpaceUI* m_distanceWorld;              // 距離を示すテクスチャ

	class PointLight* m_light;                   // 位置をわかりやすくする光源
};