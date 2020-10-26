#pragma once
#include "Actor.h"


class PatrolPoint : public Actor
{

public:


	PatrolPoint(const Vector3& in_pos);
	~PatrolPoint();

	// ヘリの巡回地点として登録されているかのセット
	void SetIsSelected(bool in_select) { m_isSelected = in_select; }
	// ヘリの巡回地点として登録されているかのゲッター
	bool GetIsSelected() { return m_isSelected; }

private:


	bool m_isSelected;      // 巡回ポイントとして指定されているか



};