#pragma once
#include <list>


class MissionManager
{

public:

	MissionManager(class GameWorld* in_world);
	~MissionManager();


private:


	std::list<class MissionBase*> m_missions;     // �C���X�^�b�N



	static const int MISSION_ALL_NUM;             // �����i�s����C���̌��E��

};