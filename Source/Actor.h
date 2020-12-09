//-----------------------------------------------------------------------+
// �A�N�^�[�N���X
// �@�\�F�A�N�^�[(��{�I�Ƀ��[���h��Ԃɍ��W��������)�̃x�[�X�N���X
// �����F�x�[�X�N���X
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

class Actor
{

public:


	// �v���C���[�̃X�e�[�g(���)
	enum State
	{
		STATE_ACTIVE,            // �A�N�e�B�u�ł���
		STATE_PAUSED,            // ��~���ł���
		STATE_DEAD,              // �폜������s��
		STATE_ALLSTATE           // �S�X�e�[�g
	};

	Actor();
	virtual ~Actor();     // ���z�f�X�g���N�^

	void Update(float in_deltaTime);                  // �X�V����(����)
	void UpdateComponents(float in_deltaTime);        // �X�V���� (�R���|�[�l���g)
	virtual void UpdateActor(float in_deltaTime);     // �X�V���� (�A�N�^�[�ŗL����:�I�[�o�[���C�h�\)
	void ProcessInput(float in_deltaTime);            // ���͏���

	//-----------------------------------------------------------------------------------+
	// Getter / Setter
	//-----------------------------------------------------------------------------------+
	// �|�W�V����
	const Vector3& GetPosition() const { return m_position; }
	void SetPosition(const Vector3& in_pos) { m_position = in_pos; m_recomputeWorldTransform = true; }
	// �X�P�[��
	float GetScale() const { return m_scale; }
	void SetScale(float in_scale) { m_scale = in_scale; m_recomputeWorldTransform = true; }
	// ���[�e�[�V����
	const Quaternion& GetRotation() const { return m_rotation; }
	void SetRotation(const Quaternion& in_rotation) { m_rotation = in_rotation; m_recomputeWorldTransform = true; }

	// ���[���h�ϊ��s��
	void ComputeWorldTransform();
	void SetWorldTransform(const Matrix4& in_mat) { m_worldTransform = in_mat; }
	const Matrix4& GetWorldTransform() const { return m_worldTransform; }
	// �x�N�g��
	const Vector3& GetForward() const { return Vector3::Transform(Vector3::UnitX, m_rotation); }     // �O�i
	void RotateToNewForward(const Vector3& in_forward);                                              // �O�i�x�N�g���̃Z�b�g
	const Vector3& GetRight() const { return Vector3::Transform(Vector3::UnitY, m_rotation); }       // �E����
	// �X�s�[�h
	float GetSpeed() const { return m_speed; }
	void SetSpeed(float in_speed) { m_speed = in_speed; }
	// �X�e�[�g
	Actor::State GetState() const { return m_state; }
	void SetState(Actor::State in_state) { m_state = in_state; }
	// �Ǘ�ID
	int GetID() { return m_ID; }
	// �R���|�[�l���g
	void AddComponent(class Component* in_comp);
	void RemoveComponent(class Component* in_comp);
	// ���b�V���R���|�[�l���g
	class MeshComponent* GetMeshComponent() { return m_meshComp; }



protected:

	Actor::State m_state;                           // �X�e�[�g(���)

	Vector3 m_position;                             // �|�W�V����
	Matrix4 m_worldTransform;                       // ���[���h�ϊ��s��
	Quaternion m_rotation;                          // ��]�p

	float m_radian;

	float m_scale;                                  // �X�P�[��(�g��E�k����)

	float m_speed;                                  // �ړ����x

	bool m_recomputeWorldTransform;                 // ���[���h���W�ϊ����s����

	class MeshComponent* m_meshComp;                // ���b�V���R���|�[�l���g�̃|�C���^

	std::vector<class Component*> m_components;     // �R���|�[�l���g�z��(�A�N�^�[�̎��R���|�[�l���g���i�[)

	int m_ID;                                       // �A�N�^�[ID
	static int m_globalActorNo;                     // �Q�[���S�̂ł̃A�N�^�[�̔ԍ�

};