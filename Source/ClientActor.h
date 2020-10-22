//--------------------------------------------------------+
// �˗��l�A�N�^ (�~�b�V���������|�C���g)
//--------------------------------------------------------+
#pragma once
#include "Actor.h"

class ClientActor : public Actor
{

public:


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

	ClientActor(const Vector3& in_pos);
	~ClientActor();

	void UpdateActor(float in_deltaTime) override;

	//--------------------------------------------------+
	// Getter / Setter
	//--------------------------------------------------+
	bool GetIsSelected() { return m_isSelected; }
	void SetIsSelected(bool in_select) { m_isSelected = in_select; }


private:

	bool m_isSelected;                           // ���݃A�N�e�B�u�ȃ~�b�V�����̍��W�Ƃ��Đݒ肳��Ă��邩

	CLIENT_ANIM m_animState;                     // �A�j���[�V�����̃X�e�[�g
	std::vector<const class Animation*> m_anim;  // �A�j���[�V����

	class SkeletalMeshComponent* m_skelComp;     // �X�P���^�����b�V��

	class BoxCollider* m_hitBox;                 // �����蔻��{�b�N�X

	class SpriteComponent* m_landMark;           // �����h�}�[�N

};