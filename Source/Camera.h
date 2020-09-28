//-----------------------------------------------------------------------+
// �J�����N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Common.h"
#include "Actor.h"

class Camera
{
public:
	
	Camera(Actor* in_targetActor);                             // �R���X�g���N�^
	virtual ~Camera();                                         // �f�X�g���N�^

	virtual void UpdateActor(float in_deltaTime);              // �J�����̍X�V
	void Update(float in_deltaTime);                           // �I�[�o�[���C�h�s��
	void SetActive();
	void Initialize(const Vector3& in_cameraPos, const Vector3& in_targetPos, const Vector3& in_upVec);

	const Vector3& GetPosition() const { return m_position; }

	Matrix4& GetViewMatrix() { return m_viewMatrix; }
	Vector3& GetViewVector() { return m_viewTarget; }

protected:

	void SetViewMatrix(const Matrix4& in_view);

	Actor *m_targetActor;               // ��ƂȂ�A�N�^�[
	Matrix4 m_viewMatrix;               // �r���[�s��
	Vector3 m_position;
	Vector3 m_viewTarget;               // �J�����̒��ړ_
	Vector3 m_viewVector;               // �����x�N�g��
};