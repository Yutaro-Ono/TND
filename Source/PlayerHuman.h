#pragma once
#include "Actor.h"

class PlayerHuman : public Actor
{

public:

	// �v���C���[�̎��_���[�h
	typedef enum PLAYER_POV
	{
		FIRST_PERSON,
		THIRD_PERSON
	};

	// �A�j���[�V�����^�C�v
	typedef enum PLAYER_ANIM
	{
		ANIM_IDLE = 0,
		ANIM_WALKING,
		ANIM_RUNNING,
		ANIM_JUMPSTART,
		ANIM_JUMPLOOP,
		ANIM_LANDING,
		ANIM_ALL_NUM
	};

	PlayerHuman(class PlayerManager* in_manager);           // �R���X�g���N�^
	~PlayerHuman();          // �f�X�g���N�^

	void UpdateActor(float in_deltaTime) override;     // �X�V����

	void ChangeState();                                // �X�e�[�g�̕ύX����

	void FixCollision(class BoxCollider* in_hitPlayerBox, class BoxCollider* in_hitBox);


	//-------------------------------------------+
	// Getter/Setter
	//-------------------------------------------+
	// POV���[�h�̃Z�b�^�[
	void SetPlayerPov(PLAYER_POV in_pov) { m_pov = in_pov; }


private:

	PLAYER_POV m_pov;       // ���_���[�h

	class HumanCamera* m_camera;        // �J����

	class PlayerManager* m_manager;     // �X�e�[�g����̃}�l�[�W�����g�N���X

	class SkeletalMeshComponent* m_skelMeshComp;         // �X�P���^�����b�V��
	
	std::vector<const class Animation*> m_animTypes;     // �A�j���[�V�����̃^�C�v
	PLAYER_ANIM m_animState;                             // �A�j���[�V�������Ƃ̏��

	// �����蔻��{�b�N�X(���ʂ���(�́E���E��))
	class BoxCollider* m_hitBox;
	class BoxCollider* m_hitGroundBox;
	class BoxCollider* m_hitHeadBox;


	bool m_isJump;           // �W�����v������
	bool m_onGround;         // �n�ʂɑ������Ă��邩

	Vector3 m_jumpVec;       // �W�����v�p�x�N�g��
	Vector3 m_velocityVec;   // �����p�x�N�g��
};