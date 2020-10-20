#pragma once
#include <vector>


class MissionManager
{

public:


	MissionManager(class GameWorld* in_world);
	~MissionManager();

	void Update(float in_deltaTime);

	void InitRandom();

	//------------------------------------------+
	// Getter / Setter
	//------------------------------------------+
	const std::vector<class MissionBase*>& GetMissionList() const { return m_missions; }     // �~�b�V�������X�g�̃Q�b�^�[

	class PlayerManager* GetPlayer() { return m_player; }

private:

	class PlayerManager* m_player;                // �v���C���[�ւ̃|�C���^

	class GameWorld* m_world;                     // ���[���h�ւ̃|�C���^

	std::vector<class MissionBase*> m_missions;     // �C���X�^�b�N

	static const int MISSION_ALL_NUM;             // �����i�s����C���̌��E��

};