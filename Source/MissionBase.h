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

	// �~�b�V�����̖ړI��
	typedef enum MISSION_TYPE
	{
		DELIVERY,     // �ו��z�B
		TAXI          // �^�N�V�[(�l���悹��)
	};
	// �~�b�V�����̃X�e�[�g
	typedef enum MISSION_STATE
	{
		HOLD,            // �ۗ����
		ACTIVE,          // �A�N�e�B�u
		SUCCESS,         // ����
		FAILED           // ���s
	};


	MissionBase(class MissionManager* in_manager, MISSION_TYPE in_type, int in_listNum);
	~MissionBase();

	// �X�V����
	void Update(float in_deltaTime);

	// �~�b�V�����ڍד��e�̒�`
	void SetMissionDetail(class ClientActor* in_start, class ClientActor* in_goal, unsigned int in_baseScore, unsigned int in_timeLimit);

	// �v���C���[�̍��W�`�J�n�n�_or�I���n�_���W�̋��������߁A���ȏ�ڋ߂��ă{�^������������^)
	void CheckDistPlayer(const Vector3& in_playerPos, const Vector3& in_missionPos);

	// �ϋv�l��������
	void DecraseDurableValue();

	//------------------------------------------------------------+
	// Getter / Setter
	//------------------------------------------------------------+
	// �~�b�V������Ԃ̃Z�b�^�[�E�Q�b�^�[
	void SetMissionState(MISSION_STATE in_state) { m_missionState = in_state; }
	MISSION_STATE GetMissionState() { return m_missionState; }
	// �J�n�n�_�A�N�^�[�̃Q�b�^�[�E�S�[���n�_�A�N�^�[�̃Q�b�^�[
	class ClientActor* GetStartActor() { return m_startActor; }
	class ClientActor* GetGoalActor() { return m_goalActor; }
	// �~�b�V�����^�C�v�̃Z�b�^�[�Q�b�^�[
	MISSION_TYPE GetMissionType() { return m_missionType; }
	// �v���C���[�Ƃ̋����̃Q�b�^�[
	float GetPlayerDistance() { return m_playerDistance; }

	// �������Ԏ擾
	unsigned int GetTimeLimit() { return m_timeLimit; }

	// �ϋv�l�擾
	unsigned int GetDurableValue() { return m_durableVal; }
	// ���X�g�ԍ��̃Q�b�^�[
	int GetListNum() { return m_listNum; }
	void SetListNum(int in_listNum) { m_listNum = in_listNum; }

	// �~�b�V�����}�l�[�W���[�̃Q�b�^�[
	class MissionManager* GetMissionManager() { return m_manager; }

	// �X�R�A�̌v�Z�E�Q�b�^�[
	int GetCalcScore();
	int GetBaseScore() { return m_baseScore; }

protected:


	MISSION_STATE m_missionState;     // �~�b�V�����̃X�e�[�g
	MISSION_TYPE m_missionType;       // �~�b�V�����^�C�v

	class Actor* m_client;           // �˗��҃A�N�^�[
	class MissionUI* m_missionUI;    // �~�b�V�����̓��e��\������UI�N���X
	class MissionManager* m_manager; // �~�b�V�����}�l�[�W���[

	// �ڕW��`
	class ClientActor* m_startActor;  // �o���n�_�ƂȂ�A�N�^�[
	class ClientActor* m_goalActor;   // �S�[���n�_�ƂȂ�A�N�^�[
	Vector3 m_startPos;              // �o���n�_(�˗��҂̈ʒu)
	Vector3 m_goalPos;               // �S�[���n�_(�z����̈ʒu)
	float m_playerDistance;          // �v���C���[�܂ł̋���


	unsigned int m_durableVal;       // �ϋv�l

	unsigned int m_baseScore;        // �X�R�A

	// �������Ԋ֘A
	unsigned int m_timeLimit;                   // ��������
	unsigned int m_lastTime, m_currentTime;     // ���Ԍv���p�̌��ݎ����ƍŏI�X�V����


	int m_listNum;                           // �~�b�V�������X�g�̉��Ԗڂ�
};