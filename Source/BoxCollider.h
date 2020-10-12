//-----------------------------------------------------------------------+
// BoxCollider�N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include "Collision.h"
#include "PhysicsWorld.h"

class BoxCollider : public Component
{
public:

	BoxCollider(class Actor* in_owner, PhysicsWorld::PhysicsType in_type, int in_updateOrder = 100);
	~BoxCollider();

	void OnUpdateWorldTransform() override;                                 // ���[���h�ϊ���
	void SetObjectBox(const AABB& in_box) { m_objectBox = in_box; }         // �����蔻��p���E�{�b�N�X���Z�b�g
	const AABB& GetWorldBox() const { return m_worldBox; }                  // ���[���h��ԏ�ł̋��E�{�b�N�X���擾
	void SetArrowRotate(bool in_value) { m_rotatable = in_value; }          // ��]�������邩

	// �J�����N���X�̃Z�b�^�[�E�Q�b�^�[
	void SetCamera(class CameraComponent* in_camera) { m_camera = in_camera; }
	class CameraComponent* GetCamera() { return m_camera; }

private:

	AABB m_objectBox;                                                       // �I�u�W�F�N�g���(�ϊ��O)�̃{�b�N�X
	AABB m_worldBox;                                                        // ���[���h��Ԃł̃{�b�N�X

	bool m_rotatable;                                                       // ��]�������邩

	class CameraComponent* m_camera;                                        // �J�����ɓ����蔻���t�����ۂɗp����|�C���^

	friend class PhysicsWorld;                                              // �t�����h��

};