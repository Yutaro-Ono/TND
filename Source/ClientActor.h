//--------------------------------------------------------+
// �˗��l�A�N�^ (�~�b�V���������|�C���g)
//--------------------------------------------------------+
#pragma once
#include "Actor.h"

class ClientActor : public Actor
{

public:

	// �˗��l�̐ݒ�
	enum CLIENT_SETTING
	{
		NONE = 0,        // �������蓖�Ă��Ă��Ȃ�
		START = 1,       // �J�n�n�_�Ƃ��Ċ��蓖��
		GOAL = 2         // �S�[���n�_�Ƃ��Ċ��蓖��
	};

	enum CLIENT_ANIM
	{
		ANIM_IDLE = 0,
		ANIM_IDLE_LOOKAROUND,
		ANIM_WALKING_FWD,
		ANIM_WALKING_BWD,
		ANIM_WALKING_LEFT,
		ANIM_WALKING_RIGHT,
		ANIM_RUNNING,
		ANIM_JUMPSTART,
		ANIM_JUMPLOOP,
		ANIM_LANDING,
		ANIM_ALL_NUM
	};

	ClientActor(const Vector3& in_pos, int in_chara = 0);
	~ClientActor();

	void UpdateActor(float in_deltaTime) override;
	
	void SetAcceptedPlayer();      // �v���C���[�ɃA�N�Z�X���ꂽ���̏���

	void LoadMeshEachChara(int in_chara = 0);      // �R���X�g���N�^�Ŏw�肳�ꂽ�����ɉ��������b�V���̃��[�h

	//--------------------------------------------------+
	// Getter / Setter
	//--------------------------------------------------+
	bool GetIsSelected() { return m_isSelected; }
	void SetIsSelected(bool in_select) { m_isSelected = in_select; }

	bool GetIsAccepted() { return m_isAccepted; }
	void SetIsAccepted(bool in_accept) { m_isAccepted = in_accept; }

	// �ݒ�
	CLIENT_SETTING GetClientSetting() { return m_setting; }
	void SetClientSetting(CLIENT_SETTING in_setting) { m_setting = in_setting; }

	// �~�b�V�����̃Z�b�g
	void SetMission(class MissionBase* in_mission) { m_mission = in_mission; }

	void SetDistancePlayer(float in_distance) { m_distancePlayer = in_distance; }

private:

	CLIENT_SETTING m_setting;                    // �˗��l���C���ɂǂ̖��Ŋ��蓖�Ă��Ă��邩

	float m_distancePlayer;                      // �v���C���[�Ƃ̋���

	bool m_isSelected;                           // ���݃A�N�e�B�u�ȃ~�b�V�����̍��W�Ƃ��Đݒ肳��Ă��邩
	bool m_isAccepted;                           // �v���C���[�ɂ��~�b�V������������ꂽ��

	class MissionBase* m_mission;                // ���g�ɐݒ肳�ꂽ�~�b�V����

	CLIENT_ANIM m_animState;                     // �A�j���[�V�����̃X�e�[�g
	std::vector<const class Animation*> m_anim;  // �A�j���[�V����
	class SkeletalMeshComponent* m_skelComp;     // �X�P���^�����b�V��

	class BoxCollider* m_hitBox;                 // �����蔻��{�b�N�X

	class WorldSpaceUI* m_landMark;              // �ʒu�����������h�}�[�N�e�N�X�`��

	class Texture* m_distanceTex;
	class WorldSpaceUI* m_distanceWorld;              // �����������e�N�X�`��

	class PointLight* m_light;                   // �ʒu���킩��₷���������
};