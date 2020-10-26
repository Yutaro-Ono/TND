#pragma once
#include "Actor.h"


class PatrolPoint : public Actor
{

public:


	PatrolPoint(const Vector3& in_pos);
	~PatrolPoint();

	// �w���̏���n�_�Ƃ��ēo�^����Ă��邩�̃Z�b�g
	void SetIsSelected(bool in_select) { m_isSelected = in_select; }
	// �w���̏���n�_�Ƃ��ēo�^����Ă��邩�̃Q�b�^�[
	bool GetIsSelected() { return m_isSelected; }

private:


	bool m_isSelected;      // ����|�C���g�Ƃ��Ďw�肳��Ă��邩



};