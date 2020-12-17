#pragma once
#include "Actor.h"

class PlayerHuman : public Actor
{

public:

	// �v���C���[�̎��_���[�h
	enum PLAYER_POV
	{
		POV_FIRST_PERSON,
		POV_THIRD_PERSON
	};

	// �A�j���[�V�����^�C�v
	enum PLAYER_ANIM
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

	PlayerHuman(class PlayerManager* in_manager);           // �R���X�g���N�^
	~PlayerHuman();          // �f�X�g���N�^

	void UpdateActor(float in_deltaTime) override;     // �X�V����

	void ChangeState(float in_deltaTime);                                // �X�e�[�g�̕ύX����

	void CollisionFix(class BoxCollider* in_hitPlayerBox, class BoxCollider* in_hitBox);

	void SetActive(bool in_active);

	//-------------------------------------------+
	// Getter/Setter
	//-------------------------------------------+
	// POV���[�h�̃Z�b�^�[
	void SetPlayerPov(PLAYER_POV in_pov) { m_pov = in_pov; }
	PLAYER_POV GetPlayerPOV() { return m_pov; }
	// �A�j���[�V�����X�e�[�g�̃Z�b�^�[
	PLAYER_ANIM SetPlayerAnimState(PLAYER_ANIM in_animState) { m_animState = in_animState; }
	// �J�����N���X(�O�l�̎��_)�̃|�C���^�Q�b�^�[
	class ThirdPersonCamera* GetCamera() { return m_cameraComp; }
	// �}�l�[�W���[�N���X�|�C���^�̎擾
	class PlayerManager* GetPlayerManager() { return m_manager; }

private:

	PLAYER_POV m_pov;                                    // ���_���[�h

	class ThirdPersonCamera* m_cameraComp;               // �J����
	class MoveComponentHuman* m_moveComp;                // �ړ��p�R���|�[�l���g

	class PointLight* m_faceLight;                       // �v���C���[�t�F�C�X�p���C�g


	class PlayerManager* m_manager;                      // �X�e�[�g����̃}�l�[�W�����g�N���X

	class SkeletalMeshComponent* m_skelMeshComp;         // �X�P���^�����b�V��
	
	std::vector<const class Animation*> m_animTypes;     // �A�j���[�V�����̃^�C�v
	PLAYER_ANIM m_animState;                             // �A�j���[�V�������Ƃ̏��

	// �����蔻��{�b�N�X(���ʂ���(�́E���E��))
	class BoxCollider* m_hitBox;
	class BoxCollider* m_hitGroundBox;
	class BoxCollider* m_hitHeadBox;

	bool m_isActive;         // �l�Ԃ̑��삪�L�����ǂ���
	bool m_isJump;           // �W�����v������
	bool m_onGround;         // �n�ʂɑ������Ă��邩

	Vector3 m_jumpVec;       // �W�����v�p�x�N�g��
	Vector3 m_velocityVec;   // �����p�x�N�g��
};