#pragma once
#include "GameMain.h"
//-------------------------------------------------+
//
// �C���x�[�X�N���X
//
//-------------------------------------------------+


class MissionBase
{

public:

	MissionBase();
	~MissionBase();

	void SetPosition(const Vector3& in_start, const Vector3& in_goal);


protected:


	// �ڕW��`
	Vector3 m_startPos;          // �o���n�_(�˗��҂̈ʒu)
	Vector3 m_goalPos;           // �S�[���n�_(�z����̈ʒu)

	unsigned int m_baseScore;    // �X�R�A

	unsigned int m_timeLimit;    // ��������
};