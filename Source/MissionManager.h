#pragma once
#include <list>


class MissionManager
{

public:


	MissionManager(class GameWorld* in_world);
	~MissionManager();


	//------------------------------------------+
	// Getter / Setter
	//------------------------------------------+
	const std::list<class MissionBase*>& GetMissionList() const { return m_missions; }     // �~�b�V�������X�g�̃Q�b�^�[


private:


	std::list<class MissionBase*> m_missions;     // �C���X�^�b�N

	static const int MISSION_ALL_NUM;             // �����i�s����C���̌��E��

};