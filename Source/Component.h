//-----------------------------------------------------------------------+
// �R���|�[�l���g�N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include <cstdint>


class Component
{
public:

	Component(class Actor* in_owner, int in_updateOrder = 100);     // �R���X�g���N�^(updateOrder���������قǑ������s�����)

	virtual ~Component();                                           // ���z�f�X�g���N�^

	virtual void Update(float in_deltaTime);                        // �f���^�^�C���ɂ��R���|�[�l���g�̍X�V(�I�[�o�[���C�h��)

	virtual void ProcessInput(float in_deltaTime) {}                                  // �R���|�[�l���g�̓��͏���(�I�[�o�[���C�h��)

	virtual void OnUpdateWorldTransform() {}                        // ���[���h�ϊ����K�v�ȂƂ��ɌĂ΂��֐�

	int GetID() { return m_ID; }                                    // ID�̎擾

	class Actor* GetOwner() { return m_owner; }                     // �I�[�i�[�A�N�^�[�̎擾

	int GetUpdateOrder() const { return m_updateOrder; }            // updateOrder�̎擾



protected:

	class Actor* m_owner;           // �I�[�i�[�A�N�^�[�ւ̃|�C���^

	int m_updateOrder;              // �R���|�[�l���g�̍X�V����

	int m_ID;                       // �R���|�[�l���g�Ǘ�ID

	static int m_globalID;          // �R���|�[�l���g�Ǘ�ID�̘A�ԗp

};