//-----------------------------------------------------------------------+
// �A�N�^�[�N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Actor.h"
#include "GameMain.h"
#include "Component.h"
#include <algorithm>
#include <typeinfo>

int Actor::m_globalActorNo = 0;

// �R���X�g���N�^
Actor::Actor()
	:m_state(STATE_ACTIVE)
	,m_position(Vector3::Zero)
	,m_rotation(Quaternion::Identity)
	,m_scale(1.0f)
	,m_speed(0.0f)
	,m_recomputeWorldTransform(true)
	,m_ID(m_globalActorNo)
{
	// �Q�[�����C���ɂ��̃A�N�^�[��ǉ��o�^
	GAME_INSTANCE.AddActor(this);
	// �Q�[���S�̂̃A�N�^�[�ԍ����X�V
	m_globalActorNo++;
}

// �f�X�g���N�^
Actor::~Actor()
{
	// �A�N�^�[����������R���|�[�l���g�̍폜
	while (!m_components.empty())
	{
		delete m_components.back();
	}

	// �Q�[�����C���ɂ��̃A�N�^�[�̍폜���˗�
	GAME_INSTANCE.RemoveActor(this);
}

// �X�V���� (Game����R�[��)
// ���� : in_deltaTime => 1�t���[�����̌o�ߎ���
void Actor::Update(float in_deltaTime)
{
	// �A�N�^�[���A�N�e�B�u�Ȃ�
	if (m_state == STATE_ACTIVE)
	{
		// ���[���h�ϊ��s����v�Z
		ComputeWorldTransform();

		// �R���|�[�l���g�̍X�V����
		UpdateComponents(in_deltaTime);
		// �A�N�^�[�ŗL�̍X�V����
		UpdateActor(in_deltaTime);

		// ���[���h�ϊ��s��̍Čv�Z
		ComputeWorldTransform();
	}
}

// �X�V���� (�R���|�[�l���g)
// ���� : in_deltaTime => 1�t���[�����̌o�ߎ���
void Actor::UpdateComponents(float in_deltaTime)
{
	for (auto comp : m_components)
	{
		comp->Update(in_deltaTime);
	}
}

// �X�V���� (�A�N�^�[�ŗL����:�I�[�o�[���C�h�\)
// ���� : in_deltaTime => 1�t���[�����̌o�ߎ���
void Actor::UpdateActor(float in_deltaTime)
{
}

// ���͏���
void Actor::ProcessInput(float in_deltaTime)
{
	if (m_state == STATE_ACTIVE)
	{
		// ���͏������󂯎��R���|�[�l���g��D�悵�Ď��s
		for (auto comp : m_components)
		{
			comp->ProcessInput(in_deltaTime);
		}
	}
}

// ���[���h�ϊ��s����v�Z
void Actor::ComputeWorldTransform()
{
	// ���[���h�ϊ��̍Čv�Z���K�v�Ȃ���s
	if (m_recomputeWorldTransform)
	{

		// �X�P�[�����O����]�����s�ړ��ƂȂ�悤�ɕϊ��s����쐬
		m_worldTransform = Matrix4::CreateScale(m_scale);
		m_worldTransform *= Matrix4::CreateFromQuaternion(m_rotation);
		m_worldTransform *= Matrix4::CreateTranslation(m_position);

		// �A�N�^�[�������Ă���S�R���|�[�l���g�̕ϊ����s��
		for (auto comp : m_components)
		{
			comp->OnUpdateWorldTransform();
		}

		m_recomputeWorldTransform = false;
	}
}


// �O�i�x�N�g���̍X�V
// in_forward => �����������O�������x�N�g��
void Actor::RotateToNewForward(const Vector3 & in_forward)
{
	// X���x�N�g��(1, 0, 0)��forward�Ԃ̊p�x�����߂�
	float dot = Vector3::Dot(Vector3::UnitX, in_forward);
	float angle = Math::Acos(dot);

	// �������������ꍇ
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// ������������ꍇ
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// ���x�N�g����forward�Ƃ̊O�ς����]�������߂ĉ�]������
		Vector3 axis = Vector3::Cross(Vector3::UnitX, in_forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}


// �R���|�[�l���g�̒ǉ�
void Actor::AddComponent(Component * in_comp)
{
	// �R���|�[�l���g���\�[�g���Ēǉ�
	// �����̃I�[�_�[�ԍ������傫���}���_��������
	int myOrder = in_comp->GetUpdateOrder();
	auto iter = m_components.begin();

	for (; iter != m_components.end(); ++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// �v�f���������|�C���g�̎�O�ɑ}������
	m_components.insert(iter, in_comp);
}

// �R���|�[�l���g�̍폜
void Actor::RemoveComponent(Component * in_comp)
{
	// �����̃R���|�[�l���g���R���|�[�l���g�z�񂩂猟�����A���̃C�e���[�^���폜
	auto iter = std::find(m_components.begin(), m_components.end(), in_comp);
	if (iter != m_components.end())
	{
		m_components.erase(iter);
	}
}
