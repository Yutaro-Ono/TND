#pragma once
#include "GameMain.h"


class PhysicsWorld
{
public:

	typedef enum
	{
		TYPE_PLAYER_CAR,
		TYPE_PLAYER_HUMAN,
		TYPE_CAMERA,
		TYPE_ENEMY,
		TYPE_FLAME,
		TYPE_TERRAIN,
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
	std::vector<class BoxCollider*> m_playerCarBoxes;                         // �v���C���[�����蔻��
	std::vector<class BoxCollider*> m_playerHumanBoxes;                       // �v���C���[(�l��)�����蔻��{�b�N�X
	std::vector<class BoxCollider*> m_cameraBoxes;                            // �J���������蔻��
	std::vector<class BoxCollider*> m_terrain;                                // �n�`�����蔻��


	void PlayerAndBGTest();                                                   // �v���C���[�ƕǂ̓����蔻��e�X�g

};