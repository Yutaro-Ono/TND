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

	// �~�b�V�����̃X�e�[�g
	typedef enum MissionState
	{
		HOLD,            // �ۗ����
		ACTIVE,          // �A�N�e�B�u
		SUCCESS,         // ����
		FAILED           // ���s
	};


	MissionBase(class Actor* in_client);
	~MissionBase();

	// �X�V����
	void Update(float in_deltaTime);

	// �~�b�V�����ڍד��e�̒�`
	void SetMissionDetail(const Vector3& in_start, const Vector3& in_goal, unsigned int in_baseScore, unsigned int in_timeLimit);

	// �~�b�V������Ԃ̃Z�b�^�[�E�Q�b�^�[
	void SetMissionState(MissionState in_state) { m_missionState = in_state; }
	MissionState GetMissionState() { return m_missionState; }


protected:


	MissionState m_missionState;     // �~�b�V�����̃X�e�[�g

	class Actor* m_client;           // �˗��҃A�N�^�[

	class MissionUI* m_missionUI;    // �~�b�V�����T�v�\��UI

	// �ڕW��`
	Vector3 m_startPos;              // �o���n�_(�˗��҂̈ʒu)
	Vector3 m_goalPos;               // �S�[���n�_(�z����̈ʒu)

	unsigned int m_baseScore;        // �X�R�A

	// �������Ԋ֘A
	unsigned int m_timeLimit;                   // ��������
	unsigned int m_lastTime, m_currentTime;     // ���Ԍv���p�̌��ݎ����ƍŏI�X�V����

};