#pragma once
#include <vector>
#include <string>
#include "Math.h"

class MissionManager
{

public:


	MissionManager(class GameWorld* in_world);
	~MissionManager();

	void Update(float in_deltaTime);
	void InitRandom();
	void ChangeSelectNum();                  // ���Ԗڂ̃~�b�V�������w���Ă��邩

	float CalcScoreForDistance(const Vector3& in_pPos, const Vector3& in_cPos1, const Vector3& in_cPos2);     // �����ɉ������~�b�V�������Ƃ̃X�R�A�Z�o

	//------------------------------------------+
	// Getter / Setter
	//------------------------------------------+
	const std::vector<class MissionBase*>& GetMissionList() const { return m_missions; }     // �~�b�V�������X�g�̃Q�b�^�[
	class PlayerManager* GetPlayer() { return m_player; }

	// �J�[�\���őI�𒆂̃~�b�V�����ԍ��Q�b�^�[
	int GetSelectedMission() { return m_selectNum; }


private:

	class PlayerManager* m_player;                // �v���C���[�ւ̃|�C���^

	class GameWorld* m_world;                     // ���[���h�ւ̃|�C���^

	std::vector<class MissionBase*> m_missions;   // �C���X�^�b�N

	int m_selectNum;                              // �I�����Ă���~�b�V����No


	static const int MISSION_ALL_NUM;             // �����i�s����C���̌��E��

	std::string m_selectSE;                       // �J�[�\���I��ύX���̌��ʉ�

};