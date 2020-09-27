#pragma once
#include "GameMain.h"


class PhysicsWorld
{
public:

	typedef enum
	{
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_FLAME,
		TYPE_BACK_GROUND,
		TYPE_HIT_CHECK
	}PhysicsType;

	PhysicsWorld();
	~PhysicsWorld();

	void AddBoxCollider(PhysicsType in_type, class BoxCollider* in_box);      // BoxCollider(AABB)�ǉ�
	void RemoveBoxCollider(class BoxCollider* in_box);                        // BoxCollider�̍폜

	void DebugShowBoxLists();                                                 // �{�b�N�X���X�g�\��
	void Collision();                                                         // �R���W����
	
	void DebugShowBox();                                                      // �f�o�b�O�p�{�b�N�X�\��(������)
	void ToggleDebugMode() { m_boolDebugMode = !m_boolDebugMode; }            // �f�o�b�O���[�h

private:

	bool m_boolDebugMode;
	std::vector<class BoxCollider*> m_bgBoxes;                                // �w�i�����蔻��
	std::vector<class BoxCollider*> m_playerBoxes;                             // �v���C���[�����蔻��
	std::vector<class BoxCollider*> m_fires;                                  // �������蔻��

	void PlayerAndBGTest();                                                   // �v���C���[�ƕǂ̓����蔻��e�X�g

};