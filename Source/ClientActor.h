//--------------------------------------------------------+
// 依頼人アクタ (ミッション発生ポイント)
//--------------------------------------------------------+
#pragma once
#include "Actor.h"

class ClientActor : public Actor
{

public:

	ClientActor(const Vector3& in_pos);
	~ClientActor();

	void UpdateActor(float in_deltaTime) override;

	//--------------------------------------------------+
	// Getter / Setter
	//--------------------------------------------------+
	bool GetIsSelected() { return m_isSelected; }
	void SetIsSelected(bool in_select) { m_isSelected = in_select; }


private:

	bool m_isSelected;             // 現在アクティブなミッションの座標として設定されているか

	class SpriteComponent* m_landMark;

};